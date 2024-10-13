#ifndef KAKTUS_EPHEMERIS_H
#define KAKTUS_EPHEMERIS_H
#include "kaktus/util.h"
namespace kaktus::astor
{
	class Ephemeris
	{
	private:
		double t0, star, end;
	public:
		virtual void getPosition(Eigen::Vector3d& p, double time);
		virtual void getVelocity(Eigen::Vector3d& v, double time);
		virtual void getPV(Eigen::Vector6d& v, double time);
		virtual std::string getBody(double time);
		virtual double getStep(double time);
	};
	class BattinEphemeris :public Ephemeris
	{
	private:
		Eigen::Vector3d pa, pav;
		double r, al;
	public:
	};
	class ConstEphemeris
	{
	private:
		const Eigen::Vector6d state;
	public:
		ConstEphemeris(const Eigen::Vector6d &state) :state(state) {};
	};
	class TabulatedEphemris:public Ephemeris
	{
	private:
		std::shared_ptr<std::vector<Eigen::Vector6d>> history;
	};
}
#endif // !KAKTUS_EPHEMERIS_H
