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

			enum PdrFormat
			{
				pdrFormat0 = 0,
				pdrFormat1 = 1,
				pdrFormat2 = 2,
				pdrFormat3 = 3
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

			class GeoKey
			{
				private:
					modri::uint16 mKeyId;
					modri::uint16 mTagLocat;
					modri::uint16 mCount;
					modri::uint16 mValOffset;

				public:
					inline GeoKey() { this->Reset(); }
					inline virtual ~GeoKey() { }

					void Reset();

					inline modri::uint16 GetKeyId() const { return this->mKeyId; }
					inline void SetKeyId(modri::uint16 nKeyId) { this->mKeyId = nKeyId; }
					inline modri::uint16 GetTagLocat() const { return this->mTagLocat; }
					inline void SetTagLocat(modri::uint16 nTagLocat) { this->mTagLocat = nTagLocat; }
					inline modri::uint16 GetCount() const { return this->mCount; }
					inline void SetCount(modri::uint16 nCount) { this->mCount = nCount; }
					inline modri::uint16 GetValOffset() const { return this->mValOffset; }
					inline void SetValOffset(modri::uint16 nValOffset) { this->mValOffset = nValOffset; }
			};

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
				public:
					typedef LASsie::Coord<modri::sint32> CoordType;

				private:
					PointDataRec::CoordType mCoord;
					modri::uint16 mInten;
					modri::uint8 mRetNum;
					modri::uint8 mRetTotal;
					bool mScanDirFlag;
					bool mFlightEdge;
					modri::uint8 mClassif;
					bool mSynthetic;
					bool mKeyPoint;
					bool mWithheld;
					modri::sint8 mScanAngle;
					modri::uint8 mUserData;
					modri::uint16 mPointSrcId;
					double mGpsTime;
					LASsie::Color mColor;

				public:
					inline PointDataRec() { this->Reset(); }
					inline virtual ~PointDataRec() { }

					void Reset();

					inline const PointDataRec::CoordType &GetCoord() const { return this->mCoord; }
					inline PointDataRec::CoordType GetCoord() { return this->mCoord; }
					inline void SetCoord(modri::sint32 nX, modri::sint32 nY, modri::sint32 nZ) { this->mCoord.sX = nX; this->mCoord.sY = nY; this->mCoord.sZ = nZ; }
					inline modri::uint16 GetInten() const { return this->mInten; }
					inline void SetInten(modri::uint16 nInten) { this->mInten = nInten; }
					inline modri::uint8 GetRetNum() const { return this->mRetNum; }
					inline void SetRetNum(modri::uint8 nRetNum) { this->mRetNum = nRetNum; }
					inline modri::uint8 GetRetTotal() const { return this->mRetTotal; }
					inline void SetRetTotal(modri::uint8 nRetTotal) { this->mRetTotal = nRetTotal; }
					inline bool GetScanDirFlag() const { return this->mScanDirFlag; }
					inline void SetScanDirFlag(bool nScanDirFlag) { this->mScanDirFlag = nScanDirFlag; }
					inline bool IsFlightEdge() const { return this->mFlightEdge; }
					inline void SetFlightEdge(bool nFlightEdge) { this->mFlightEdge = nFlightEdge; }
					inline modri::uint8 GetClassif() const { return this->mClassif; }
					inline void SetClassif(modri::uint8 nClassif) { this->mClassif = nClassif; }
					inline bool IsSynthetic() const { return this->mSynthetic; }
					inline void SetSynthetic(bool nSynthetic) { this->mSynthetic = nSynthetic; }
					inline bool IsKeyPoint() const { return this->mKeyPoint; }
					inline void SetKeyPoint(bool nKeyPoint) { this->mKeyPoint = nKeyPoint; }
					inline bool IsWithheld() const { return this->mWithheld; }
					inline void SetWithheld(bool nWithheld) { this->mWithheld = nWithheld; }
					inline modri::sint8 GetScanAngle() const { return this->mScanAngle; }
					inline void SetScanAngle(modri::sint8 nScanAngle) { this->mScanAngle = nScanAngle; }
					inline modri::uint8 GetUserData() const { return this->mUserData; }
					inline void SetUserData(modri::uint8 nUserData) { this->mUserData = nUserData; }
					inline modri::uint16 GetPointSrcId() const { return this->mPointSrcId; }
					inline void SetPointSrcId(modri::uint16 nPointSrcId) { this->mPointSrcId = nPointSrcId; }
					inline double GetGpsTime() const { return this->mGpsTime; }
					inline void SetGpsTime(double nGpsTime) { this->mGpsTime = nGpsTime; }
					inline const LASsie::Color &GetColor() const { return this->mColor; }
					inline LASsie::Color &GetColor() { return this->mColor; }
					inline void SetColor(modri::uint16 nR, modri::uint16 nG, modri::uint16 nB) { this->mColor.sR = nR; this->mColor.sG = nG; this->mColor.sB = nB; }
			};

			class RecProviderIface
			{
				public:
					virtual size_t GetGeoKeyCount() const = 0;
					virtual bool FillGeoKey(size_t nIdx, LASsie::GeoKey &nGk) const = 0;
					virtual size_t GetVarLenRecCount() const = 0;
					virtual size_t GetVarLenRecDataSize(size_t nIdx) const = 0;
					virtual const void *GetVarLenRecData(size_t nIdx) const = 0;
					virtual bool FillVarLenRec(size_t nIdx, LASsie::VarLenRec &nVlr) const = 0;
					virtual size_t GetPointDataRecCount() const = 0;
					virtual size_t GetPointDataRecCountByRet(modri::uint8 nRet) const = 0;
					virtual bool FillPointDataRec(size_t nIdx, LASsie::PointDataRec &nPdr) const = 0;
			};

			// class RecProcessorIface to be added for processing parsed records

			class InoutIface
			{
				public:
					virtual bool Read(void *nBfr, size_t nBfrSize, size_t &nReadSize) = 0;
					virtual bool Write(const void *nData, size_t nDataSize) = 0;
			};

			enum LastError
			{
				leNone,
				leSysError,
				leNotImplemeted,
				leNoInoutIface,
				leWriteFail,
				leReadFail,
				leGeoKeysSizeTooBig,
				lePdrCountTooBig,
				leGeoKeyFillFail,
				leVlrFillFail,
				lePdrFillFail
			};

		private:
			modri::uint16 mFileSrcId;
			bool mGlobalEnc;
			LASsie::Guid mGuid;
			LASsie::GenerSwType mGenerSw;
			modri::uint16 mCreatDay;
			modri::uint16 mCreatYear;
			LASsie::PdrFormat mPdrFormat;
			LASsie::ScaleType mScale;
			LASsie::OffsetType mOffset;
			LASsie::CoordEdgeType mMax;
			LASsie::CoordEdgeType mMin;

			const LASsie::RecProviderIface *mRecProvider;
			LASsie::InoutIface *mInout;

			mutable LASsie::LastError mLastError;

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
			inline LASsie::PdrFormat GetPdrFormat() const { return this->mPdrFormat; }
			inline void SetPdrFormat(LASsie::PdrFormat nPdrFormat) { this->mPdrFormat = nPdrFormat; }
			inline const LASsie::ScaleType &GetScale() const { return this->mScale; }
			inline void SetScale(double nX, double nY, double nZ) { this->mScale.sX = nX; this->mScale.sY = nY; this->mScale.sZ = nZ; }
			inline const LASsie::OffsetType &GetOffset() const { return this->mOffset; }
			inline void SetOffset(double nX, double nY, double nZ) { this->mOffset.sX = nX; this->mOffset.sY = nY; this->mOffset.sZ = nZ; }
			inline const LASsie::CoordEdgeType &GetMax() const { return this->mMax; }
			inline void SetMax(double nX, double nY, double nZ) { this->mMax.sX = nX; this->mMax.sY = nY; this->mMax.sZ = nZ; }
			inline const LASsie::CoordEdgeType &GetMin() const { return this->mMin; }
			inline void SetMin(double nX, double nY, double nZ) { this->mMin.sX = nX; this->mMin.sY = nY; this->mMin.sZ = nZ; }

			inline void SetRecProvider(const LASsie::RecProviderIface *nRecProvider) { this->mRecProvider = nRecProvider; }
			inline void SetInout(LASsie::InoutIface *nInout) { this->mInout = nInout; }

			inline LASsie::LastError GetLastError() const { return this->mLastError; }

			// bool Parse() will come later
			bool Generate();
		};
}

#endif // MODRI_LASSIE_LASSIE_H
