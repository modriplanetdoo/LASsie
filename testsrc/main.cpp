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
	Test(oLas.GetPdrf() == modri::LASsie::Pdrf0);
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

	oLas.SetPdrf(modri::LASsie::Pdrf3);
	Test(oLas.GetPdrf() == modri::LASsie::Pdrf3);

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

	oPdr.SetRetNum(5);
	Test(oPdr.GetRetNum() == 5);
	oPdr.SetRetNum(6); // more than the value 5 not allowed
	Test(oPdr.GetRetNum() == 0);

	oPdr.SetRetTotal(5);
	Test(oPdr.GetRetTotal() == 5);
	oPdr.SetRetTotal(6); // more than the value 5 not allowed
	Test(oPdr.GetRetTotal() == 0);

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

	oPdr.SetScanAngle(90);
	Test(oPdr.GetScanAngle() == 90);
	oPdr.SetScanAngle(91); // value over 90 (degrees) not allowed
	Test(oPdr.GetScanAngle() == 0);
	oPdr.SetScanAngle(-90);
	Test(oPdr.GetScanAngle() == -90);
	oPdr.SetScanAngle(-91); // value under -90 (degrees) not allowed
	Test(oPdr.GetScanAngle() == 0);

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


// main()

int main(int argc, char **argv)
{
	modri::LASsie::String<32> oStr;

	Test(TestLASsieString() == 0);
	Test(TestLASsie() == 0);
	Test(TestLASsieVarLenRec() == 0);
	Test(TestLASsiePointDataRec() == 0);
	
	printf("=== ALL TESTS PASSED ===\n");

    return 0;
}
