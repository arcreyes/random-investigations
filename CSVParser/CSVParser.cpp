#include "stdafx.h"

#include "CSVParser.h"

#include <string>


CsvParser::CsvParser(const TCHAR *filePath, const TCHAR *delimiter, int firstLineIsHeader) 
{
	if (filePath == NULL) {
		filePath_ = NULL;
	}
	else {
		size_t filePathLen = _tcslen(filePath);
		filePath_ = new TCHAR[filePathLen + 1];
		_tcscpy(filePath_, filePath);
	}
	firstLineIsHeader_ = firstLineIsHeader;
	errMsg_ = NULL;
	if (delimiter == NULL) {
		delimiter_ = ',';
	}
	else if (_CsvParser_delimiterIsAccepted(delimiter)) {
		delimiter_ = *delimiter;
	}
	else {
		delimiter_ = '\0';
	}
	header_ = NULL;
	fileHandler_ = NULL;
	fromString_ = 0;
	csvString_ = NULL;
	csvStringIter_ = 0;
}

//CsvParser *CsvParser::CsvParser_new_from_string(const TCHAR *csvString, const TCHAR *delimiter, int firstLineIsHeader) {
//	CsvParser *csvParser = CsvParser_new(NULL, delimiter, firstLineIsHeader);
//	csvParser->fromString_ = 1;
//	if (csvString != NULL) {
//		size_t csvStringLen = _tcslen(csvString);
//		csvParser->csvString_ = new TCHAR[csvStringLen + 1];
//		_tcscpy(csvParser->csvString_, csvString);
//	}
//	return csvParser;
//}

CsvParser::~CsvParser(void) 
{
	if (filePath_ != NULL) {
		delete[] (filePath_);
	}
	if (errMsg_ != NULL) {
		delete[] (errMsg_);
	}
	if (fileHandler_ != NULL) {
		fclose(fileHandler_);
	}
	if (header_ != NULL) {
		DestroyRow(header_);
	}
	if (csvString_ != NULL) {
		delete[] (csvString_);
	}
}

void CsvParser::DestroyRow(CsvRow *csvRow)
{
	int i;
	for (i = 0; i < csvRow->numOfFields_; i++) {
		delete[] (csvRow->fields_[i]);
	}
	delete (csvRow);
}

CsvRow *CsvParser::GetHeader(void) 
{
	if (!firstLineIsHeader_) {
		_CsvParser_setErrorMessage(_T("Cannot supply header, as current CsvParser object does not support header"));
		return NULL;
	}
	if (header_ == NULL) {
		header_ = _CsvParser_getRow();
	}
	return header_;
}

CsvRow *CsvParser::GetRow()
{
	if (firstLineIsHeader_ && header_ == NULL) {
		header_ = _CsvParser_getRow();
	}
	return _CsvParser_getRow();
}

int CsvParser::GetNumFields(CsvRow *csvRow) 
{
	return csvRow->numOfFields_;
}

std::vector<TCHAR*> CsvParser::GetFields(CsvRow *csvRow) 
{
	return csvRow->fields_;
}

CsvRow *CsvParser::_CsvParser_getRow(void) {
	int numRowRealloc = 0;
	int acceptedFields = 64;
	int acceptedCharsInField = 64;
	if (filePath_ == NULL && (!fromString_)) {
		_CsvParser_setErrorMessage(_T("Supplied CSV file path is NULL"));
		return NULL;
	}
	if (csvString_ == NULL && fromString_) {
		_CsvParser_setErrorMessage(_T("Supplied CSV string is NULL"));
		return NULL;
	}
	if (delimiter_ == '\0') {
		_CsvParser_setErrorMessage(_T("Supplied delimiter is not supported"));
		return NULL;
	}
	if (!fromString_) {
		if (fileHandler_ == NULL) {
			fileHandler_ = _tfopen(filePath_, _T("r"));
			if (fileHandler_ == NULL) {
				int errorNum = errno;
				const TCHAR *errStr = _tcserror(errorNum);
				size_t count = 1024 + _tcslen(errStr);
				TCHAR *errMsg = new TCHAR[count];
				_tcscpy(errMsg, _T(""));
#ifdef _UNICODE
				swprintf(errMsg, count, _T("Error opening CSV file for reading: %s : %s"), filePath_, errStr);
#else
				sprintf(errMsg, _T("Error opening CSV file for reading: %s : %s"), csvParser->filePath_, errStr);
#endif
				_CsvParser_setErrorMessage(errMsg);
				delete[] (errMsg);
				return NULL;
			}
		}
	}
	CsvRow *csvRow = new CsvRow;
	csvRow->fields_.resize(acceptedFields);
	csvRow->numOfFields_ = 0;
	int fieldIter = 0;
	std::vector<TCHAR> currField;
	currField.resize(acceptedCharsInField);
	//TCHAR *currField = new TCHAR[acceptedCharsInField];
	int inside_complex_field = 0;
	int currFieldCharIter = 0;
	int seriesOfQuotesLength = 0;
	int lastCharIsQuote = 0;
	int isEndOfFile = 0;
	while (1) {
		TCHAR currChar = (fromString_) ? csvString_[csvStringIter_] : fgetwc(fileHandler_);
		csvStringIter_++;
		int endOfFileIndicator;
		if (fromString_) {
			endOfFileIndicator = (currChar == '\0');
		}
		else {
			endOfFileIndicator = feof(fileHandler_);
		}
		if (endOfFileIndicator) {
			if (currFieldCharIter == 0 && fieldIter == 0) {
				_CsvParser_setErrorMessage(_T("Reached EOF"));
				return NULL;
			}
			currChar = '\n';
			isEndOfFile = 1;
		}
		if (currChar == '\r') {
			continue;
		}
		if (currFieldCharIter == 0 && !lastCharIsQuote) {
			if (currChar == '\"') {
				inside_complex_field = 1;
				lastCharIsQuote = 1;
				continue;
			}
		}
		else if (currChar == '\"') {
			seriesOfQuotesLength++;
			inside_complex_field = (seriesOfQuotesLength % 2 == 0);
			if (inside_complex_field) {
				currFieldCharIter--;
			}
		}
		else {
			seriesOfQuotesLength = 0;
		}
		if (isEndOfFile || ((currChar == delimiter_ || currChar == '\n') && !inside_complex_field)){
			currField[lastCharIsQuote ? currFieldCharIter - 1 : currFieldCharIter] = '\0';
			csvRow->fields_[fieldIter] = new TCHAR[currFieldCharIter + 1];
			_tcscpy(csvRow->fields_[fieldIter], &currField[0]);
			currField.clear();

			csvRow->numOfFields_++;
			if (currChar == '\n') {
				return csvRow;
			}
			if (csvRow->numOfFields_ != 0 && csvRow->numOfFields_ % acceptedFields == 0) {
				csvRow->fields_.resize((numRowRealloc + 2) * acceptedFields) ;
				numRowRealloc++;
			}
			acceptedCharsInField = 64;
			currField.resize(acceptedCharsInField);
			currFieldCharIter = 0;
			fieldIter++;
			inside_complex_field = 0;
		}
		else {
			currField[currFieldCharIter] = currChar;
			currFieldCharIter++;
			if (currFieldCharIter == acceptedCharsInField - 1) {
				acceptedCharsInField *= 2;
				currField.resize(acceptedCharsInField);
			}
		}
		lastCharIsQuote = (currChar == '\"') ? 1 : 0;
	}
}

int CsvParser::_CsvParser_delimiterIsAccepted(const TCHAR *delimiter) 
{
	TCHAR actualDelimiter = *delimiter;
	if (actualDelimiter == '\n' || actualDelimiter == '\r' || actualDelimiter == '\0' ||
		actualDelimiter == '\"') {
		return 0;
	}
	return 1;
}

void CsvParser::_CsvParser_setErrorMessage(const TCHAR *errorMessage) 
{
	if (errMsg_ != NULL) 
	{
		delete[] (errMsg_);
	}
	size_t errMsgLen = _tcslen(errorMessage);
	errMsg_ = new TCHAR[errMsgLen + 1];
	_tcscpy(errMsg_, errorMessage);
}

const TCHAR *CsvParser::GetErrorMessage(void) 
{
	return errMsg_;
}
