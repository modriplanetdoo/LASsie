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
	Test(nGuid1.sData1 == nGuid2.sData1);
	Test(nGuid1.sData2 == nGuid2.sData2);
	Test(nGuid1.sData3 == nGuid2.sData3);
	Test(nGuid1.sData4[0] == nGuid2.sData4[0]);
	Test(nGuid1.sData4[1] == nGuid2.sData4[1]);
	Test(nGuid1.sData4[2] == nGuid2.sData4[2]);
	Test(nGuid1.sData4[3] == nGuid2.sData4[3]);
	Test(nGuid1.sData4[4] == nGuid2.sData4[4]);
	Test(nGuid1.sData4[5] == nGuid2.sData4[5]);
	Test(nGuid1.sData4[6] == nGuid2.sData4[6]);
	Test(nGuid1.sData4[7] == nGuid2.sData4[7]);
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

int _cmp_LASsieGenSw(const char *nGenSw, const char *nStr)
{
	// nGenSw is a type of char[33] in LASsie class, so we need to actually check all 33 chars even if nStr ends before that
	for (size_t i = 0; i < 33; i++)
	{
		Test(nGenSw[i] == *nStr);
		if (*nStr != '\0')
			nStr++;
	}
	return 0;
}


// Test functions

static int TestLASsieInit()
{
	PrintFn();

	modri::LASsie oLas;
	modri::LASsie::Guid oGuidZero;
	modri::LASsie::Coord<double> oCoordZero;

	memset(&oGuidZero, 0, sizeof(oGuidZero));
	memset(&oCoordZero, 0, sizeof(oCoordZero));

	Test(oLas.GetFileSourceId() == 0);
	Test(oLas.IsGlobalEncoding() == false);
	Test(_cmp_LASsieGuid(oLas.GetGuid(), oGuidZero) == 0);
	Test(_cmp_LASsieGenSw(oLas.GetGeneratingSoftware(), "") == 0);
	Test(oLas.GetCreationDay() == 0);
	Test(oLas.GetCreationYear() == 0);
	Test(oLas.GetPointDataFormat() == modri::LASsie::PointDataFormat0);
	Test(_cmp_LASsieCoord(oLas.GetScaleFactor(), oCoordZero) == 0);
	Test(_cmp_LASsieCoord(oLas.GetOffset(), oCoordZero) == 0);
	Test(_cmp_LASsieCoord(oLas.GetMax(), oCoordZero) == 0);
	Test(_cmp_LASsieCoord(oLas.GetMin(), oCoordZero) == 0);

	return 0;
}

static int TestLASsieSetters()
{
	PrintFn();

	modri::LASsie oLas;
	modri::LASsie::Guid oGuidVolatile;
	modri::LASsie::Coord<double> oCoordVolatile;

	oLas.SetFileSourceId(0x1234);
	Test(oLas.GetFileSourceId() == 0x1234);

	oLas.SetGlobalEncoding(true);
	Test(oLas.IsGlobalEncoding() == true);

	oGuidVolatile.sData1 = 0xFEDCBA98;
	oGuidVolatile.sData2 = 0x7654;
	oGuidVolatile.sData3 = 0x3210;
	oGuidVolatile.sData4[0] = 0xFE;
	oGuidVolatile.sData4[1] = 0xDC;
	oGuidVolatile.sData4[2] = 0xBA;
	oGuidVolatile.sData4[3] = 0x98;
	oGuidVolatile.sData4[4] = 0x76;
	oGuidVolatile.sData4[5] = 0x54;
	oGuidVolatile.sData4[6] = 0x32;
	oGuidVolatile.sData4[7] = 0x10;
	oLas.SetGuid(oGuidVolatile);
	Test(_cmp_LASsieGuid(oLas.GetGuid(), oGuidVolatile) == 0);

	oLas.SetGeneratingSoftware("Some Generating Software");
	Test(_cmp_LASsieGenSw(oLas.GetGeneratingSoftware(), "Some Generating Software") == 0);
	oLas.SetGeneratingSoftware("012345678901234567890123456789");
	Test(_cmp_LASsieGenSw(oLas.GetGeneratingSoftware(), "012345678901234567890123456789") == 0);
	oLas.SetGeneratingSoftware("0123456789012345678901234567891");
	Test(_cmp_LASsieGenSw(oLas.GetGeneratingSoftware(), "0123456789012345678901234567891") == 0);
	oLas.SetGeneratingSoftware("01234567890123456789012345678912");
	Test(_cmp_LASsieGenSw(oLas.GetGeneratingSoftware(), "01234567890123456789012345678912") == 0);
	oLas.SetGeneratingSoftware("012345678901234567890123456789123");
	Test(_cmp_LASsieGenSw(oLas.GetGeneratingSoftware(), "01234567890123456789012345678912") == 0); // already cuts string longer than 32 chars
	oLas.SetGeneratingSoftware("012345678901234567890123456789123456789");
	Test(_cmp_LASsieGenSw(oLas.GetGeneratingSoftware(), "01234567890123456789012345678912") == 0);

	oLas.SetCreation(2016, 366);
	Test(oLas.GetCreationDay() == 366);
	Test(oLas.GetCreationYear() == 2016);

	oLas.SetPointDataFormat(modri::LASsie::PointDataFormat3);
	Test(oLas.GetPointDataFormat() == modri::LASsie::PointDataFormat3);

	oLas.SetScaleFactor(1.1, 1.2, 1.3);
	oCoordVolatile.sX = 1.1;
	oCoordVolatile.sY = 1.2;
	oCoordVolatile.sZ = 1.3;
	Test(_cmp_LASsieCoord(oLas.GetScaleFactor(), oCoordVolatile) == 0);

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


// main()

int main(int argc, char **argv)
{
	Test(TestLASsieInit() == 0);
	Test(TestLASsieSetters() == 0);
	
	printf("=== ALL TESTS PASSED ===\n");

    return 0;
}
