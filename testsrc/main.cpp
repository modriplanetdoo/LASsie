#include "../src/LASsie.h"

#include <stdio.h>
#include <string.h>

// main() is at the very bottom of the file


// Macros

#define _stringify(N_ELEMENT) #N_ELEMENT
#define _stringify_indrct(N_ELEMENT) _stringify(N_ELEMENT)
#define _file_line __FILE__ ":" _stringify_indrct(__LINE__)

#define PrintFn() printf(__FUNCTION__ "()\n")
#define Test(N_CND) { if ((N_CND) == false) { printf(_file_line ": no error message\n"); return 1; } }


// Test helper classes

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

	oPdr.SetClassif(0xA5);
	Test(oPdr.GetClassif() == 0xA5);

	oPdr.SetScanAngle(127); // Value checking will happen at generation time
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
	_local_Inout<226> oInoutTooSmall;
	_local_Inout<227> oInout;
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


	// Generating

	oLas.SetInout(&oInoutTooSmall);
	Test(oLas.Generate() == false);

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
	Test(_cmp_LASsieStr("LASsie Library v1.0", oBfrPtr, 32) == 0);
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

	// Offset Point Data; WARNING: These are only testing placeholder values
	Test(oBfrPtr[0] == 0x5A);
	Test(oBfrPtr[1] == 0x5A);
	Test(oBfrPtr[2] == 0xA5);
	Test(oBfrPtr[3] == 0xA5);
	oBfrPtr += 4;

	// Number of Variable Length Records; WARNING: These are only testing placeholder values
	Test(oBfrPtr[0] == 0xF0);
	Test(oBfrPtr[1] == 0x0F);
	Test(oBfrPtr[2] == 0x0F);
	Test(oBfrPtr[3] == 0xF0);
	oBfrPtr += 4;

	// Point Data Format ID
	Test(*oBfrPtr++ == 0x03); // Format 3

	// Point Data Record Length
	Test(oBfrPtr[0] == 0x22); // 0x22 == 34
	Test(oBfrPtr[1] == 0x00);
	oBfrPtr += 2;

	// Number of point records; WARNING: These are only testing placeholder values
	Test(oBfrPtr[0] == 0x67);
	Test(oBfrPtr[1] == 0x45);
	Test(oBfrPtr[2] == 0x23);
	Test(oBfrPtr[3] == 0x01);
	oBfrPtr += 4;

	// Number of points by return; WARNING: These are only testing placeholder values
	Test(oBfrPtr[0] == 0x01);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0xF0);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x02);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0xE0);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x03);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0xD0);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x04);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0xC0);
	oBfrPtr += 4;
	Test(oBfrPtr[0] == 0x05);
	Test(oBfrPtr[1] == 0x00);
	Test(oBfrPtr[2] == 0x00);
	Test(oBfrPtr[3] == 0xB0);
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

	return 0;
}


// main()

int main(int argc, char **argv)
{
	Test(TestLASsieString() == 0);
	Test(TestLASsie() == 0);
	Test(TestLASsieVarLenRec() == 0);
	Test(TestLASsiePointDataRec() == 0);
	Test(TestLASsieGenerate() == 0);
	
	printf("=== ALL TESTS PASSED ===\n");

    return 0;
}
