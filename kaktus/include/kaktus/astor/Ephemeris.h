#ifndef KAKTUS_EPHEMERIS_H
#define KAKTUS_EPHEMERIS_H
#include "kaktus/util.h"
#include <vector>
namespace kaktus::astor
{
	class Ephemeris
	{
	private:
	public:
		virtual void getPosition(Eigen::Vector3d& p, double time);
		virtual void getVelocity(Eigen::Vector3d& v, double time);
		virtual void getPV(Eigen::Vector6d& v, double time);
	};
	class KepEph :public Ephemeris
	{
	private:
		Eigen::Vector6d& Kel;

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
