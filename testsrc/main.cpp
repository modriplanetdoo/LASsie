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

	Test(oLas.GetFileSrcId() == 0);
	Test(oLas.IsGlobalEnc() == false);
	Test(_cmp_LASsieGuid(oLas.GetGuid(), oGuidZero) == 0);
	Test(_cmp_LASsieGenSw(oLas.GetGenerSw(), "") == 0);
	Test(oLas.GetCreatDay() == 0);
	Test(oLas.GetCreatYear() == 0);
	Test(oLas.GetPdrf() == modri::LASsie::Pdrf0);
	Test(_cmp_LASsieCoord(oLas.GetScale(), oCoordZero) == 0);
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

	oLas.SetGenerSw("Some Generating Software");
	Test(_cmp_LASsieGenSw(oLas.GetGenerSw(), "Some Generating Software") == 0);
	oLas.SetGenerSw("012345678901234567890123456789");
	Test(_cmp_LASsieGenSw(oLas.GetGenerSw(), "012345678901234567890123456789") == 0);
	oLas.SetGenerSw("0123456789012345678901234567891");
	Test(_cmp_LASsieGenSw(oLas.GetGenerSw(), "0123456789012345678901234567891") == 0);
	oLas.SetGenerSw("01234567890123456789012345678912");
	Test(_cmp_LASsieGenSw(oLas.GetGenerSw(), "01234567890123456789012345678912") == 0);
	oLas.SetGenerSw("012345678901234567890123456789123");
	Test(_cmp_LASsieGenSw(oLas.GetGenerSw(), "01234567890123456789012345678912") == 0); // already cuts string longer than 32 chars
	oLas.SetGenerSw("012345678901234567890123456789123456789");
	Test(_cmp_LASsieGenSw(oLas.GetGenerSw(), "01234567890123456789012345678912") == 0);

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


// main()

int main(int argc, char **argv)
{
	Test(TestLASsieInit() == 0);
	Test(TestLASsieSetters() == 0);
	
	printf("=== ALL TESTS PASSED ===\n");

    return 0;
}
