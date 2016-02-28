// CSVParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CSVParser.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0;
	//                                   file, delimiter, first_line_is_header?
	CsvParser csvparser(_T("C:\\Users\\arcreyes\\Desktop\\escape.csv"), _T(","), 1);
	CsvRow *header;
	CsvRow *row;

	header = csvparser.GetHeader();
	if (header == NULL) {
		_tprintf(_T("%s\n"), csvparser.GetErrorMessage());
		return 1;
	}
	std::vector<TCHAR*> headerFields = csvparser.GetFields(header);
	for (i = 0; i < csvparser.GetNumFields(header); i++) {
		_tprintf(_T("TITLE: %s\n"), headerFields[i]);
	}
	// Do NOT destroy the headear manually if you plan to destroy the parser later.
	// If you destroy both header and parser, you will get double free runtime error
	// CsvParser_destroy_row(header);
	while ((row = csvparser.GetRow())) {
		_tprintf(_T("NEW LINE:\n"));
		std::vector<TCHAR*> rowFields = csvparser.GetFields(row);
		for (i = 0; i < csvparser.GetNumFields(row); i++) {
			_tprintf(_T("FIELD: %s\n"), rowFields[i]);
		}
		csvparser.DestroyRow(row);
	}
	//CsvParser_destroy(csvparser);
	return 0;
}

