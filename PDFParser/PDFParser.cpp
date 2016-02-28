#include "stdafx.h"
#include "PDFParser.h"

#include <vector>
#include <string>

PDFParser::PDFParser(PdfMemDocument *pDoc)
{
	m_pDoc = pDoc;
}


PDFParser::~PDFParser()
{
}

void PDFParser::ParsePath()
{
	int count = m_pDoc->GetPageCount();

	for (int i = 0; i < count; i++)
	{
		PdfPage *pPage = m_pDoc->GetPage(i);

		ParsePage(pPage);
	}
}

void PDFParser::ParsePage(PdfPage *pPage)
{
	const char*      pszToken = NULL;
	PdfVariant       var;
	EPdfContentsType eType;

	PdfContentsTokenizer tokenizer(pPage);
	std::vector<PdfVariant> vVar;

	while (tokenizer.ReadNext(eType, pszToken, var))
	{
		if (eType == ePdfContentsType_Keyword)
		{

			vVar.clear();
		}
		else if (eType == ePdfContentsType_Variant)
		{
			vVar.push_back(var);
		}
		else if (eType == ePdfContentsType_ImageData)
		{
			vVar.push_back(var);
		}
	}


}