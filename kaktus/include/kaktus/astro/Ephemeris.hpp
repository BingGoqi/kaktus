#ifndef KAKTUS_EPHEMERIS_H
#define KAKTUS_EPHEMERIS_H
#include <Eigen/Geometry>
#include "kaktus/util.h"
namespace kaktus::astro
{
	//设置好轨道外推的模型后，进行二次编译优化并计算//参考julia
	class Ephemeris
	{
	private:
		//double t0, star, end;
		string FrameBody, Body;
	public:
		Ephemeris(string fb, string bb) :FrameBody(fb), Body(bb) {}
		virtual void getPV(Eigen::Vector6d& pv, double time) = 0;
		virtual string getCentralBody(double time) { return FrameBody; }
		virtual double getStep(double time) = 0;
		//virtual double getLocalTime(const Eigen::Vector3d& p, double time) { return time - p.size() / Light; }
	};
	enum KplEle
	{
		a, e, t, la, i, ap
	};
	class KplEphemeris :public Ephemeris
	{
	private:
		//Eigen::Vector3d r0, v0;
		double mu, n, p;
		Eigen::Vector6d kele;
		Eigen::Quaterniond Plane;
		//double r, al;
	public:
		KplEphemeris(string fb, string bb, Eigen::Vector6d& kele, double mu)
			:kele(kele), Ephemeris(fb, bb), mu(mu)
		{
			p = kele(e) == 1 ? kele(a) : kele(a) * abs(1 - pow2(kele(e)));
			/*
			v = sqrt(mu / p) * (1 + kele(e));
			r = p / (1 + kele(e));
			*/
			n = sqrt(mu / abs(pow3(kele(a))));
			Plane =
				Eigen::AngleAxisd(kele(la), Eigen::Vector3d::UnitZ()) *
				Eigen::AngleAxisd(kele(i), Eigen::Vector3d::UnitX()) *
				Eigen::AngleAxisd(kele(ap), Eigen::Vector3d::UnitZ());
		}
		virtual void getPV(Eigen::Vector6d& pv, double time);
		virtual double getStep(double time);
		virtual double getLocalTime(const Eigen::Vector3d& p, double time);
	};
	class AprxEphemeris :public Ephemeris
	{
	private:
		double mu, n, p;
		double pt;
		Eigen::Matrix6d sl;
		Eigen::Vector6d nke;
		Eigen::Quaterniond Plane;
		//fma(v1, v2, t) + v3 * sin(fma(v4, v5, t));
		//1/sec
	public:
		AprxEphemeris(string fb, string bb, Eigen::Matrix6d& cp, double mu)
			:sl(cp), Ephemeris(fb, bb), mu(mu)
		{
			//参考aprx_pos_planets_pdf
			pt = 0;
			Plane = Eigen::Quaterniond::Identity();
		};
		void setAprx(int type, const Eigen::Vector6d& val);
		void setAprx(int type, int index, double val);
		void flushKeplerElements(double time);
		virtual void getPV(Eigen::Vector6d& pv, double time);
		virtual double getStep(double time);
		virtual double getLocalTime(const Eigen::Vector3d& p, double time);
	};
	class ConstEphemeris :public Ephemeris
	{
	private:
		const Eigen::Vector3d state;
	public:
		ConstEphemeris(string fb, string bb, Eigen::Vector3d& state) :Ephemeris(fb, bb), state(state) {};
		void getPV(Eigen::Vector6d& pv, double time) { pv = { state(0),state(0),state(0),0,0,0}; }
	};
	class TabulatedEphemris :public Ephemeris
	{
	private:
		std::shared_ptr<std::vector<Eigen::Vector6d>> history;
	public:

	};
}
#endif // !KAKTUS_EPHEMERIS_H