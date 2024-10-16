#ifndef KAKTUS_BODY_H
#define KAKTUS_BODY_H
#include "kaktus/astor/Ephemeris.h"
#include "kaktus/astor/Model.h"
namespace kaktus::astor
{

	class Body 
	{
	private:
		const string name;
		//int BodyType;
		//getframe(pva,subframe)
		std::unordered_map<int, std::vector<model::Model<DataBase,DataBase>>> modelListMap;
		Ephemeris &eph;
	public:
		Body(string name, Ephemeris& eph)
			:name(name), eph(eph) {};
		void getStateToBody(Body& b, double time, Eigen::Vector6d& state);//to的视角
		double getStateFromBody(Body& b, double time, Eigen::Vector6d& state);//this的视角
		string& getSup();
		//void getForce(Eigen::Vector3d& f, const Eigen::Vector3d& p, const Eigen::Vector3d& v, double time);
	};
}
#endif // !KAKTUS_BODY_H