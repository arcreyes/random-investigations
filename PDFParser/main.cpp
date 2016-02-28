// PDFParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "PathObjectParser.h"

int _tmain(int argc, _TCHAR* argv[])
{
	PdfMemDocument doc;

	PathObjectParser parser(&doc);

	parser.ParsePath();

	return 0;
}

