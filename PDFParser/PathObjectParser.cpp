#include "stdafx.h"
#include "PathObjectParser.h"

#include <vector>
#include <string>

PathObjectParser::PathObjectParser(PdfMemDocument *pDoc)
{
	m_pDoc = pDoc;
}


PathObjectParser::~PathObjectParser()
{
}

void PathObjectParser::ParsePath()
{
	int count = m_pDoc->GetPageCount();

	for (int i = 0; i < count; i++)
	{
		PdfPage *pPage = m_pDoc->GetPage(i);

		ParsePage(pPage);
	}
}

void PathObjectParser::ParsePage(PdfPage *pPage)
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