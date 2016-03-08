#include "LASsie.h"

#include <string.h>
#include <stdio.h>

static const char lSystemIdentifier[33] = "LASsie Library v1.0\0\0\0\0\0\0\0\0\0\0\0\0\0"; // NOTE: Has to be NULL-filled to the very brim

static const size_t lPublicHeaderSize =
	 4 + // File Signature ("LASF")
	 2 + // File Source ID
	 2 + // Global Encoding
	 4 + // GUID data 1
	 2 + // GUID data 2
	 2 + // GUID data 3
	 8 + // GUID data 4
	 1 + // Version Major
	 1 + // Version Minor
	32 + // System Identifier
	32 + // Generating Software
	 2 + // File Creation Day of Year
	 2 + // File Creation Year
	 2 + // Header Size
	 4 + // Offset to point data
	 4 + // Number of Variable Length Records
	 1 + // Point Data Format ID
	 2 + // Point Data Record Length
	 4 + // Number of point records
	20 + // Number of points by return (5 * 4_bytes)
	 8 + // X scale factor
	 8 + // Y scale factor
	 8 + // Z scale factor
	 8 + // X offset
	 8 + // Y offset
	 8 + // Z offset
	 8 + // Max X
	 8 + // Min X
	 8 + // Max Y
	 8 + // Min Y
	 8 + // Max Z
	 8 + // Min Z
	 0;  // === END ===

static const size_t lVarLenRecHeaderSize =
	 2 + // Reserved
	16 + // User ID
	 2 + // Record ID
	 2 + // Record Length After Header
	32 + // Description
	 0;  // === END ===

static const size_t lGeoKeysHeaderSize = 
	 2 + // Key Directory Version
	 2 + // Key Revision
	 2 + // Minor Revision
	 2 + // Number Of Keys
	 0;  // === END ===

static const size_t lGeoKeysEntrySize = 
	 2 + // Key ID
	 2 + // TIFF Tag Location
	 2 + // Count
	 2 + // Value Offset
	 0;  // === END ===

static const size_t lPointDataRecSizeBase =
	 4 + // X
	 4 + // Y
	 4 + // Z
	 2 + // Intensity
	 1 + // Return Number (bits=0..2), Number of Returns (bits=3..5), Scan Direction Flag (bit=6), Edge of Flight Line (bit=7)
	 1 + // Classification
	 1 + // Scan Angle Rank
	 1 + // User Data
	 2 + // Point Source ID
	 0;  // === END ===

static const size_t lPointDataRecSizeExt0 = // Format 0 additional fields (none)
	 0;  // === END ===

static const size_t lPointDataRecSizeExt1 = // Format 1 additional fields
	 8 + // GPS Time
	 0;  // === END ===

static const size_t lPointDataRecSizeExt2 = // Format 2 additional fields
	 2 + // Red
	 2 + // Green
	 2 + // Blue
	 0;  // === END ===

static const size_t lPointDataRecSizeExt3 = // Format 3 additional fields
	 8 + // GPS Time
	 2 + // Red
	 2 + // Green
	 2 + // Blue
	 0;  // === END ===

static const size_t lPointDataRecFmt0Size = lPointDataRecSizeBase + lPointDataRecSizeExt0;
static const size_t lPointDataRecFmt1Size = lPointDataRecSizeBase + lPointDataRecSizeExt1;
static const size_t lPointDataRecFmt2Size = lPointDataRecSizeBase + lPointDataRecSizeExt2;
static const size_t lPointDataRecFmt3Size = lPointDataRecSizeBase + lPointDataRecSizeExt3;

static const size_t lPointDataRecSizes[4] =
{
	lPointDataRecFmt0Size,
	lPointDataRecFmt1Size,
	lPointDataRecFmt2Size,
	lPointDataRecFmt3Size
};


static inline bool _local_IsLe()
{
	static const union { modri::uint16 uVal; modri::uint8 uByte[2]; } oUni = { 0xFEDC };
	return (oUni.uByte[0] == 0xDC);
}

template <class tType>
static inline void _local_WriteLe(modri::uint8 *nBfr, tType nVal) // nBfr needs to have at least as much space as sizeof(nVal)
{
	if (_local_IsLe() == true)
		memcpy(nBfr, &nVal, sizeof(nVal));
	else
	{
		for (size_t i = sizeof(nVal); i != 0;)
			nBfr[sizeof(nVal) - i] = reinterpret_cast<modri::uint8 *>(&nVal)[--i]; // NOTE: i is decreased here
	}
}

template <class tType>
static inline void _local_WriteLeAdv(modri::uint8 *&nBfr, tType nVal) // NOTE: This function will advance nBfr pointer for sizeof(nVal) bytes
{
	_local_WriteLe(nBfr, nVal);
	nBfr += sizeof(tType);
}

static inline void _local_MemcpyAdv(modri::uint8 *&nBfr, const void *nSrc, size_t nSrcSize) // NOTE: This function will advance nBfr pointer for sizeof(nVal) bytes
{
	memcpy(nBfr, nSrc, nSrcSize);
	nBfr += nSrcSize;
}

static inline void _local_WriteDoubleAdv(modri::uint8 *&nBfr, double nVal)
{
	_local_MemcpyAdv(nBfr, &nVal, sizeof(nVal));
}

typedef modri::uint8 _local_VarLenRecHeader[lVarLenRecHeaderSize];
static inline void _local_FillVarLenRecHeader(_local_VarLenRecHeader &nVlrHeader, const modri::LASsie::VarLenRec &nVlr, size_t nDataSize)
{
	nVlrHeader[ 0] = 0x00;
	nVlrHeader[ 1] = 0x00;
	memcpy((nVlrHeader +  2), nVlr.UserId().Get(), 16);
	nVlrHeader[18] = static_cast<modri::uint8>(nVlr.GetRecId());
	nVlrHeader[19] = static_cast<modri::uint8>(nVlr.GetRecId() >> 8);
	nVlrHeader[20] = static_cast<modri::uint8>(nDataSize);
	nVlrHeader[21] = static_cast<modri::uint8>(nDataSize >> 8);
	memcpy((nVlrHeader + 22), nVlr.Desc().Get(), 32);
}

typedef modri::uint8 _local_GeoKeyHeader[lGeoKeysHeaderSize];
static inline void _local_FillGeoKeyHeader(_local_GeoKeyHeader &nGkHeader, size_t nEntryCount)
{
	nGkHeader[0] = 0x01;
	nGkHeader[1] = 0x00;
	nGkHeader[2] = 0x01;
	nGkHeader[3] = 0x00;
	nGkHeader[4] = 0x00;
	nGkHeader[5] = 0x00;
	nGkHeader[6] = static_cast<modri::uint8>(nEntryCount);
	nGkHeader[7] = static_cast<modri::uint8>(nEntryCount >> 8);
}

typedef modri::uint8 _local_GeoKeyEntry[lGeoKeysEntrySize];
static inline void _local_FillGeoKeyEntry(_local_GeoKeyEntry &nGkEntry, const modri::LASsie::GeoKey &nKey)
{
	nGkEntry[0] = static_cast<modri::uint8>(nKey.GetKeyId());
	nGkEntry[1] = static_cast<modri::uint8>(nKey.GetKeyId() >> 8);
	nGkEntry[2] = static_cast<modri::uint8>(nKey.GetTagLocat());
	nGkEntry[3] = static_cast<modri::uint8>(nKey.GetTagLocat() >> 8);
	nGkEntry[4] = static_cast<modri::uint8>(nKey.GetCount());
	nGkEntry[5] = static_cast<modri::uint8>(nKey.GetCount() >> 8);
	nGkEntry[6] = static_cast<modri::uint8>(nKey.GetValOffset());
	nGkEntry[7] = static_cast<modri::uint8>(nKey.GetValOffset() >> 8);
}


// 
// LASsie
// 

void modri::LASsie::Reset()
{
	this->mFileSrcId = 0;
	this->mGlobalEnc = false;
	memset(&this->mGuid, 0, sizeof(this->mGuid));
	this->mGenerSw.Set("");
	this->mCreatDay = 0;
	this->mCreatYear = 0;
	this->mPdrFormat = LASsie::pdrFormat0;
	memset(&this->mScale, 0, sizeof(this->mScale));
	memset(&this->mOffset, 0, sizeof(this->mOffset));
	memset(&this->mMax, 0, sizeof(this->mMax));
	memset(&this->mMin, 0, sizeof(this->mMin));

	this->mRecProvider = NULL;
	this->mInout = NULL;
	
	this->mLastError = LASsie::leNone;
}

void modri::LASsie::SetGuid(const LASsie::Guid &nGuid)
{
	memcpy(&this->mGuid, &nGuid, sizeof(this->mGuid));
}

bool modri::LASsie::Generate()
{
	if (this->mInout == NULL)
	{
		this->mLastError = LASsie::leNoInoutIface;
		return false;
	}

	modri::uint8 oPubHeader[lPublicHeaderSize];
	modri::uint8 *oCurPos = oPubHeader;

	// File signature
	oPubHeader[0] = 'L';
	oPubHeader[1] = 'A';
	oPubHeader[2] = 'S';
	oPubHeader[3] = 'F';
	oCurPos += 4;

	_local_WriteLeAdv(oCurPos, this->mFileSrcId); // File Source ID
	_local_WriteLeAdv(oCurPos, static_cast<modri::uint16>(((this->mGlobalEnc == true) ? 0x0001 : 0x0000))); // Global Encoding
	_local_WriteLeAdv(oCurPos, this->mGuid.sD1);  // GUID data 1
	_local_WriteLeAdv(oCurPos, this->mGuid.sD2);  // GUID data 2
	_local_WriteLeAdv(oCurPos, this->mGuid.sD3);  // GUID data 3
	_local_MemcpyAdv(oCurPos, this->mGuid.sD4, sizeof(this->mGuid.sD4));   // GUID data 4
	*oCurPos++ = 1; // (LASF) Version Major
	*oCurPos++ = 2; // (LASF) Version Minor
	_local_MemcpyAdv(oCurPos, lSystemIdentifier, (sizeof(lSystemIdentifier) - 1)); // System Identifier
	memcpy(oCurPos, this->mGenerSw.Get(), 32); // Generating Software
	oCurPos += 32;
	_local_WriteLeAdv(oCurPos, this->mCreatDay);     // File Creation Day of Year
	_local_WriteLeAdv(oCurPos, this->mCreatYear);    // File Creation Year
	_local_WriteLeAdv(oCurPos, static_cast<modri::uint16>(lPublicHeaderSize)); // Header Size


	// VLR values

	modri::uint32 oGeoKeyDataSize = static_cast<modri::uint32>(lGeoKeysHeaderSize);
	modri::uint32 oPdOffset =
		static_cast<modri::uint32>(lPublicHeaderSize) +
		static_cast<modri::uint32>(lVarLenRecHeaderSize) +
		oGeoKeyDataSize; // At least public header + GeoKeyDirectoryTag with its key entries
	modri::uint32 oVlrCount = 1; // At least one, which is the mandatory GeoKeyDirectoryTag record

	if (this->mRecProvider != NULL)
	{
		oGeoKeyDataSize += static_cast<modri::uint32>(this->mRecProvider->GetGeoKeyCount() * lGeoKeysEntrySize);
		oPdOffset += static_cast<modri::uint32>(this->mRecProvider->GetGeoKeyCount() * lGeoKeysEntrySize);

		oVlrCount += static_cast<modri::uint32>(this->mRecProvider->GetVarLenRecCount());
		for (size_t i = (oVlrCount - 1); i-- != 0;)
			oPdOffset += 
				static_cast<modri::uint32>(lVarLenRecHeaderSize) +
				static_cast<modri::uint32>(this->mRecProvider->GetVarLenRecDataSize(i));
	}

	_local_WriteLeAdv(oCurPos, oPdOffset); // Offset to point data
	_local_WriteLeAdv(oCurPos, oVlrCount); // Number of Variable Length Records 


	*oCurPos++ = static_cast<modri::uint8>(this->mPdrFormat); // Point Data Format ID
	_local_WriteLeAdv(oCurPos, static_cast<modri::uint16>(lPointDataRecSizes[static_cast<modri::uint8>(this->mPdrFormat)]));


	// PDR values

	size_t oPdrCount = 0; // values will be checked later
	size_t oPdrCountByRet[5]; // values will be checked later
	oPdrCountByRet[0] = 0;
	oPdrCountByRet[1] = 0;
	oPdrCountByRet[2] = 0;
	oPdrCountByRet[3] = 0;
	oPdrCountByRet[4] = 0;

	if (this->mRecProvider != NULL)
	{
		oPdrCount = this->mRecProvider->GetPointDataRecCount();
		oPdrCountByRet[0] = this->mRecProvider->GetPointDataRecCountByRet(0);
		oPdrCountByRet[1] = this->mRecProvider->GetPointDataRecCountByRet(1);
		oPdrCountByRet[2] = this->mRecProvider->GetPointDataRecCountByRet(2);
		oPdrCountByRet[3] = this->mRecProvider->GetPointDataRecCountByRet(3);
		oPdrCountByRet[4] = this->mRecProvider->GetPointDataRecCountByRet(4);

		if (oPdrCount > 0xFFFFFFFF ||
			oPdrCountByRet[0] > 0xFFFFFFFF ||
			oPdrCountByRet[1] > 0xFFFFFFFF ||
			oPdrCountByRet[2] > 0xFFFFFFFF ||
			oPdrCountByRet[3] > 0xFFFFFFFF ||
			oPdrCountByRet[4] > 0xFFFFFFFF)
		{
			this->mLastError = LASsie::lePdrCountTooBig;
			return false;
		}
	}

	_local_WriteLeAdv(oCurPos, static_cast<modri::uint32>(oPdrCount)); // Number of point records 
	_local_WriteLeAdv(oCurPos, static_cast<modri::uint32>(oPdrCountByRet[0])); // Number of points by return [0]
	_local_WriteLeAdv(oCurPos, static_cast<modri::uint32>(oPdrCountByRet[1])); // Number of points by return [1]
	_local_WriteLeAdv(oCurPos, static_cast<modri::uint32>(oPdrCountByRet[2])); // Number of points by return [2]
	_local_WriteLeAdv(oCurPos, static_cast<modri::uint32>(oPdrCountByRet[3])); // Number of points by return [3]
	_local_WriteLeAdv(oCurPos, static_cast<modri::uint32>(oPdrCountByRet[4])); // Number of points by return [4]


	_local_WriteDoubleAdv(oCurPos, this->mScale.sX);
	_local_WriteDoubleAdv(oCurPos, this->mScale.sY);
	_local_WriteDoubleAdv(oCurPos, this->mScale.sZ);
	_local_WriteDoubleAdv(oCurPos, this->mOffset.sX);
	_local_WriteDoubleAdv(oCurPos, this->mOffset.sY);
	_local_WriteDoubleAdv(oCurPos, this->mOffset.sZ);
	_local_WriteDoubleAdv(oCurPos, this->mMax.sX);
	_local_WriteDoubleAdv(oCurPos, this->mMin.sX);
	_local_WriteDoubleAdv(oCurPos, this->mMax.sY);
	_local_WriteDoubleAdv(oCurPos, this->mMin.sY);
	_local_WriteDoubleAdv(oCurPos, this->mMax.sZ);
	_local_WriteDoubleAdv(oCurPos, this->mMin.sZ);


	// Write Public Header Block
	if (this->mInout->Write(oPubHeader, sizeof(oPubHeader)) != true)
	{
		this->mLastError = LASsie::leWriteFail;
		return false;
	}


	// Write (mandatory) GeoKeyDirectoryTag VLR
	if (oGeoKeyDataSize > 0xFFFF) // Max (2^16 - 1) size of geo keys data
	{
		this->mLastError = LASsie::leGeoKeysSizeTooBig;
		return false;
	}
	
	_local_VarLenRecHeader oVlrHeader;
	modri::LASsie::VarLenRec oVlr;

	oVlr.UserId().Set("LASF_Projection");
	oVlr.SetRecId(34735);
	_local_FillVarLenRecHeader(oVlrHeader, oVlr, oGeoKeyDataSize);
	if (this->mInout->Write(oVlrHeader, sizeof(oVlrHeader)) != true)
	{
		this->mLastError = LASsie::leWriteFail;
		return false;
	}


	_local_GeoKeyHeader oGkHeader;
	if (this->mRecProvider == NULL)
	{
		// No record provider; write empty GK header and return
		_local_FillGeoKeyHeader(oGkHeader, 0);
		if (this->mInout->Write(oGkHeader, sizeof(oGkHeader)) != true)
		{
			this->mLastError = LASsie::leWriteFail;
			return false;
		}
		return true;
	}
	

	// Write GKs
	_local_FillGeoKeyHeader(oGkHeader, this->mRecProvider->GetGeoKeyCount());
	if (this->mInout->Write(oGkHeader, sizeof(oGkHeader)) != true)
	{
		this->mLastError = LASsie::leWriteFail;
		return false;
	}

	LASsie::GeoKey oGk;
	_local_GeoKeyEntry oGkEntry;
	for (size_t i = 0; i < this->mRecProvider->GetGeoKeyCount(); i++)
	{
		oGk.Reset();
		if (this->mRecProvider->FillGeoKey(i, oGk) != true)
		{
			this->mLastError = LASsie::leGeoKeyFillFail;
			return false;
		}

		_local_FillGeoKeyEntry(oGkEntry, oGk);
		if (this->mInout->Write(oGkEntry, sizeof(oGkEntry)) != true)
		{
			this->mLastError = LASsie::leWriteFail;
			return false;
		}
	}


	// Write VLRs
	const void *oVlrData;
	size_t oVlrDataSize;
	for (size_t i = 0; i < this->mRecProvider->GetVarLenRecCount(); i++)
	{
		oVlr.Reset();
		if (this->mRecProvider->FillVarLenRec(i, oVlr) != true)
		{
			this->mLastError = LASsie::leVlrFillFail;
			return false;
		}
		oVlrData = this->mRecProvider->GetVarLenRecData(i);
		oVlrDataSize = this->mRecProvider->GetVarLenRecDataSize(i);

		_local_FillVarLenRecHeader(oVlrHeader, oVlr, oVlrDataSize);
		if (this->mInout->Write(oVlrHeader, sizeof(oVlrHeader)) != true ||
			this->mInout->Write(oVlrData, oVlrDataSize) != true)
		{
			this->mLastError = LASsie::leWriteFail;
			return false;
		}
	}


	// Write PDRs
	for (size_t i = 0; i < this->mRecProvider->GetPointDataRecCount(); i++)
	{
		
	}


	return true;
}


// 
// LASsie::GeoKey
// 

void modri::LASsie::GeoKey::Reset()
{
	this->mKeyId = 0;
	this->mTagLocat = 0;
	this->mCount = 0;
	this->mValOffset = 0;
}


// 
// LASsie::VarLenRec
// 

void modri::LASsie::VarLenRec::Reset()
{
	this->mUserId.Set("");
	this->mRecId = 0;
	this->mDesc.Set("");
}


// 
// LASsie::PointDataRec
// 

void modri::LASsie::PointDataRec::Reset()
{
	memset(&this->mCoord, 0, sizeof(this->mCoord));
	this->mInten = 0;
	this->mRetNum = 0;
	this->mRetTotal = 0;
	this->mScanDirFlag = false;
	this->mFlightEdge = false;
	this->mClassif = 0;
	this->mScanAngle = 0;
	this->mUserData = 0;
	this->mPointSrcId = 0;
	this->mGpsTime = 0.0;
	memset(&this->mColor, 0, sizeof(this->mColor));
}


// 
// LASsie::StringHelper
// 

void modri::LASsie::StringHelper::Set(char *nStr, size_t nStrSize, const char *nNewStr)
{
	if (nNewStr != NULL && *nNewStr != '\0')
	{
		for (size_t i = 0; i < (nStrSize - 1); i++)
		{
			nStr[i] = *nNewStr;
			if (*nNewStr != '\0') // it's like this because we need to fill NULLs in the entire nStr buffer
				nNewStr++;
		}
		nStr[nStrSize - 1] = '\0';
	}
	else
		memset(nStr, 0, nStrSize);
}

size_t modri::LASsie::StringHelper::Len(const char *nStr, size_t nStrSize)
{
	for (size_t i = 0; i < (nStrSize - 1); i++)
		if (nStr[i] == '\0')
			return i;

	return (nStrSize - 1);
}
