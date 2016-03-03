#ifndef MODRI_LASSIE_LASSIE_H
#define MODRI_LASSIE_LASSIE_H

#include "Types.h"

namespace modri
{
	class LASsie
	{
		public:
			class StringHelper
			{
			public:
				static void Set(char *nStr, size_t nStrSize, const char *nNewStr);
				static size_t Len(const char *nStr, size_t nStrSize);
			};
			
			template <size_t tSize>
			class String
			{
				private:
					char mStr[tSize + 1]; // tSize + NULL terminator

				public:
					inline String() { this->Set(""); }
					inline virtual ~String() { }

					inline size_t Size() const { return tSize; }
					inline size_t Len() const { return LASsie::StringHelper::Len(this->mStr, sizeof(this->mStr)); }
					inline const char *Get() const { return this->mStr; }
					inline void Set(const char *nNewStr) { LASsie::StringHelper::Set(this->mStr, sizeof(this->mStr), nNewStr); }
			};
			
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

			typedef LASsie::String<32> GenerSwType;
			typedef LASsie::Coord<double> ScaleType;
			typedef LASsie::Coord<double> OffsetType;
			typedef LASsie::Coord<double> CoordEdgeType;

			class VarLenRec
			{
				public:
					typedef LASsie::String<16> UserIdType;
					typedef LASsie::String<32> DescType;
				
				private:
					VarLenRec::UserIdType mUserId;
					modri::uint16 mRecId;
					VarLenRec::DescType mDesc;
				
				public:
					inline VarLenRec() { this->Reset(); }
					inline virtual ~VarLenRec() { }

					void Reset();

					inline const VarLenRec::UserIdType &UserId() const { return this->mUserId; }
					inline VarLenRec::UserIdType &UserId() { return this->mUserId; }
					inline modri::uint16 GetRecId() const { return this->mRecId; }
					inline void SetRecId(modri::uint16 nRecId) { this->mRecId = nRecId; }
					inline const VarLenRec::DescType &Desc() const { return this->mDesc; }
					inline VarLenRec::DescType &Desc() { return this->mDesc; }
			};

			class PointDataRec
			{
				typedef LASsie::Coord<modri::sint32> CoordType;

				private:
					PointDataRec::CoordType mCoord;
					modri::uint16 mInten;
					modri::uint8 mScanFields; // Return Number (0..2), Number of Returns (3..5), Scan Direction Flag(6), and Edge of Flight Line(7)
					modri::uint8 mClassif;
					modri::uint8 mScanAngle;
					modri::uint8 mUserData;
					modri::uint16 mPointSrcId;
					double mGpsTime;
					LASsie::Color mColor;

				public:
					inline PointDataRec() { this->Reset(); }
					inline virtual ~PointDataRec() { }

					void Reset();

					inline const PointDataRec::CoordType &GetCoord() const { return this->mCoord; }
					inline void SetCoord(modri::sint32 nX, modri::sint32 nY, modri::sint32 nZ) { this->mCoord.sX = nX; this->mCoord.sY = nY; this->mCoord.sZ = nZ; }
					inline modri::uint16 GetInten() const { return this->mInten; }
					inline void SetInten(modri::uint16 nInten) { this->mInten = nInten; }
					modri::uint8 GetRetNum() const;
					void SetRetNum(modri::uint8 nRetNum);
					modri::uint8 GetRetTotal() const;
					void SetRetTotal(modri::uint8 nRetTotal);
					bool HasScanDirFlag() const;
					void SetScanDirFlag(bool nScanDirFlag);
					bool IsFlightEdge() const;
					void SetFlightEdge(bool nFlightEdge);
					inline modri::uint8 GetClassif() const { return this->mClassif; }
					inline void SetClassif(modri::uint8 nClassif) { this->mClassif = nClassif; }
					inline modri::uint8 GetScanAngle() const { return this->mScanAngle; }
					inline void SetScanAngle(modri::uint8 nScanAngle) { this->mScanAngle = nScanAngle; }
					inline modri::uint8 GetUserData() const { return this->mUserData; }
					inline void SetUserData(modri::uint8 nUserData) { this->mUserData = nUserData; }
					inline modri::uint16 GetPointSrcId() const { this->mPointSrcId; }
					inline void SetPointSrcId(modri::uint16 nPointSrcId) { this->mPointSrcId = nPointSrcId; }
					inline double GetGpsTime() const { return this->mGpsTime; }
					inline void SetGpsTime(double nGpsTime) { this->mGpsTime = nGpsTime; }
					inline const LASsie::Color &GetColor() const { return this->mColor; }
					inline LASsie::Color &GetColor() { return this->mColor; }
			};

		private:
			modri::uint16 mFileSrcId;
			bool mGlobalEnc;
			LASsie::Guid mGuid;
			LASsie::GenerSwType mGenerSw;
			modri::uint16 mCreatDay;
			modri::uint16 mCreatYear;
			LASsie::Pdrf mPdrf;
			LASsie::ScaleType mScale;
			LASsie::OffsetType mOffset;
			LASsie::CoordEdgeType mMax;
			LASsie::CoordEdgeType mMin;

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
			inline const LASsie::GenerSwType &GenerSw() const { return this->mGenerSw; }
			inline LASsie::GenerSwType &GenerSw() { return this->mGenerSw; }
			inline modri::uint16	GetCreatDay() const { return this->mCreatDay; }
			inline modri::uint16 GetCreatYear() const { return this->mCreatYear; }
			inline void SetCreat(modri::uint16 nCreatYear, modri::uint16 nCreatDay) { this->mCreatYear = nCreatYear; this->mCreatDay = nCreatDay; }
			inline LASsie::Pdrf GetPdrf() const { return this->mPdrf; }
			inline void SetPdrf(LASsie::Pdrf nPdrf) { this->mPdrf = nPdrf; }
			inline const LASsie::ScaleType &GetScale() const { return this->mScale; }
			inline void SetScale(double nX, double nY, double nZ) { this->mScale.sX = nX; this->mScale.sY = nY; this->mScale.sZ = nZ; }
			inline const LASsie::OffsetType &GetOffset() const { return this->mOffset; }
			inline void SetOffset(double nX, double nY, double nZ) { this->mOffset.sX = nX; this->mOffset.sY = nY; this->mOffset.sZ = nZ; }
			inline const LASsie::CoordEdgeType &GetMax() const { return this->mMax; }
			inline void SetMax(double nX, double nY, double nZ) { this->mMax.sX = nX; this->mMax.sY = nY; this->mMax.sZ = nZ; }
			inline const LASsie::CoordEdgeType &GetMin() const { return this->mMin; }
			inline void SetMin(double nX, double nY, double nZ) { this->mMin.sX = nX; this->mMin.sY = nY; this->mMin.sZ = nZ; }
		};
}

#endif // MODRI_LASSIE_LASSIE_H
