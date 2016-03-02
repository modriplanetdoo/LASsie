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

			void Reset() { }
	};
}

#endif // LASSIE_LASSIE_H
