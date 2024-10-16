#include "kaktus/astor/Ephemeris.h"
#include "kaktus/math.h"
//#include <cmath>
#include <tudat/astro/basic_astro/orbitalElementConversions.h>
//#include <tudat/astro/ephemerides/ephemeris.h>
namespace kaktus::astor
{
	enum KplEme
	{

	};
	void AprxEphemeris::setAprx(int type, const Eigen::Vector6d& val)
	{
		sl.row(type) = val;
	}
	void AprxEphemeris::setAprx(int type, int index, double val)
	{
		sl(type, index) = val;
	}
	void AprxEphemeris::flushKeplerElements(double time)
	{
		if (abs(time - pt) < 1e-3) return;
		pt = time;
		for (int i = 0; i < 6; i++)
		{
			nke[i] = 0;
			switch ((int)sl(i, 0) & 3)
			{
			case 0:
				nke[i] = sl(i, 1); break;
			case 1:
				nke[i] = fma(sl(i, 2), time, sl(i, 1)); break;
			case 3:
				nke[i] = fma(sl(i, 2), time, sl(i, 1));
			case 2:
				nke[i] += sl(i, 3) * sin(fma(sl(i, 4), time, sl(i, 5))); break;
			}
		}
		p = nke(e) == 1?nke(a):nke(a) * abs(1 - pow2(nke(e)));
		n = sqrt(mu / abs(pow3(nke(a))));
		Plane =
			Eigen::AngleAxisd(nke(la), Eigen::Vector3d::UnitZ()) *
			Eigen::AngleAxisd(nke(i), Eigen::Vector3d::UnitX()) *
			Eigen::AngleAxisd(nke(ap), Eigen::Vector3d::UnitZ());
	}
	void AprxEphemeris::getPV(Eigen::Vector6d& pv, double time)
	{//改写成相邻时域PCHIP插值
		flushKeplerElements(time);
		const double
			e = nke(KplEle::e),
			E = kaktus::math::KeplerSlove(e, n, time - nke(t));
		const SinCos f = kaktus::math::E2f(e, E);
		const double r = this->p / (1 + e * f.cos);
		pv(0) = r * f.cos;
		pv(1) = r * f.sin;
		pv(2) = 0;
		pv(3) = sqrt(mu / this->p);
		pv(4) = pv(3) * (e + f.cos);
		pv(3) = -pv(3) * f.sin;
		pv(5) = 0;
		pv.segment(0, 3) = pv.segment(0, 3) * Plane;
		pv.segment(3, 3) = pv.segment(3, 3) * Plane;
	}
	//Eigen::Vector6d getCartesianState(double time)
	//{//改写成相邻时域PCHIP插值
	//	Eigen::Vector6d currentCartesianState = Eigen::Vector6d::Zero();
	//	getCartesianState(time, currentCartesianState);
	//	return currentCartesianState;
	//}
	void KplEphemeris::getPV(Eigen::Vector6d& pv, double time)
	{
		const double
			e = kele(KplEle::e),
			E = kaktus::math::KeplerSlove(e, n, time - kele(t));
		const SinCos f = kaktus::math::E2f(e, E);
		const double r = this->p / (1 + e * f.cos);
		pv(0) = r * f.cos;
		pv(1) = r * f.sin;
		pv(2) = 0;
		pv(3) = sqrt(mu / this->p);
		pv(4) = pv(3) * (e + f.cos);
		pv(3) = -pv(3) * f.sin;
		pv(5) = 0;
		pv.segment(0, 3) = pv.segment(0, 3) * Plane;
		pv.segment(3, 3) = pv.segment(3, 3) * Plane;
	}
	//KplEphemeris::getPosition
}