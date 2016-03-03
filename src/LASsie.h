#ifndef MODRI_LASSIE_LASSIE_H
#define MODRI_LASSIE_LASSIE_H

#include "Types.h"

namespace modri
{
	class LASsie
	{
		public:
			struct Guid
			{
				modri::uint32 sData1;
				modri::uint16 sData2;
				modri::uint16 sData3;
				modri::uint8 sData4[8];
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
			modri::uint16 mFileSourceId;
			bool mGlobalEncoding;
			LASsie::Guid mGuid;
			char mGeneratingSoftware[33]; // 32 chars + NULL terminator
			modri::uint16 mCreationDay;
			modri::uint16 mCreationYear;
			LASsie::PointDataFormat mPointDataFormat;
			LASsie::Coord<double> mScaleFactor;
			LASsie::Coord<double> mOffset;
			LASsie::Coord<double> mMax;
			LASsie::Coord<double> mMin;

		public:
			inline LASsie() { this->Reset(); }
			inline virtual ~LASsie() { }

			void Reset();

			inline modri::uint16 GetFileSourceId() const { return this->mFileSourceId; }
			inline void SetFileSourceId(modri::uint16 nFileSourceId) { this->mFileSourceId = nFileSourceId; }
			inline bool IsGlobalEncoding() const { return this->mGlobalEncoding; }
			inline void SetGlobalEncoding(bool nGlobalEncoding) { this->mGlobalEncoding = nGlobalEncoding; }
			inline const LASsie::Guid &GetGuid() const { return this->mGuid; }
			void SetGuid(const LASsie::Guid &nGuid);
			inline const char *GetGeneratingSoftware() const { return this->mGeneratingSoftware; }
			void SetGeneratingSoftware(const char *nGeneratingSoftware);
			inline modri::uint16	GetCreationDay() const { return this->mCreationDay; }
			inline modri::uint16 GetCreationYear() const { return this->mCreationYear; }
			inline void SetCreation(modri::uint16 nCreationYear, modri::uint16 nCreationDay) { this->mCreationYear = nCreationYear; this->mCreationDay = nCreationDay; }
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

#endif // MODRI_LASSIE_LASSIE_H
