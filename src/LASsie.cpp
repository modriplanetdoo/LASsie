#include "LASsie.h"

#include <string.h>

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
	this->mPdrf = LASsie::Pdrf0;
	memset(&this->mScale, 0, sizeof(this->mScale));
	memset(&this->mOffset, 0, sizeof(this->mOffset));
	memset(&this->mMax, 0, sizeof(this->mMax));
	memset(&this->mMin, 0, sizeof(this->mMin));

	this->mRecProvider = NULL;
}

void modri::LASsie::SetGuid(const LASsie::Guid &nGuid)
{
	memcpy(&this->mGuid, &nGuid, sizeof(this->mGuid));
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
