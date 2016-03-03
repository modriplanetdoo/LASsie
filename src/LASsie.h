#ifndef LASSIE_LASSIE_H
#define LASSIE_LASSIE_H

#include "Types.h"

namespace modri
{
	class LASsie
	{
		public:
			struct Guid
			{
				uint32_t sData1;
				uint16_t sData2;
				uint16_t sData3;
				uint8_t sData4[8];
			};

			enum PointDataFormat
			{
				PointDataFormat0,
				PointDataFormat1,
				PointDataFormat2,
				PointDataFormat3
			};

			template <class tType>
			struct Coord
			{
				tType sX;
				tType sY;
				tType sZ;
			};

		private:
			uint16_t mFileSourceId;
			bool mGlobalEncoding;
			LASsie::Guid mGuid;
			char mGeneratingSoftware[33]; // 32 chars + NULL terminator
			uint16_t mCreationDay;
			uint16_t mCreationYear;
			LASsie::PointDataFormat mPointDataFormat;
			LASsie::Coord<double> mScaleFactor;
			LASsie::Coord<double> mOffset;
			LASsie::Coord<double> mMax;
			LASsie::Coord<double> mMin;

		public:
			inline LASsie() { this->Reset(); }
			inline virtual ~LASsie() { }

			void Reset();

			inline uint16_t GetFileSourceId() const { return this->mFileSourceId; }
			inline void SetFileSourceId(uint16_t nFileSourceId) { this->mFileSourceId = nFileSourceId; }
			inline bool IsGlobalEncoding() const { return this->mGlobalEncoding; }
			inline void SetGlobalEncoding(bool nGlobalEncoding) { this->mGlobalEncoding = nGlobalEncoding; }
			inline const LASsie::Guid &GetGuid() const { return this->mGuid; }
			void SetGuid(const LASsie::Guid &nGuid);
			inline const char *GetGeneratingSoftware() const { return this->mGeneratingSoftware; }
			void SetGeneratingSoftware(const char *nGeneratingSoftware);
			inline uint16_t	GetCreationDay() const { return this->mCreationDay; }
			inline uint16_t GetCreationYear() const { return this->mCreationYear; }
			inline void SetCreation(uint16_t nCreationYear, uint16_t nCreationDay) { this->mCreationYear = nCreationYear; this->mCreationDay = nCreationDay; }
			inline LASsie::PointDataFormat GetPointDataFormat() const { return this->mPointDataFormat; }
			inline void SetPointDataFormat(LASsie::PointDataFormat nPointDataFormat) { this->mPointDataFormat = nPointDataFormat; }
			inline const LASsie::Coord<double> &GetScaleFactor() const { return this->mScaleFactor; }
			inline void SetScaleFactor(double nX, double nY, double nZ) { this->mScaleFactor.sX = nX; this->mScaleFactor.sY = nY; this->mScaleFactor.sZ = nZ; }
			inline const LASsie::Coord<double> &GetOffset() const { return this->mOffset; }
			inline void SetOffset(double nX, double nY, double nZ) { this->mOffset.sX = nX; this->mOffset.sY = nY; this->mOffset.sZ = nZ; }
			inline const LASsie::Coord<double> &GetMax() const { return this->mMax; }
			inline void SetMax(double nX, double nY, double nZ) { this->mMax.sX = nX; this->mMax.sY = nY; this->mMax.sZ = nZ; }
			inline const LASsie::Coord<double> &GetMin() const { return this->mMin; }
			inline void SetMin(double nX, double nY, double nZ) { this->mMin.sX = nX; this->mMin.sY = nY; this->mMin.sZ = nZ; }
		};
}

#endif // LASSIE_LASSIE_H
