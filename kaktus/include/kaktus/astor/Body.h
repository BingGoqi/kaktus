#ifndef KAKTUS_BODY_H
#define KAKTUS_BODY_H
#include "kaktus/astor/Ephemeris.h"
#include "kaktus/astor/Model.h"
#include <cmath>
#include <string>
namespace kaktus::astor
{
	class Body
	{
	private:
		const double RU, TU, MU, VU, FU;
		const std::string name;
		//int BodyType;
		//getframe(pva,subframe)
		std::unordered_map<int, std::vector<std::shared_ptr<model::Model<DataBase,DataBase>>>> modelListMap;
		Ephemeris eph;
	public:
		Body(std::string name, double MU, double DU)
			:MU(MU), RU(RU), TU(sqrt(pow3(RU) / invG)), VU(RU / TU), FU(VU / TU),name(name) {}
		Body(std::string name) :MU(1), RU(1), TU(1), VU(1), FU(1),name(name) {}
		void setEphemeris(){}
		void getPosition(Eigen::Vector3d& p, double time)
		{
			eph.getPosition(p, time);
		}
		void getVelocity(Eigen::Vector3d& v, double time) {
			eph.getVelocity(v, time);
		}
		void getPV(Eigen::Vector6d& pv, double time)
		{
			eph.getPV(pv, time);
		}
		//void getForce(Eigen::Vector3d& f, const Eigen::Vector3d& p, const Eigen::Vector3d& v, double time);
	};
}
#endif // !KAKTUS_BODY_H