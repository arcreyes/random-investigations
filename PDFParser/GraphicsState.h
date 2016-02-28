#pragma once

#include "TransformMatrix.h"

#include <vector>
#include <string>

#define DEFAULT_LINEWIDTH			1.0
#define DEFAULT_DASHPATTERN			"[] 0"
#define DEFAULT_LINECAP				0
#define DEFAULT_LINEJOIN			0
#define DEFAULT_MITERLIMIT			10.0
#define DEFAULT_RENDERINGINTENT		"RelativeColorimetric"
#define DEFAULT_FLATNESS			1

class GraphicsState
{
public:
	GraphicsState();
	~GraphicsState();

	inline void AddTransformMatrix(TransformMatrix cm)
	{
		m_ctm.push_back(cm);
	}

	inline void AddTransformMatrix(double a, double b, double c, double d, double e, double f)
	{
		m_ctm.push_back(TransformMatrix(a, b, c, d, e, f));
	}

	inline const std::vector<TransformMatrix> & GetTransformMatrix(void)
	{
		return m_ctm;
	}

	inline void SetClippingPath(std::vector<std::string> clippingPath)
	{
		m_clippingPath = clippingPath;
	}

	inline const std::vector<std::string> & GetClippingPath(void)
	{
		return m_clippingPath;
	}

	inline void SetLineWidth(double lineWidth)
	{
		m_lineWidth = lineWidth;
	}

	inline double GetLineWidth(void)
	{
		return m_lineWidth;
	}

	inline void SetLineCap(int lineCap)
	{
		m_lineCap = lineCap;
	}

	inline int GetLineCap(void)
	{
		return m_lineCap;
	}

	inline void SetLineJoin(int lineJoin)
	{
		m_lineJoin = lineJoin;
	}

	inline int GetLineJoin(void)
	{
		return m_lineJoin;
	}

	inline void SetMiterLimit(double miterLimit)
	{
		m_miterLimit = miterLimit;
	}

	inline double GetMiterLimit(void)
	{
		return m_miterLimit;
	}

	inline void SetDashPattern(std::string dashPattern)
	{
		m_dashPattern = dashPattern;
	}

	inline std::string GetDashPattern(void)
	{
		return m_dashPattern;
	}


private:
	std::vector<TransformMatrix> m_ctm;

	std::vector<std::string> m_clippingPath;

	std::string		m_colorSpace;
	std::string		m_colorSpaceNS;
	std::string		m_color;
	std::string		m_colorNS;

	double			m_lineWidth;
	int				m_lineCap;
	int				m_lineJoin;
	double			m_miterLimit;
	std::string		m_dashPattern;
	std::string		m_renderingIntent;
	int				m_flatness;
	std::string		m_dictName;
};

