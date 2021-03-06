#include "../src/LASsie.h"

#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <time.h>

// main() is at the very bottom of the file


// Macros

#define _stringify_indrct(N_ELEMENT) #N_ELEMENT
#define _stringify(N_ELEMENT) _stringify_indrct(N_ELEMENT)
#define _file_line __FILE__ ":" _stringify(__LINE__)

#define PrintFn() printf(__FUNCTION__ "()\n")
#define Test(N_CND) { if ((N_CND) == false) { printf(_file_line ": no error message\n"); return 1; } }


// Test helper classes

class _local_RecProvider : public modri::LASsie::RecProviderIface
{
	public:
		class Gk : public modri::LASsie::GeoKey { };
		typedef std::vector<_local_RecProvider::Gk> GkList;
		
		class Vlr : public modri::LASsie::VarLenRec
		{
			public:
				typedef modri::LASsie::String<32> Str;

			private:
				Vlr::Str mStr;
			
			public:
				const Vlr::Str &Data() const { return this->mStr; }
				Vlr::Str &Data() { return this->mStr; }
		};
		typedef std::vector<_local_RecProvider::Vlr> VlrList;

		class Pdr : public modri::LASsie::PointDataRec { };
		typedef std::vector<_local_RecProvider::Pdr> PdrList;
	
	private:
		_local_RecProvider::GkList mGkList;
		_local_RecProvider::VlrList mVlrList;
		_local_RecProvider::PdrList mPdrList;

	public:
		const _local_RecProvider::GkList &GetGkList() const { return this->mGkList; }
		_local_RecProvider::GkList &GetGkList() { return this->mGkList; }
		const _local_RecProvider::VlrList &GetVlrList() const { return this->mVlrList; }
		_local_RecProvider::VlrList &GetVlrList() { return this->mVlrList; }
		const _local_RecProvider::PdrList &GetPdrList() const { return this->mPdrList; }
		_local_RecProvider::PdrList &GetPdrList() { return this->mPdrList; }
		
		virtual size_t GetGeoKeyCount() const { return this->mGkList.size(); }
		virtual bool FillGeoKey(size_t nIdx, modri::LASsie::GeoKey &nGk) const
		{
			if (nIdx >= this->mGkList.size())
				return false;

			const _local_RecProvider::Gk &oGk = this->mGkList.at(nIdx);
			nGk.SetKeyId(oGk.GetKeyId());
			nGk.SetTagLocat(oGk.GetTagLocat());
			nGk.SetCount(oGk.GetCount());
			nGk.SetValOffset(oGk.GetValOffset());
			return true;
		}

		virtual size_t GetVarLenRecCount() const { return this->mVlrList.size(); }
		virtual size_t GetVarLenRecDataSize(size_t nIdx) const { return ((nIdx < this->mVlrList.size()) ? this->mVlrList.at(nIdx).Data().Len() : 0); }
		virtual const void *GetVarLenRecData(size_t nIdx) const { return ((nIdx < this->mVlrList.size()) ? this->mVlrList.at(nIdx).Data().Get() : NULL); }
		virtual bool FillVarLenRec(size_t nIdx, modri::LASsie::VarLenRec &nVlr) const
		{
			if (nIdx >= this->mVlrList.size())
				return false;

			const _local_RecProvider::Vlr &oVlr = this->mVlrList.at(nIdx);
			nVlr.UserId().Set(oVlr.UserId().Get());
			nVlr.SetRecId(oVlr.GetRecId());
			nVlr.Desc().Set(oVlr.Desc().Get());
			return true;
		}

		virtual size_t GetPointDataRecCount() const { return this->mPdrList.size(); }
		virtual size_t GetPointDataRecCountByRet(modri::uint8 nRetIdx) const { return ((nRetIdx == 0) ? this->mPdrList.size() : 0); }
		virtual bool FillPointDataRec(size_t nIdx, modri::LASsie::PointDataRec &nPdr) const
		{
			if (nIdx >= this->mPdrList.size())
				return false;

			const _local_RecProvider::Pdr &oPdr = this->mPdrList.at(nIdx);
			nPdr.SetCoord(oPdr.GetCoord().sX, oPdr.GetCoord().sY, oPdr.GetCoord().sZ);
			nPdr.SetInten(oPdr.GetInten());
			nPdr.SetRetNum(oPdr.GetRetNum());
			nPdr.SetRetTotal(oPdr.GetRetTotal());
			nPdr.SetScanDirFlag(oPdr.GetScanDirFlag());
			nPdr.SetFlightEdge(oPdr.IsFlightEdge());
			nPdr.SetClassif(oPdr.GetClassif());
			nPdr.SetSynthetic(oPdr.IsSynthetic());
			nPdr.SetKeyPoint(oPdr.IsKeyPoint());
			nPdr.SetWithheld(oPdr.IsWithheld());
			nPdr.SetScanAngle(oPdr.GetScanAngle());
			nPdr.SetUserData(oPdr.GetUserData());
			nPdr.SetPointSrcId(oPdr.GetPointSrcId());
			nPdr.SetGpsTime(oPdr.GetGpsTime());
			nPdr.SetColor(oPdr.GetColor().sR, oPdr.GetColor().sG, oPdr.GetColor().sB);
			return true;
		}
};

class _local_RecProviderRandPdr : public modri::LASsie::RecProviderIface
{
	public:
		

	private:
		modri::uint32 mDimens;
		const size_t mPointNum;
		const modri::uint32 mScaleDiv;
		
		mutable modri::uint32 mCurColor;

	public:
		inline _local_RecProviderRandPdr(modri::uint32 nDimens, modri::uint32 nScaleDiv)
			: mDimens(nDimens), mPointNum(nDimens * nDimens * nDimens), mScaleDiv(nScaleDiv), mCurColor(0) { }

		virtual size_t GetGeoKeyCount() const { return 0; }
		virtual bool FillGeoKey(size_t nIdx, modri::LASsie::GeoKey &nGk) const { return false; }
		virtual size_t GetVarLenRecCount() const { return 0; }
		virtual size_t GetVarLenRecDataSize(size_t nIdx) const { return 0; }
		virtual const void *GetVarLenRecData(size_t nIdx) const { return NULL; }
		virtual bool FillVarLenRec(size_t nIdx, modri::LASsie::VarLenRec &nVlr) const { return false; }

		virtual size_t GetPointDataRecCount() const { return this->mPointNum; }
		virtual size_t GetPointDataRecCountByRet(modri::uint8 nRetIdx) const { return ((nRetIdx == 0) ? this->mPointNum : 0); }
		virtual bool FillPointDataRec(size_t nIdx, modri::LASsie::PointDataRec &nPdr) const
		{
			if (nIdx >= this->mPointNum)
				return false;

			static const modri::uint32 oSubtr = (this->mDimens / 2);
			static const modri::uint32 oDivX = 1;
			static const modri::uint32 oDivY = this->mDimens;
			static const modri::uint32 oDivZ = (this->mDimens * this->mDimens);

			nPdr.SetCoord(
					((((nIdx / oDivX) % this->mDimens) - oSubtr) * this->mScaleDiv),
					((((nIdx / oDivY) % this->mDimens) - oSubtr) * this->mScaleDiv),
					((((nIdx / oDivZ) % this->mDimens) - oSubtr) * this->mScaleDiv)
				);
			nPdr.SetSynthetic(true);
			nPdr.SetColor(
				((this->mCurColor & 0x0000FF) << 8),
				((this->mCurColor & 0x00FF00)),
				((this->mCurColor & 0xFF0000) >> 8));

			this->mCurColor += 16;
			return true;
		}
};

template <size_t tSize>
class _local_Inout : public modri::LASsie::InoutIface
{
	private:
		modri::uint8 mBfr[tSize];
		size_t mBfrOff;

	public:
		inline _local_Inout() { this->Reset(); }
		
		inline void Reset() { this->mBfrOff = 0; }
		inline size_t GetBfrSize() const { return tSize; }
		inline size_t GetBfrLen() const { return this->mBfrOff; }
		inline const modri::uint8 *GetBfr() const { return this->mBfr; }
		
		virtual bool Read(void *nBfr, size_t nBfrSize, size_t &nReadSize)
		{
			return false;
		}

		virtual bool Write(const void *nData, size_t nDataSize)
		{
			if ((tSize - this->mBfrOff) >= nDataSize)
			{
				memcpy((this->mBfr + this->mBfrOff), nData, nDataSize);
				this->mBfrOff += nDataSize;
				return true;
			}
			else
				return false;
		}
};

class _local_InoutFile : public modri::LASsie::InoutIface
{
	private:
		std::ofstream mFs;

	public:
		inline _local_InoutFile() { }
		inline virtual ~_local_InoutFile() { this->Close(); }

		inline bool IsOpen() const { return this->mFs.is_open(); }
		inline bool Open(const char *nFilename)
		{
			this->mFs.open(nFilename, (std::ios_base::out | std::ios_base::trunc | std::ios_base::binary));
			return this->mFs.is_open();
		}
		inline void Close() { this->mFs.close(); }

		virtual bool Read(void *nBfr, size_t nBfrSize, size_t &nReadSize) { return false; }
		virtual bool Write(const void *nData, size_t nDataSize)
		{
			if (this->mFs.is_open() == true)
			{
				this->mFs.write(static_cast<const char *>(nData), nDataSize);
				return true;
			}
			else
				return false;
		}
};


// Test helper functions

int _cmp_LASsieGuid(const modri::LASsie::Guid &nGuid1, const modri::LASsie::Guid &nGuid2)
{
	Test(nGuid1.sD1 == nGuid2.sD1);
	Test(nGuid1.sD2 == nGuid2.sD2);
	Test(nGuid1.sD3 == nGuid2.sD3);
	Test(nGuid1.sD4[0] == nGuid2.sD4[0]);
	Test(nGuid1.sD4[1] == nGuid2.sD4[1]);
	Test(nGuid1.sD4[2] == nGuid2.sD4[2]);
	Test(nGuid1.sD4[3] == nGuid2.sD4[3]);
	Test(nGuid1.sD4[4] == nGuid2.sD4[4]);
	Test(nGuid1.sD4[5] == nGuid2.sD4[5]);
	Test(nGuid1.sD4[6] == nGuid2.sD4[6]);
	Test(nGuid1.sD4[7] == nGuid2.sD4[7]);
	return 0;
}

template <class tType>
int _cmp_LASsieCoord(const modri::LASsie::Coord<tType> &nCoord1, const modri::LASsie::Coord<tType> &nCoord2)
{
	Test(nCoord1.sX == nCoord2.sX);
	Test(nCoord1.sY == nCoord2.sY);
	Test(nCoord1.sZ == nCoord2.sZ);
	return 0;
}

int _cmp_LASsieColor(const modri::LASsie::Color &nColor1, const modri::LASsie::Color &nColor2)
{
	Test(nColor1.sR == nColor2.sR);
	Test(nColor1.sG == nColor2.sG);
	Test(nColor1.sB == nColor2.sB);
	return 0;
}

template <size_t tSize>
int _cmp_LASsieStr(const modri::LASsie::String<tSize> &nLStr, const char *nStr)
{
	for (size_t i = 0; i < (tSize + 1); i++) // +1 because LASsie string has an extra char for NULL terminator
	{
		Test(nLStr.Get()[i] == *nStr);
		if (*nStr != '\0') nStr++;
	}
	return 0;
}

int _cmp_LASsieStr(const char *nStr, const modri::uint8 *nBfrPtr, size_t nBfrSize)
{
	for (size_t i = 0; i < nBfrSize; i++)
	{
		Test(nBfrPtr[i] == *nStr);
		if (*nStr != '\0') nStr++;
	}
	return 0;
}

int _cmp_BfrDoubleAdv(const modri::uint8 *&nBfr, double nVal)
{
	Test(*reinterpret_cast<const double *>(nBfr) == nVal);
	nBfr += sizeof(double);
	return 0;
}


// Test functions

static int TestLASsieString()
{
	PrintFn();

	modri::LASsie::String<1>  oStr1;
	modri::LASsie::String<16> oStr16;
	modri::LASsie::String<32> oStr32;


	Test(oStr1.Size() == 1);
	Test(oStr1.Len() == 0);
	Test(_cmp_LASsieStr(oStr1, "") == 0);

	oStr1.Set("");
	Test(oStr1.Size() == 1);
	Test(oStr1.Len() == 0);
	Test(_cmp_LASsieStr(oStr1, "") == 0);

	oStr1.Set("0");
	Test(oStr1.Size() == 1);
	Test(oStr1.Len() == 1);
	Test(_cmp_LASsieStr(oStr1, "0") == 0);

	oStr1.Set("01");
	Test(oStr1.Size() == 1);
	Test(oStr1.Len() == 1);
	Test(_cmp_LASsieStr(oStr1, "0") == 0);


	Test(oStr16.Size() == 16);
	Test(oStr16.Len() == 0);
	Test(_cmp_LASsieStr(oStr16, "") == 0);

	oStr16.Set("");
	Test(oStr16.Size() == 16);
	Test(oStr16.Len() == 0);
	Test(_cmp_LASsieStr(oStr16, "") == 0);

	oStr16.Set("012345678901234");
	Test(oStr16.Size() == 16);
	Test(oStr16.Len() == 15);
	Test(_cmp_LASsieStr(oStr16, "012345678901234") == 0);

	oStr16.Set("0123456789012345");
	Test(oStr16.Size() == 16);
	Test(oStr16.Len() == 16);
	Test(_cmp_LASsieStr(oStr16, "0123456789012345") == 0);

	oStr16.Set("01234567890123456");
	Test(oStr16.Size() == 16);
	Test(oStr16.Len() == 16);
	Test(_cmp_LASsieStr(oStr16, "0123456789012345") == 0);


	Test(oStr32.Size() == 32);
	Test(oStr32.Len() == 0);
	Test(_cmp_LASsieStr(oStr32, "") == 0);

	oStr32.Set("");
	Test(oStr32.Size() == 32);
	Test(oStr32.Len() == 0);
	Test(_cmp_LASsieStr(oStr32, "") == 0);

	oStr32.Set("0123456789012345678901234567890");
	Test(oStr32.Size() == 32);
	Test(oStr32.Len() == 31);
	Test(_cmp_LASsieStr(oStr32, "0123456789012345678901234567890") == 0);

	oStr32.Set("01234567890123456789012345678901");
	Test(oStr32.Size() == 32);
	Test(oStr32.Len() == 32);
	Test(_cmp_LASsieStr(oStr32, "01234567890123456789012345678901") == 0);

	oStr32.Set("012345678901234567890123456789012");
	Test(oStr32.Size() == 32);
	Test(oStr32.Len() == 32);
	Test(_cmp_LASsieStr(oStr32, "01234567890123456789012345678901") == 0);


	return 0;
}

static int TestLASsie()
{
	PrintFn();

	modri::LASsie oLas;
	modri::LASsie::Guid oGuidVolatile;
	modri::LASsie::Coord<double> oCoordVolatile;


	// Init

	memset(&oGuidVolatile, 0, sizeof(oGuidVolatile));
	memset(&oCoordVolatile, 0, sizeof(oCoordVolatile));

	Test(oLas.GetFileSrcId() == 0);
	Test(oLas.IsGlobalEnc() == false);
	Test(_cmp_LASsieGuid(oLas.GetGuid(), oGuidVolatile) == 0);
	Test(oLas.GenerSw().Size() == 32);
	Test(oLas.GenerSw().Len() == 0);
	Test(_cmp_LASsieStr(oLas.GenerSw(), "") == 0);
	Test(oLas.GetCreatDay() == 0);
	Test(oLas.GetCreatYear() == 0);
	Test(oLas.GetPdrFormat() == modri::LASsie::pdrFormat0);
	Test(_cmp_LASsieCoord(oLas.GetScale(), oCoordVolatile) == 0);
	Test(_cmp_LASsieCoord(oLas.GetOffset(), oCoordVolatile) == 0);
	Test(_cmp_LASsieCoord(oLas.GetMax(), oCoordVolatile) == 0);
	Test(_cmp_LASsieCoord(oLas.GetMin(), oCoordVolatile) == 0);


	// {G,S}etters

	oLas.SetFileSrcId(0x1234);
	Test(oLas.GetFileSrcId() == 0x1234);

	oLas.SetGlobalEnc(true);
	Test(oLas.IsGlobalEnc() == true);

	oGuidVolatile.sD1 = 0xFEDCBA98;
	oGuidVolatile.sD2 = 0x7654;
	oGuidVolatile.sD3 = 0x3210;
	oGuidVolatile.sD4[0] = 0xFE;
	oGuidVolatile.sD4[1] = 0xDC;
	oGuidVolatile.sD4[2] = 0xBA;
	oGuidVolatile.sD4[3] = 0x98;
	oGuidVolatile.sD4[4] = 0x76;
	oGuidVolatile.sD4[5] = 0x54;
	oGuidVolatile.sD4[6] = 0x32;
	oGuidVolatile.sD4[7] = 0x10;
	oLas.SetGuid(oGuidVolatile);
	Test(_cmp_LASsieGuid(oLas.GetGuid(), oGuidVolatile) == 0);

	Test(oLas.GenerSw().Size() == 32);
	Test(oLas.GenerSw().Len() == 0);
	oLas.GenerSw().Set("Some Generating Software");
	Test(oLas.GenerSw().Len() == 24);
	Test(_cmp_LASsieStr(oLas.GenerSw(), "Some Generating Software") == 0);

	oLas.SetCreat(2016, 366);
	Test(oLas.GetCreatDay() == 366);
	Test(oLas.GetCreatYear() == 2016);

	oLas.SetPdrFormat(modri::LASsie::pdrFormat3);
	Test(oLas.GetPdrFormat() == modri::LASsie::pdrFormat3);

	oLas.SetScale(1.1, 1.2, 1.3);
	oCoordVolatile.sX = 1.1;
	oCoordVolatile.sY = 1.2;
	oCoordVolatile.sZ = 1.3;
	Test(_cmp_LASsieCoord(oLas.GetScale(), oCoordVolatile) == 0);

	oLas.SetOffset(2.1, 2.2, 2.3);
	oCoordVolatile.sX = 2.1;
	oCoordVolatile.sY = 2.2;
	oCoordVolatile.sZ = 2.3;
	Test(_cmp_LASsieCoord(oLas.GetOffset(), oCoordVolatile) == 0);

	oLas.SetMax(3.1, 3.2, 3.3);
	oCoordVolatile.sX = 3.1;
	oCoordVolatile.sY = 3.2;
	oCoordVolatile.sZ = 3.3;
	Test(_cmp_LASsieCoord(oLas.GetMax(), oCoordVolatile) == 0);

	oLas.SetMin(4.1, 4.2, 4.3);
	oCoordVolatile.sX = 4.1;
	oCoordVolatile.sY = 4.2;
	oCoordVolatile.sZ = 4.3;
	Test(_cmp_LASsieCoord(oLas.GetMin(), oCoordVolatile) == 0);

	return 0;
}

static int TestLASsieGeoKey()
{
	PrintFn();

	modri::LASsie::GeoKey oGk;


	// Init

	Test(oGk.GetKeyId() == 0);
	Test(oGk.GetTagLocat() == 0);
	Test(oGk.GetCount() == 0);
	Test(oGk.GetValOffset() == 0);


	// {G,S}etters

	oGk.SetKeyId(0xFEDC);
	Test(oGk.GetKeyId() == 0xFEDC);

	oGk.SetTagLocat(0xBA98);
	Test(oGk.GetTagLocat() == 0xBA98);

	oGk.SetCount(0x7654);
	Test(oGk.GetCount() == 0x7654);

	oGk.SetValOffset(0x3210);
	Test(oGk.GetValOffset() == 0x3210);

	return 0;
}

static int TestLASsieVarLenRec()
{
	PrintFn();

	modri::LASsie::VarLenRec oVlr;


	// Init

	Test(_cmp_LASsieStr(oVlr.UserId(), "") == 0);
	Test(oVlr.GetRecId() == 0);
	Test(_cmp_LASsieStr(oVlr.Desc(), "") == 0);


	// {G,S}etters

	Test(oVlr.UserId().Size() == 16);
	Test(oVlr.UserId().Len() == 0);
	oVlr.UserId().Set("Some User ID");
	Test(oVlr.UserId().Len() == 12);
	Test(_cmp_LASsieStr(oVlr.UserId(), "Some User ID") == 0);

	oVlr.SetRecId(0x5678);
	Test(oVlr.GetRecId() == 0x5678);

	Test(oVlr.Desc().Size() == 32);
	Test(oVlr.Desc().Len() == 0);
	oVlr.Desc().Set("Some User ID");
	Test(oVlr.Desc().Len() == 12);
	Test(_cmp_LASsieStr(oVlr.Desc(), "Some User ID") == 0);

	return 0;
}

static int TestLASsiePointDataRec()
{
	PrintFn();

	modri::LASsie::PointDataRec oPdr;
	modri::LASsie::PointDataRec::CoordType oCoordVolatile;
	modri::LASsie::Color oColorVolatile;


	// Init

	memset(&oCoordVolatile, 0, sizeof(oCoordVolatile));
	memset(&oColorVolatile, 0, sizeof(oColorVolatile));

	Test(_cmp_LASsieCoord(oPdr.GetCoord(), oCoordVolatile) == 0);
	Test(oPdr.GetInten() == 0);
	Test(oPdr.GetRetNum() == 0);
	Test(oPdr.GetRetTotal() == 0);
	Test(oPdr.GetScanDirFlag() == false);
	Test(oPdr.IsFlightEdge() == false);
	Test(oPdr.GetClassif() == 0);
	Test(oPdr.IsSynthetic() == false);
	Test(oPdr.IsKeyPoint() == false);
	Test(oPdr.IsWithheld() == false);
	Test(oPdr.GetScanAngle() == 0);
	Test(oPdr.GetUserData() == 0);
	Test(oPdr.GetPointSrcId() == 0);
	Test(oPdr.GetGpsTime() == 0.0);
	Test(_cmp_LASsieColor(oPdr.GetColor(), oColorVolatile) == 0);


	// {G,S}etters

	oPdr.SetCoord(0xFEDC, 0xBA98, 0x7654);
	oCoordVolatile.sX = 0xFEDC;
	oCoordVolatile.sY = 0xBA98;
	oCoordVolatile.sZ = 0x7654;
	Test(_cmp_LASsieCoord(oPdr.GetCoord(), oCoordVolatile) == 0);

	oPdr.SetInten(0xF0A5);
	Test(oPdr.GetInten() == 0xF0A5);

	oPdr.SetRetNum(255); // Value checking will happen at generation time
	Test(oPdr.GetRetNum() == 255);

	oPdr.SetRetTotal(255); // Value checking will happen at generation time
	Test(oPdr.GetRetTotal() == 255);

	oPdr.SetScanDirFlag(true);
	Test(oPdr.GetScanDirFlag() == true);
	oPdr.SetScanDirFlag(false);
	Test(oPdr.GetScanDirFlag() == false);

	oPdr.SetFlightEdge(true);
	Test(oPdr.IsFlightEdge() == true);
	oPdr.SetFlightEdge(false);
	Test(oPdr.IsFlightEdge() == false);

	oPdr.SetClassif(0xA5); // This value is too big, but setters don't complain
	Test(oPdr.GetClassif() == 0xA5);

	oPdr.SetSynthetic(true);
	Test(oPdr.IsSynthetic() == true);

	oPdr.SetKeyPoint(true);
	Test(oPdr.IsKeyPoint() == true);

	oPdr.SetWithheld(true);
	Test(oPdr.IsWithheld() == true);

	oPdr.SetScanAngle(127);
	Test(oPdr.GetScanAngle() == 127);
	oPdr.SetScanAngle(-128);
	Test(oPdr.GetScanAngle() == -128);

	oPdr.SetUserData(0xBB);
	Test(oPdr.GetUserData() == 0xBB);

	oPdr.SetPointSrcId(0xA55A);
	Test(oPdr.GetPointSrcId() == 0xA55A);

	oPdr.SetGpsTime(12345.6789);
	Test(oPdr.GetGpsTime() == 12345.6789);

	oPdr.SetColor(0xF1, 0xE2, 0xD3);
	oColorVolatile.sR = 0xF1;
	oColorVolatile.sG = 0xE2;
	oColorVolatile.sB = 0xD3;
	Test(_cmp_LASsieColor(oPdr.GetColor(), oColorVolatile) == 0);

	return 0;
}

static int TestLASsieGenerate()
{
	PrintFn();

	modri::LASsie oLas;
	modri::LASsie::Guid oGuid;
	_local_RecProvider::Gk oGeoKey;
	_local_RecProvider::Vlr oVlr;
	_local_RecProvider::Pdr oPdr;
	_local_RecProvider oRecProv;
	_local_Inout<615> oInoutTooSmall;
	_local_Inout<616> oInout;
	const modri::uint8 *oBfrPtr;


	// Setting

	oGuid.sD1 = 0xFEDCBA98;
	oGuid.sD2 = 0x7654;
	oGuid.sD3 = 0x3210;
	oGuid.sD4[0] = 0xFE;
	oGuid.sD4[1] = 0xDC;
	oGuid.sD4[2] = 0xBA;
	oGuid.sD4[3] = 0x98;
	oGuid.sD4[4] = 0x76;
	oGuid.sD4[5] = 0x54;
	oGuid.sD4[6] = 0x32;
	oGuid.sD4[7] = 0x10;

	oLas.SetFileSrcId(0x1234);
	oLas.SetGlobalEnc(true);
	oLas.SetGuid(oGuid);
	oLas.GenerSw().Set("Some Generating Software");
	oLas.SetCreat(2016, 366);
	oLas.SetPdrFormat(modri::LASsie::pdrFormat3);
	oLas.SetScale(1.1, 1.2, 1.3);
	oLas.SetOffset(2.1, 2.2, 2.3);
	oLas.SetMax(3.1, 3.2, 3.3);
	oLas.SetMin(4.1, 4.2, 4.3);

	oGeoKey.SetKeyId(0x1001);
	oGeoKey.SetTagLocat(0x2002);
	oGeoKey.SetCount(0x3003);
	oGeoKey.SetValOffset(0x4004);
	oRecProv.GetGkList().push_back(oGeoKey);

	oGeoKey.SetKeyId(0x5005);
	oGeoKey.SetTagLocat(0x6006);
	oGeoKey.SetCount(0x7007);
	oGeoKey.SetValOffset(0x8008);
	oRecProv.GetGkList().push_back(oGeoKey);

	oVlr.UserId().Set("User_VLR_1");
	oVlr.SetRecId(0x3210);
	oVlr.Desc().Set("User Variable Length Record 1");
	oVlr.Data().Set("This is VLR string data"); // 23 bytes
	oRecProv.GetVlrList().push_back(oVlr);

	oVlr.UserId().Set("User_VLR_2");
	oVlr.SetRecId(0x7654);
	oVlr.Desc().Set("User Variable Length Record 2");
	oVlr.Data().Set("This is another VLR string data"); // 31 bytes
	oRecProv.GetVlrList().push_back(oVlr);

	oVlr.UserId().Set("User_VLR_3");
	oVlr.SetRecId(0xBA98);
	oVlr.Desc().Set("User Variable Length Record 3");
	oVlr.Data().Set("And another VLR string data"); // 27 bytes
	oRecProv.GetVlrList().push_back(oVlr);

	oPdr.SetCoord(0xF0E1D2B3, 0xA4958677, 0x68594A3B);
	oPdr.SetInten(0x2C1D);
	oPdr.SetRetNum(6);
	oPdr.SetRetTotal(6);
	oPdr.SetScanDirFlag(true);
	oPdr.SetFlightEdge(false);
	oPdr.SetClassif(0xE1); // will be masked to 1
	oPdr.SetSynthetic(false);
	oPdr.SetKeyPoint(true);
	oPdr.SetWithheld(false);
	oPdr.SetScanAngle(-120);
	oPdr.SetUserData(0x5A);
	oPdr.SetPointSrcId(0xFA50);
	oPdr.SetGpsTime(12345.6789);
	oPdr.SetColor(0xDC29, 0x63BD, 0x347F);
	oRecProv.GetPdrList().push_back(oPdr);

	oPdr.SetCoord(0x0F1E2D3B, 0x4A596877, 0x8695A4B3);
	oPdr.SetInten(0xC2D1);
	oPdr.SetRetNum(1);
	oPdr.SetRetTotal(2);
	oPdr.SetScanDirFlag(false);
	oPdr.SetFlightEdge(true);
	oPdr.SetClassif(0x1F); // max value
	oPdr.SetSynthetic(true);
	oPdr.SetKeyPoint(false);
	oPdr.SetWithheld(true);
	oPdr.SetScanAngle(-15);
	oPdr.SetUserData(0xA5);
	oPdr.SetPointSrcId(0x05AF);
	oPdr.SetGpsTime(98765.4321);
	oPdr.SetColor(0x8CAA, 0x3381, 0x2B03);
	oRecProv.GetPdrList().push_back(oPdr);


	// Generating
	
	oLas.SetRecProvider(&oRecProv);

	oLas.SetInout(&oInoutTooSmall);
	Test(oLas.Generate() == false);
	Test(oLas.GetLastError() == modri::LASsie::leWriteFail);

	oLas.SetInout(&oInout);
	Test(oLas.Generate() == true);
	Test(oInout.GetBfrLen() == oInout.GetBfrSize());
	oBfrPtr = oInout.GetBfr();


	// Verifying

	// File signature
	Test(oBfrPtr[0] == 'L');
	Test(oBfrPtr[1] == 'A');
	Test(oBfrPtr[2] == 'S');
	Test(oBfrPtr[3] == 'F');
	oBfrPtr += 4;

	// File Source ID
	Test(oBfrPtr[0] == 0x34);
	Test(oBfrPtr[1] == 0x12);
	oBfrPtr += 2;

	// Global Encoding
	Test(oBfrPtr[0] == 0x01);
	Test(oBfrPtr[1] == 0x00);
	oBfrPtr += 2;

	// GUID data 1
	Test(oBfrPtr[0] == 0x98);
	Test(oBfrPtr[1] == 0xBA);
	Test(oBfrPtr[2] == 0xDC);
	Test(oBfrPtr[3] == 0xFE);
	oBfrPtr += 4;

	// GUID data 2
	Test(oBfrPtr[0] == 0x54);
	Test(oBfrPtr[1] == 0x76);
	oBfrPtr += 2;

	// GUID data 3
	Test(oBfrPtr[0] == 0x10);
	Test(oBfrPtr[1] == 0x32);
	oBfrPtr += 2;

	// GUID data 4
	Test(oBfrPtr[0] == 0xFE);
	Test(oBfrPtr[1] == 0xDC);
	Test(oBfrPtr[2] == 0xBA);
	Test(oBfrPtr[3] == 0x98);
	Test(oBfrPtr[4] == 0x76);
	Test(oBfrPtr[5] == 0x54);
	Test(oBfrPtr[6] == 0x32);
	Test(oBfrPtr[7] == 0x10);
	oBfrPtr += 8;

	// Version {Major,Minor}
	Test(*oBfrPtr++ == 1);
	Test(*oBfrPtr++ == 2);

	// System Identifier
	Test(_cmp_LASsieStr("LASsie Library v"
		_stringify(LASSIE_VERSION_MAJOR) "."
		_stringify(LASSIE_VERSION_MINOR) "."
		_stringify(LASSIE_VERSION_PATCH), oBfrPtr, 32) == 0);
	oBfrPtr += 32;

	// Generating Software
	Test(_cmp_LASsieStr("Some Generating Software", oBfrPtr, 32) == 0);
	oBfrPtr += 32;

	// Creation {Day,Year}
	Test(oBfrPtr[0] == 0x6E); // 0x016E == 366
	Test(oBfrPtr[1] == 0x01);
	Test(oBfrPtr[2] == 0xE0); // 0x07E0 == 2016
	Test(oBfrPtr[3] == 0x07);
	oBfrPtr += 4;

	// Header Size
	Test(oBfrPtr[0] == 0xE3); // 0xE3 == 227
	Test(oBfrPtr[1] == 0x00);
	oBfrPtr += 2;

	// Offset Point Data
	Test(oBfrPtr[0] == 0x24);
	Test(oBfrPtr[1] == 0x02);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0x00);
	size_t oPdOffsetStat =  // Stated Point Data Offset (used later to verify it matches the actual PD offset in data)
		(static_cast<size_t>(oBfrPtr[3]) << 24) |
		(static_cast<size_t>(oBfrPtr[2]) << 16) |
		(static_cast<size_t>(oBfrPtr[1]) <<  8) |
		(static_cast<size_t>(oBfrPtr[0]));
	oBfrPtr += 4;

	// Number of Variable Length Records
	Test(oBfrPtr[0] == 0x04);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;

	// Point Data Format ID
	Test(*oBfrPtr++ == 0x03); // Format 3

	// Point Data Record Length
	Test(oBfrPtr[0] == 0x22); // 0x22 == 34
	Test(oBfrPtr[1] == 0x00);
	oBfrPtr += 2;

	// Number of point records; WARNING: These are only testing placeholder values
	Test(oBfrPtr[0] == 0x02);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;

	// Number of points by return; WARNING: These are only testing placeholder values
	Test(oBfrPtr[0] == 0x02);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x00);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x00);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x00);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x00);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;

	// {X,Y,Z} scale factor
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 1.1) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 1.2) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 1.3) == 0);

	// {X,Y,Z} offset
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 2.1) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 2.2) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 2.3) == 0);

	// {X,Y,Z} {Max,Min}
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 3.1) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 4.1) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 3.2) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 4.2) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 3.3) == 0);
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 4.3) == 0);


	// VLR header for GeoKeyDirectoryTag
	Test(oBfrPtr[0] == 0x00);
	Test(oBfrPtr[1] == 0x00);
	oBfrPtr += 2;
	Test(_cmp_LASsieStr("LASF_Projection", oBfrPtr, 16) == 0);
	oBfrPtr += 16;
	Test(oBfrPtr[0] == 0xAF);
	Test(oBfrPtr[1] == 0x87);
	Test(oBfrPtr[2] == 0x18);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;
	Test(_cmp_LASsieStr("", oBfrPtr, 32) == 0);
	oBfrPtr += 32;
	
	// GeoKeyDirectoryTag header
	Test(oBfrPtr[0] == 0x01);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x01);
	Test(oBfrPtr[3] == 0x00);
	Test(oBfrPtr[4] == 0x00);
	Test(oBfrPtr[5] == 0x00);
	Test(oBfrPtr[6] == 0x02);
	Test(oBfrPtr[7] == 0x00);
	oBfrPtr += 8;

	// GeoKeyDirectoryTag KeyEntry[0]
	Test(oBfrPtr[0] == 0x01);
	Test(oBfrPtr[1] == 0x10);
	Test(oBfrPtr[2] == 0x02);
	Test(oBfrPtr[3] == 0x20);
	Test(oBfrPtr[4] == 0x03);
	Test(oBfrPtr[5] == 0x30);
	Test(oBfrPtr[6] == 0x04);
	Test(oBfrPtr[7] == 0x40);
	oBfrPtr += 8;

	// GeoKeyDirectoryTag KeyEntry[1]
	Test(oBfrPtr[0] == 0x05);
	Test(oBfrPtr[1] == 0x50);
	Test(oBfrPtr[2] == 0x06);
	Test(oBfrPtr[3] == 0x60);
	Test(oBfrPtr[4] == 0x07);
	Test(oBfrPtr[5] == 0x70);
	Test(oBfrPtr[6] == 0x08);
	Test(oBfrPtr[7] == 0x80);
	oBfrPtr += 8;


	// VLR[0]
	Test(oBfrPtr[0] == 0x00);
	Test(oBfrPtr[1] == 0x00);
	oBfrPtr += 2;
	Test(_cmp_LASsieStr("User_VLR_1", oBfrPtr, 16) == 0);
	oBfrPtr += 16;
	Test(oBfrPtr[0] == 0x10);
	Test(oBfrPtr[1] == 0x32);
	Test(oBfrPtr[2] == 0x17);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;
	Test(_cmp_LASsieStr("User Variable Length Record 1", oBfrPtr, 32) == 0);
	oBfrPtr += 32;
	Test(memcmp(oBfrPtr, "This is VLR string data", 23) == 0);
	oBfrPtr += 23;

	// VLR[1]
	Test(oBfrPtr[0] == 0x00);
	Test(oBfrPtr[1] == 0x00);
	oBfrPtr += 2;
	Test(_cmp_LASsieStr("User_VLR_2", oBfrPtr, 16) == 0);
	oBfrPtr += 16;
	Test(oBfrPtr[0] == 0x54);
	Test(oBfrPtr[1] == 0x76);
	Test(oBfrPtr[2] == 0x1F);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;
	Test(_cmp_LASsieStr("User Variable Length Record 2", oBfrPtr, 32) == 0);
	oBfrPtr += 32;
	Test(memcmp(oBfrPtr, "This is another VLR string data", 31) == 0);
	oBfrPtr += 31;

	// VLR[2]
	Test(oBfrPtr[0] == 0x00);
	Test(oBfrPtr[1] == 0x00);
	oBfrPtr += 2;
	Test(_cmp_LASsieStr("User_VLR_3", oBfrPtr, 16) == 0);
	oBfrPtr += 16;
	Test(oBfrPtr[0] == 0x98);
	Test(oBfrPtr[1] == 0xBA);
	Test(oBfrPtr[2] == 0x1B);
	Test(oBfrPtr[3] == 0x00);
	oBfrPtr += 4;
	Test(_cmp_LASsieStr("User Variable Length Record 3", oBfrPtr, 32) == 0);
	oBfrPtr += 32;
	Test(memcmp(oBfrPtr, "And another VLR string data", 27) == 0);
	oBfrPtr += 27;


	// Check stated Point Data Offset with actual offset in data
	size_t oPdOffsetCalc = (oBfrPtr - oInout.GetBfr());
	Test(oPdOffsetStat == oPdOffsetCalc);


	// PDR[0]
	Test(oBfrPtr[0] == 0xB3);
	Test(oBfrPtr[1] == 0xD2);
	Test(oBfrPtr[2] == 0xE1);
	Test(oBfrPtr[3] == 0xF0);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x77);
	Test(oBfrPtr[1] == 0x86);
	Test(oBfrPtr[2] == 0x95);
	Test(oBfrPtr[3] == 0xA4);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x3B);
	Test(oBfrPtr[1] == 0x4A);
	Test(oBfrPtr[2] == 0x59);
	Test(oBfrPtr[3] == 0x68);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x1D);
	Test(oBfrPtr[1] == 0x2C);
	oBfrPtr += 2;
	Test(*oBfrPtr++ == (
		((6 & 0x07)) |
		((6 & 0x07) << 3) |
		0x40 |
		0x00));
	Test(*oBfrPtr++ == 0x41);
	Test(*oBfrPtr++ == static_cast<modri::uint8>(-120));
	Test(*oBfrPtr++ == 0x5A);
	Test(oBfrPtr[0] == 0x50);
	Test(oBfrPtr[1] == 0xFA);
	oBfrPtr += 2;
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 12345.6789) == 0);
	Test(oBfrPtr[0] == 0x29);
	Test(oBfrPtr[1] == 0xDC);
	Test(oBfrPtr[2] == 0xBD);
	Test(oBfrPtr[3] == 0x63);
	Test(oBfrPtr[4] == 0x7F);
	Test(oBfrPtr[5] == 0x34);
	oBfrPtr += 6;


	// PDR[1]
	Test(oBfrPtr[0] == 0x3B);
	Test(oBfrPtr[1] == 0x2D);
	Test(oBfrPtr[2] == 0x1E);
	Test(oBfrPtr[3] == 0x0F);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x77);
	Test(oBfrPtr[1] == 0x68);
	Test(oBfrPtr[2] == 0x59);
	Test(oBfrPtr[3] == 0x4A);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0xB3);
	Test(oBfrPtr[1] == 0xA4);
	Test(oBfrPtr[2] == 0x95);
	Test(oBfrPtr[3] == 0x86);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0xD1);
	Test(oBfrPtr[1] == 0xC2);
	oBfrPtr += 2;
	Test(*oBfrPtr++ == (
		((1 & 0x07)) |
		((2 & 0x07) << 3) |
		0x00 |
		0x80));
	Test(*oBfrPtr++ == 0xBF);
	Test(*oBfrPtr++ == static_cast<modri::uint8>(-15));
	Test(*oBfrPtr++ == 0xA5);
	Test(oBfrPtr[0] == 0xAF);
	Test(oBfrPtr[1] == 0x05);
	oBfrPtr += 2;
	Test(_cmp_BfrDoubleAdv(oBfrPtr, 98765.4321) == 0);
	Test(oBfrPtr[0] == 0xAA);
	Test(oBfrPtr[1] == 0x8C);
	Test(oBfrPtr[2] == 0x81);
	Test(oBfrPtr[3] == 0x33);
	Test(oBfrPtr[4] == 0x03);
	Test(oBfrPtr[5] == 0x2B);
	oBfrPtr += 6;


	return 0;
}


int TestLASsieToFile(modri::uint32 nDimens, const char *nFilename)
{
	static const modri::uint32 oScaleDiv = 1000;

	time_t oCreatTimeT = time(NULL);
	struct tm oCreatTm;
	localtime_s(&oCreatTm, &oCreatTimeT);

	modri::LASsie oLas;
	_local_RecProviderRandPdr oRecProv(nDimens, oScaleDiv);
	_local_InoutFile oFile;
	Test(oFile.Open(nFilename) == true);

	oLas.SetFileSrcId(12345);
	oLas.SetGlobalEnc(false);
	oLas.GenerSw().Set("LASsie Library v1.0 Test Suite");
	oLas.SetCreat((oCreatTm.tm_year + 1900), oCreatTm.tm_yday);
	oLas.SetPdrFormat(modri::LASsie::pdrFormat2); // We don't need GPS time
	oLas.SetScale(
		(1.0 / static_cast<double>(oScaleDiv)),
		(1.0 / static_cast<double>(oScaleDiv)),
		(1.0 / static_cast<double>(oScaleDiv)));
	oLas.SetMax(
		(static_cast<double>(nDimens)),
		(static_cast<double>(nDimens)),
		(static_cast<double>(nDimens)));
	oLas.SetMin(
		(static_cast<double>(nDimens) * -1),
		(static_cast<double>(nDimens) * -1),
		(static_cast<double>(nDimens) * -1));

	oLas.SetRecProvider(&oRecProv);
	oLas.SetInout(&oFile);
	Test(oLas.Generate() == true);

	oFile.Close();
	return 0;
}


// main()

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		Test(TestLASsieString() == 0);
		Test(TestLASsie() == 0);
		Test(TestLASsieGeoKey() == 0);
		Test(TestLASsieVarLenRec() == 0);
		Test(TestLASsiePointDataRec() == 0);
		Test(TestLASsieGenerate() == 0);
		printf("=== ALL TESTS PASSED ===\n");
	}
	else
	if (argc == 3)
	{
		modri::uint32 oDimens = strtoul(argv[1], NULL, 10);
		if (oDimens > 0)
			Test(TestLASsieToFile(oDimens, argv[2]) == 0)
		else
		{
			printf("\nPoint count value is not valid or is 0.\nRun program like:\n    testLASsie CubeDimensions FilePath\n");
			return 1;
		}
	}

    return 0;
}
