#include "LASsie.h"

#include <string.h>

static const char lSystemIdentifier[33] = "LASsie Library v1.0\0\0\0\0\0\0\0\0\0\0\0\0\0"; // NOTE: Has to be NULL-filled to the very brim

void modri::LASsie::Reset()
{
	this->mFileSrcId = 0;
	this->mGlobalEnc = false;
	memset(&this->mGuid, 0, sizeof(this->mGuid));
	memset(this->mGenerSw, 0, sizeof(this->mGenerSw));
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

void modri::LASsie::SetGenerSw(const char *nGenerSw)
{
	if (nGenerSw != NULL)
	{
		for (size_t i = 0; i < (sizeof(this->mGenerSw) - 1); i++)
		{
			this->mGenerSw[i] = *nGenerSw;
			if (*nGenerSw != '\0') // this is like that because we need to fill NULLs in the entire mGenerSw buffer
				nGenerSw++;
		}
		this->mGenerSw[sizeof(this->mGenerSw) - 1] = '\0';
	}
}
