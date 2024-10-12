#ifndef KAKTUS_MATH_H
#define KAKTUS_MATH_H
#include "kaktus/util.h"
namespace kaktus::math::pri
{
	double KeplerStart3(double ecc, double M)
	{
		double
			t34 = ecc*ecc,
			t35 = ecc * t34,
			t33 = cos(M);
		return M + (-.5 * t35 + ecc + (t34 + 1.5 * t33 * t35) * t33) * sin(M);
	}
	double eps3(double ecc, double M, double x)
	{
		double
			t1 = cos(x),
			t2 = ecc * t1 - 1,
			t3 = sin(x),
			t4 = ecc * t3,
			t5 = t4 - x + M,
			t6 = t5 / (.5 * t5 * t4 / t2 + t2);
		return t5 / ((.5 * t3 - 1. / 6. * t1 * t6) * ecc * t6 + t2);
	}
}
namespace kaktus::math
{
	double KeplerSlove(double ecc, double M)
	{//XYH
		double E = pri::KeplerStart3(ecc, M);
		E -= pri::eps3(ecc, M, E);
		E -= pri::eps3(ecc, M, E);
		E -= pri::eps3(ecc, M, E);
		return E;
	}
}
#endif // !KAKTUS_MATH_H