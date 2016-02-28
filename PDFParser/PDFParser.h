#pragma once

#include <podofo\podofo.h>

using namespace PoDoFo;

class PDFParser
{
public:
	PDFParser(PdfMemDocument *pDoc);
	~PDFParser();

	void ParsePath();
	void ParsePage(PdfPage *pPage);

private:
	PdfMemDocument *m_pDoc;



	enum EPdfKeyword
	{
		//General graphics state
		//w, J, j, M, d, ri, i, gs
		ePdfKeyword_LineWidth = 0,
		ePdfKeyword_LineCap,
		ePdfKeyword_LineJoin,
		ePdfKeyword_MiterLimit,
		ePdfKeyword_DashPattern,
		ePdfKeyword_RenderingIntent,
		ePdfKeyword_FlatnessTolerance,

		//Special graphics state
		//q, Q, cm
		ePdfKeyword_Save,
		ePdfKeyword_Restore,
		ePdfKeyword_CommonTransformMatrix,

		//Path painting
		//m, l, c, v, y, h, re
		ePdfKeyword_Move,
		ePdfKeyword_Line,
		ePdfKeyword_Curve,
		ePdfKeyword_Curve2,
		ePdfKeyword_Curve3,
		ePdfKeyword_Close,
		ePdfKeyword_Rectangle,

		//Clipping paths
		//W, W*
		ePdfKeyword_Clip_NonZero,
		ePdfKeyword_Clip_EvenOdd,


		//Color
		//CS, cs, SC, SCN, sc, scn, G, g, RG, rg, K, k
		ePdfKeyword_ColorSpace,
		ePdfKeyword_ColorSpace_NS,
		ePdfKeyword_Color_SC,
		ePdfKeyword_Color_SCN,
		ePdfKeyword_Color_sc,
		ePdfKeyword_Color_scn,
		ePdfKeyword_Color_G,
		ePdfKeyword_Color_g,
		ePdfKeyword_Color_RG,
		ePdfKeyword_Color_rg,
		ePdfKeyword_Color_K,
		ePdfKeyword_Color_k,


		//XObjects
		//Do
		ePdfKeyword_XObject


	};

	typedef void (*ParseVariant)(std::vector<PdfVariant> &vVar);

	struct ParseDictionary
	{
		EPdfKeyword eKeyword;
		const char *pszKeyword;
		ParseVariant func;
	};

	std::vector<ParseDictionary> m_vParseDictionary;
};

