#include "LASsie.h"

#include <string.h>

static const char lSystemIdentifier[33] = "LASsie Library v1.0\0\0\0\0\0\0\0\0\0\0\0\0\0"; // NOTE: Has to be NULL-filled to the very brim


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
	this->mScanFields = 0;
	this->mClassif = 0;
	this->mScanAngle = 0;
	this->mUserData = 0;
	this->mPointSrcId = 0;
	this->mGpsTime = 0.0;
	memset(&this->mColor, 0, sizeof(this->mColor));
}

modri::uint8 modri::LASsie::PointDataRec::GetRetNum() const
{
	return (this->mScanFields & 0x07);
}

void modri::LASsie::PointDataRec::SetRetNum(modri::uint8 nRetNum)
{
	this->mScanFields = ((this->mScanFields & ~0x07) | (nRetNum & 0x07));
}

modri::uint8 modri::LASsie::PointDataRec::GetRetTotal() const
{
	return ((this->mScanFields >> 3) & 0x07);
}

void modri::LASsie::PointDataRec::SetRetTotal(modri::uint8 nRetTotal)
{
	this->mScanFields = ((this->mScanFields & ~0x38) | ((nRetTotal & 0x07) << 3));
}

bool modri::LASsie::PointDataRec::HasScanDirFlag() const
{
	return ((this->mScanFields & 0x40) != 0);
}

void modri::LASsie::PointDataRec::SetScanDirFlag(bool nScanDirFlag)
{
	if (nScanDirFlag == true)
		this->mScanFields |= 0x40;
	else
		this->mScanFields &= ~0x40;
}

bool modri::LASsie::PointDataRec::IsFlightEdge() const
{
	return ((this->mScanFields & 0x80) != 0);
}

void modri::LASsie::PointDataRec::SetFlightEdge(bool nFlightEdge)
{
	if (nFlightEdge == true)
		this->mScanFields |= 0x80;
	else
		this->mScanFields &= ~0x80;
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
			if (*nNewStr != '\0') // this is like that because we need to fill NULLs in the entire mGenerSw buffer
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
