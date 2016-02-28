#include "stdafx.h"

#include "CSVParser.h"

#include <string>


CsvParser *CsvParser_new(const TCHAR *filePath, const TCHAR *delimiter, int firstLineIsHeader) {
	CsvParser *csvParser = new CsvParser;
	if (filePath == NULL) {
		csvParser->filePath_ = NULL;
	}
	else {
		size_t filePathLen = _tcslen(filePath);
		csvParser->filePath_ = new TCHAR[filePathLen + 1];
		_tcscpy(csvParser->filePath_, filePath);
	}
	csvParser->firstLineIsHeader_ = firstLineIsHeader;
	csvParser->errMsg_ = NULL;
	if (delimiter == NULL) {
		csvParser->delimiter_ = ',';
	}
	else if (_CsvParser_delimiterIsAccepted(delimiter)) {
		csvParser->delimiter_ = *delimiter;
	}
	else {
		csvParser->delimiter_ = '\0';
	}
	csvParser->header_ = NULL;
	csvParser->fileHandler_ = NULL;
	csvParser->fromString_ = 0;
	csvParser->csvString_ = NULL;
	csvParser->csvStringIter_ = 0;

	return csvParser;
}

CsvParser *CsvParser_new_from_string(const TCHAR *csvString, const TCHAR *delimiter, int firstLineIsHeader) {
	CsvParser *csvParser = CsvParser_new(NULL, delimiter, firstLineIsHeader);
	csvParser->fromString_ = 1;
	if (csvString != NULL) {
		size_t csvStringLen = _tcslen(csvString);
		csvParser->csvString_ = new TCHAR[csvStringLen + 1];
		_tcscpy(csvParser->csvString_, csvString);
	}
	return csvParser;
}

void CsvParser_destroy(CsvParser *csvParser) {
	if (csvParser == NULL) {
		return;
	}
	if (csvParser->filePath_ != NULL) {
		delete[] (csvParser->filePath_);
	}
	if (csvParser->errMsg_ != NULL) {
		delete[] (csvParser->errMsg_);
	}
	if (csvParser->fileHandler_ != NULL) {
		fclose(csvParser->fileHandler_);
	}
	if (csvParser->header_ != NULL) {
		CsvParser_destroy_row(csvParser->header_);
	}
	if (csvParser->csvString_ != NULL) {
		delete[] (csvParser->csvString_);
	}
	delete (csvParser);
}

void CsvParser_destroy_row(CsvRow *csvRow) {
	int i;
	for (i = 0; i < csvRow->numOfFields_; i++) {
		delete[] (csvRow->fields_[i]);
	}
	delete (csvRow);
}

CsvRow *CsvParser_getHeader(CsvParser *csvParser) {
	if (!csvParser->firstLineIsHeader_) {
		_CsvParser_setErrorMessage(csvParser, _T("Cannot supply header, as current CsvParser object does not support header"));
		return NULL;
	}
	if (csvParser->header_ == NULL) {
		csvParser->header_ = _CsvParser_getRow(csvParser);
	}
	return csvParser->header_;
}

CsvRow *CsvParser_getRow(CsvParser *csvParser) {
	if (csvParser->firstLineIsHeader_ && csvParser->header_ == NULL) {
		csvParser->header_ = _CsvParser_getRow(csvParser);
	}
	return _CsvParser_getRow(csvParser);
}

int CsvParser_getNumFields(CsvRow *csvRow) {
	return csvRow->numOfFields_;
}

std::vector<TCHAR*> CsvParser_getFields(CsvRow *csvRow) {
	return csvRow->fields_;
}

CsvRow *_CsvParser_getRow(CsvParser *csvParser) {
	int numRowRealloc = 0;
	int acceptedFields = 64;
	int acceptedCharsInField = 64;
	if (csvParser->filePath_ == NULL && (!csvParser->fromString_)) {
		_CsvParser_setErrorMessage(csvParser, _T("Supplied CSV file path is NULL"));
		return NULL;
	}
	if (csvParser->csvString_ == NULL && csvParser->fromString_) {
		_CsvParser_setErrorMessage(csvParser, _T("Supplied CSV string is NULL"));
		return NULL;
	}
	if (csvParser->delimiter_ == '\0') {
		_CsvParser_setErrorMessage(csvParser, _T("Supplied delimiter is not supported"));
		return NULL;
	}
	if (!csvParser->fromString_) {
		if (csvParser->fileHandler_ == NULL) {
			csvParser->fileHandler_ = _tfopen(csvParser->filePath_, _T("r"));
			if (csvParser->fileHandler_ == NULL) {
				int errorNum = errno;
				const TCHAR *errStr = _tcserror(errorNum);
				size_t count = 1024 + _tcslen(errStr);
				TCHAR *errMsg = new TCHAR[count];
				_tcscpy(errMsg, _T(""));
#ifdef _UNICODE
				swprintf(errMsg, count, _T("Error opening CSV file for reading: %s : %s"), csvParser->filePath_, errStr);
#else
				sprintf(errMsg, _T("Error opening CSV file for reading: %s : %s"), csvParser->filePath_, errStr);
#endif
				_CsvParser_setErrorMessage(csvParser, errMsg);
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
		TCHAR currChar = (csvParser->fromString_) ? csvParser->csvString_[csvParser->csvStringIter_] : fgetwc(csvParser->fileHandler_);
		csvParser->csvStringIter_++;
		int endOfFileIndicator;
		if (csvParser->fromString_) {
			endOfFileIndicator = (currChar == '\0');
		}
		else {
			endOfFileIndicator = feof(csvParser->fileHandler_);
		}
		if (endOfFileIndicator) {
			if (currFieldCharIter == 0 && fieldIter == 0) {
				_CsvParser_setErrorMessage(csvParser, _T("Reached EOF"));
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
		if (isEndOfFile || ((currChar == csvParser->delimiter_ || currChar == '\n') && !inside_complex_field)){
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

int _CsvParser_delimiterIsAccepted(const TCHAR *delimiter) {
	TCHAR actualDelimiter = *delimiter;
	if (actualDelimiter == '\n' || actualDelimiter == '\r' || actualDelimiter == '\0' ||
		actualDelimiter == '\"') {
		return 0;
	}
	return 1;
}

void _CsvParser_setErrorMessage(CsvParser *csvParser, const TCHAR *errorMessage) {
	if (csvParser->errMsg_ != NULL) {
		delete[] (csvParser->errMsg_);
	}
	size_t errMsgLen = _tcslen(errorMessage);
	csvParser->errMsg_ = new TCHAR[errMsgLen + 1];
	_tcscpy(csvParser->errMsg_, errorMessage);
}

const TCHAR *CsvParser_getErrorMessage(CsvParser *csvParser) {
	return csvParser->errMsg_;
}
