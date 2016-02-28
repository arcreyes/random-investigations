#pragma once

// From
// https://sourceforge.net/projects/cccsvparser/files/2015-08-01/

#include <Windows.h>

#include <vector>

typedef struct CsvRow {
	std::vector<TCHAR*> fields_;
	int numOfFields_;
} CsvRow;

typedef struct CsvParser {
	TCHAR *filePath_;
	TCHAR delimiter_;
	int firstLineIsHeader_;
	TCHAR *errMsg_;
	CsvRow *header_;
	FILE *fileHandler_;
	int fromString_;
	TCHAR *csvString_;
	int csvStringIter_;
} CsvParser;


// Public
CsvParser *CsvParser_new(const TCHAR *filePath, const TCHAR *delimiter, int firstLineIsHeader);
CsvParser *CsvParser_new_from_string(const TCHAR *csvString, const TCHAR *delimiter, int firstLineIsHeader);
void CsvParser_destroy(CsvParser *csvParser);
void CsvParser_destroy_row(CsvRow *csvRow);
CsvRow *CsvParser_getHeader(CsvParser *csvParser);
CsvRow *CsvParser_getRow(CsvParser *csvParser);
int CsvParser_getNumFields(CsvRow *csvRow);
std::vector<TCHAR*> CsvParser_getFields(CsvRow *csvRow);
const TCHAR* CsvParser_getErrorMessage(CsvParser *csvParser);

// Private
CsvRow *_CsvParser_getRow(CsvParser *csvParser);
int _CsvParser_delimiterIsAccepted(const TCHAR *delimiter);
void _CsvParser_setErrorMessage(CsvParser *csvParser, const TCHAR *errorMessage);

