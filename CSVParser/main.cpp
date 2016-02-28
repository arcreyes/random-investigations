// CSVParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CSVParser.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0;
	//                                   file, delimiter, first_line_is_header?
	CsvParser *csvparser = CsvParser_new(_T("C:\\Users\\arcreyes\\Desktop\\unicodedata.csv"), _T(","), 1);
	CsvRow *header;
	CsvRow *row;

	header = CsvParser_getHeader(csvparser);
	if (header == NULL) {
		_tprintf(_T("%s\n"), CsvParser_getErrorMessage(csvparser));
		return 1;
	}
	std::vector<TCHAR*> headerFields = CsvParser_getFields(header);
	for (i = 0; i < CsvParser_getNumFields(header); i++) {
		_tprintf(_T("TITLE: %s\n"), headerFields[i]);
	}
	// Do NOT destroy the headear manually if you plan to destroy the parser later.
	// If you destroy both header and parser, you will get double free runtime error
	// CsvParser_destroy_row(header);
	while ((row = CsvParser_getRow(csvparser))) {
		_tprintf(_T("NEW LINE:\n"));
		std::vector<TCHAR*> rowFields = CsvParser_getFields(row);
		for (i = 0; i < CsvParser_getNumFields(row); i++) {
			_tprintf(_T("FIELD: %s\n"), rowFields[i]);
		}
		CsvParser_destroy_row(row);
	}
	CsvParser_destroy(csvparser);
	return 0;
}

