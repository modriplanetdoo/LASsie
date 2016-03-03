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
