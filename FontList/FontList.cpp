// FontList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>
#include <strsafe.h>

#include <iostream>
#include <vector>


using namespace std;


vector<wstring> vFontList;

int CALLBACK EnumFontFamExProc(
	ENUMLOGFONTEX *lpelfe,
	NEWTEXTMETRICEX *lpntme,
DWORD FontType,
LPARAM lParam
)
{
	//wcout << lpelfe->elfFullName << L"\n";

	if (find(vFontList.begin(), vFontList.end(), lpelfe->elfFullName) == vFontList.end())
	{
		vFontList.push_back(lpelfe->elfFullName);
	}



	//Return non--zero to continue enumeration
	return 1;
}


void ListFonts(BYTE charSet)
{
	LOGFONT lf;
	lf.lfFaceName[0] = '\0';
	lf.lfCharSet = charSet;
	HDC hDC = ::GetDC(NULL);
	EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC)&EnumFontFamExProc, 0, 0);
	ReleaseDC(NULL, hDC);
}

static int CHARSET_LIST[] =
{
	ANSI_CHARSET,
	DEFAULT_CHARSET,
	SYMBOL_CHARSET,
	SHIFTJIS_CHARSET,
	HANGEUL_CHARSET,
	HANGUL_CHARSET,
	GB2312_CHARSET,
	CHINESEBIG5_CHARSET,
	OEM_CHARSET,
	JOHAB_CHARSET,
	HEBREW_CHARSET,
	ARABIC_CHARSET,
	GREEK_CHARSET,
	TURKISH_CHARSET,
	VIETNAMESE_CHARSET,
	THAI_CHARSET,
	EASTEUROPE_CHARSET,
	RUSSIAN_CHARSET,

	MAC_CHARSET,
	BALTIC_CHARSET
};

static int CHAR_SET_COUNT = sizeof(CHARSET_LIST) / sizeof(int);



void PrintFontList()
{
	for (int i = 0; i < CHAR_SET_COUNT; i++)
	{
		ListFonts(CHARSET_LIST[i]);
	}

	for (size_t j = 0; j < vFontList.size(); j++)
	{
		wcout << vFontList[j].c_str() << L"\n";
	}
}

int LoadLogFont(LOGFONT &lf, _TCHAR *faceName, bool bold, bool italic, bool underline, BYTE charSet = DEFAULT_CHARSET)
{
	HRESULT hr;

	lf.lfHeight = 0;
	lf.lfWidth = 0;

	lf.lfWeight = bold ? FW_BOLD : FW_REGULAR;
	lf.lfItalic = italic;
	lf.lfUnderline = underline;

	lf.lfCharSet = charSet;

	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = ANTIALIASED_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	hr = StringCchCopy(lf.lfFaceName, LF_FACESIZE, faceName);
	if (FAILED(hr))
	{
		return -1;
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;


	LOGFONT lf = { 0 };
	if (LoadLogFont(lf, L"‚l‚r ƒSƒVƒbƒN", false, false, false) != 0)
	{
		return -1;
	}

	//HFONT hf;
	HFONT hf = CreateFontIndirect(&lf);
	if (!hf)
	{
		return -1;
	}

	SelectObject(NULL, hf);

	return 0;
}

