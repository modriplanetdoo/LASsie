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
				modri::uint32 sD1;
				modri::uint16 sD2;
				modri::uint16 sD3;
				modri::uint8 sD4[8];
			};

			enum Pdrf
			{
				Pdrf0,
				Pdrf1,
				Pdrf2,
				Pdrf3
			};

			template <class tType>
			struct Coord
			{
				tType sX;
				tType sY;
				tType sZ;
			};

			struct Color
			{
				modri::uint16 sR;
				modri::uint16 sG;
				modri::uint16 sB;
			};

		private:
			modri::uint16 mFileSrcId;
			bool mGlobalEnc;
			LASsie::Guid mGuid;
			char mGenerSw[33]; // 32 chars + NULL terminator
			modri::uint16 mCreatDay;
			modri::uint16 mCreatYear;
			LASsie::Pdrf mPdrf;
			LASsie::Coord<double> mScale;
			LASsie::Coord<double> mOffset;
			LASsie::Coord<double> mMax;
			LASsie::Coord<double> mMin;

		public:
			inline LASsie() { this->Reset(); }
			inline virtual ~LASsie() { }

			void Reset();

			inline modri::uint16 GetFileSrcId() const { return this->mFileSrcId; }
			inline void SetFileSrcId(modri::uint16 nFileSrcId) { this->mFileSrcId = nFileSrcId; }
			inline bool IsGlobalEnc() const { return this->mGlobalEnc; }
			inline void SetGlobalEnc(bool nGlobalEnc) { this->mGlobalEnc = nGlobalEnc; }
			inline const LASsie::Guid &GetGuid() const { return this->mGuid; }
			void SetGuid(const LASsie::Guid &nGuid);
			inline const char *GetGenerSw() const { return this->mGenerSw; }
			void SetGenerSw(const char *nGenerSw);
			inline modri::uint16	GetCreatDay() const { return this->mCreatDay; }
			inline modri::uint16 GetCreatYear() const { return this->mCreatYear; }
			inline void SetCreat(modri::uint16 nCreatYear, modri::uint16 nCreatDay) { this->mCreatYear = nCreatYear; this->mCreatDay = nCreatDay; }
			inline LASsie::Pdrf GetPdrf() const { return this->mPdrf; }
			inline void SetPdrf(LASsie::Pdrf nPdrf) { this->mPdrf = nPdrf; }
			inline const LASsie::Coord<double> &GetScale() const { return this->mScale; }
			inline void SetScale(double nX, double nY, double nZ) { this->mScale.sX = nX; this->mScale.sY = nY; this->mScale.sZ = nZ; }
			inline const LASsie::Coord<double> &GetOffset() const { return this->mOffset; }
			inline void SetOffset(double nX, double nY, double nZ) { this->mOffset.sX = nX; this->mOffset.sY = nY; this->mOffset.sZ = nZ; }
			inline const LASsie::Coord<double> &GetMax() const { return this->mMax; }
			inline void SetMax(double nX, double nY, double nZ) { this->mMax.sX = nX; this->mMax.sY = nY; this->mMax.sZ = nZ; }
			inline const LASsie::Coord<double> &GetMin() const { return this->mMin; }
			inline void SetMin(double nX, double nY, double nZ) { this->mMin.sX = nX; this->mMin.sY = nY; this->mMin.sZ = nZ; }
		};
}

#endif // MODRI_LASSIE_LASSIE_H
