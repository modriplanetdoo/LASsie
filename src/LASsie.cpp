#include "LASsie.h"

#include <string.h>

static const char lSystemIdentifier[33] = "LASsie Library v1.0\0\0\0\0\0\0\0\0\0\0\0\0\0"; // NOTE: Has to be NULL-filled to the very brim

void modri::LASsie::Reset()
{
	this->mFileSourceId = 0;
	this->mGlobalEncoding = false;
	memset(&this->mGuid, 0, sizeof(this->mGuid));
	memset(this->mGeneratingSoftware, 0, sizeof(this->mGeneratingSoftware));
	this->mCreationDay = 0;
	this->mCreationYear = 0;
	this->mPointDataFormat = LASsie::PointDataFormat0;
	memset(&this->mScaleFactor, 0, sizeof(this->mScaleFactor));
	memset(&this->mOffset, 0, sizeof(this->mOffset));
	memset(&this->mMax, 0, sizeof(this->mMax));
	memset(&this->mMin, 0, sizeof(this->mMin));
}

void modri::LASsie::SetGuid(const LASsie::Guid &nGuid)
{
	memcpy(&this->mGuid, &nGuid, sizeof(this->mGuid));
}

void modri::LASsie::SetGeneratingSoftware(const char *nGeneratingSoftware)
{
	if (nGeneratingSoftware != NULL)
	{
		for (size_t i = 0; i < (sizeof(this->mGeneratingSoftware) - 1); i++)
		{
			this->mGeneratingSoftware[i] = *nGeneratingSoftware;
			if (*nGeneratingSoftware != '\0') // this is like that because we need to fill NULLs in the entire mGeneratingSoftware buffer
				nGeneratingSoftware++;
		}
		this->mGeneratingSoftware[sizeof(this->mGeneratingSoftware) - 1] = '\0';
	}
}
