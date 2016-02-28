#include "stdafx.h"

#include "TransformMatrix.h"

TransformMatrix::TransformMatrix() 
	: m_a(1),
	m_b(0),
	m_c(0),
	m_d(1),
	m_e(0),
	m_f(0)
{
}

TransformMatrix::TransformMatrix(double a, double b, double c, double d, double e, double f)
	: m_a(a),
	m_b(b),
	m_c(c),
	m_d(d),
	m_e(e),
	m_f(f)
{

}


TransformMatrix::~TransformMatrix()
{
}
