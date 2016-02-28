#pragma once

class TransformMatrix
{
public:
	TransformMatrix();
	TransformMatrix(double a, double b, double c, double d, double e, double f);
	~TransformMatrix();

private:
	double m_a;
	double m_b;
	double m_c;
	double m_d;
	double m_e;
	double m_f;
};

