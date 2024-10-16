#ifndef KAKTUS_MATH_H
#define KAKTUS_MATH_H
#include "kaktus/util.h"
namespace kaktus::math
{
	namespace pri
	{
		double es3(double ecc, double M)
		{
			double
				t34 = ecc * ecc,
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
		double hs(double ecc, double H)
		{
			if (abs(H) < .01)
				return H / (ecc - 1.0);
			return asinh(H / ecc);
		}
		double hps(double ecc, double H, double x)
		{
			double t1, t2, t3, t4;
			t1 = cosh(x);
			t2 = 1 - ecc * t1;
			t3 = sinh(x);
			t4 = ecc * t3;
			return (x + H - t4) / t2;
		}
		double X2u(double x, double al)
		{
			double
				pa = 0,
				pb = 1,
				a = x / 2,
				b = 1;
			double ppa, ppb;
			double ak = -al * pow2(a), bk;
			for (int i = 2; i < 15; i++)
			{
				ppa = a;
				ppb = b;
				bk = 2 * i - 1;
				a = fma(bk, a, ak * pa);
				b = fma(bk, b, ak * pb);
				pa = ppa;
				pb = ppb;
			}
			return a / b;
		}
	}
	double KeplerSlove(double ecc, double n, double dt)
	{
		if (ecc < 1)
		{
			double M = n * dt;
			double E = pri::es3(ecc, M);
			double dE = -pri::eps3(ecc, M, E);
			if (abs(dE) < 5e-16)return E + dE;
			E += dE;
			dE = -pri::eps3(ecc, M, E);
			if (abs(dE) < 5e-16)return E + dE;
			E += dE;
			dE = -pri::eps3(ecc, M, E);
			return E + dE;
		}
		else if (ecc > 1)
		{
			double H = n * dt;
			double a = pri::hs(ecc, H);
			double da = pri::hps(ecc, H, a);//ÐèÓÅ»¯
			do {
				a -= da;
			} while (abs(da) > 4e-15);
			return a;
		}
		return 0;
	}
	SinCos E2f(double ecc, double E)
	{
		if (ecc < 1)
		{
			const double
				sinE = sin(E),
				cosE = cos(E),
				n = 1 / (1 - ecc * cosE),
				sinf = sqrt(1 - pow2(ecc)) * sinE * n,
				cosf = (cosE - ecc) * n;
			return {sinf,cosf};
		}
		else if (ecc > 1)
		{
			const double
				sh = sinh(E),
				ch = cosh(E),
				n = 1 / (ecc * ch - 1),
				sinf = sqrt(1 - pow2(ecc)) * sh * n,
				cosf = (ecc - ch) * n;
			return {sinf,cosf};
		}

	}
	template<typename IN, typename OUT>
	class Function
	{
		virtual OUT val(const IN& in);
		virtual void val(const IN& in, OUT& out);
	};
	class LineSine :public Function<double, double>
	{
	private:
		Eigen::Vector6d paras;
		double pt, pv;
	public:
		double val(double time)
		{
			if (abs(time - pt) < 2e-14)
			{
				return pv;
			}
			pt = time;
			pv = 0;
			switch ((int)paras(0) & 3)
			{
			case 0:
				pv = paras(1); break;
			case 1:
				pv = fma(paras(2), time, paras(1)); break;
			case 3:
				pv = fma(paras(2), time, paras(1));
			case 2:
				pv += paras(3) * sin(fma(paras(4), time, paras(5))); break;
			}
			return pv;
		}
	};
}
#endif // !KAKTUS_MATH_H