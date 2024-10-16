#ifndef KAKTUS_MODEL_H
#define KAKTUS_MODEL_H
#include "kaktus/util.h"
namespace kaktus::astor::model
{
	enum ModelType
	{//暂定
		p2a,
		pt2a,
		v2a,
		pv2a
	};
	template<typename IN,typename OUT>
	class Model
	{
	private:
		int type = 0;
	public:
		virtual void getval(int type, const IN &in, OUT &out) = 0;
		virtual int getType() { return type; };
	};
	//例子：
	class PointGravityModel :public Model<Eigen::Vector3d, Eigen::Vector3d>
	{
	private:
		double mu;
	public:
		void getval(int type, const Eigen::Vector3d& p, Eigen::Vector3d& f){}
	};
	class PlanteRotate:public Model<double,Eigen::Matrix6d>
	{
	private:
	public:
	};
}
#endif // !KAKTUS_MODEL_H
/*
enum AvailableAcceleration
{
	undefined_acceleration,//无
	point_mass_gravity,//质点
	aerodynamic,//大气
	cannon_ball_radiation_pressure,// 球形辐射
	spherical_harmonic_gravity,//球谐
	mutual_spherical_harmonic_gravity,//互球谐
	polyhedron_gravity,//多面体
	ring_gravity,//环形
	third_body_point_mass_gravity,//非惯性力
	third_body_spherical_harmonic_gravity,
	third_body_mutual_spherical_harmonic_gravity,
	third_body_polyhedron_gravity,
	third_body_ring_gravity,
	thrust_acceleration,//发动机
	relativistic_correction_acceleration,//相对论矫正
	empirical_acceleration,//经验
	direct_tidal_dissipation_in_central_body_acceleration,//潮汐
	direct_tidal_dissipation_in_orbiting_body_acceleration,
	radiation_pressure,//辐射
	momentum_wheel_desaturation_acceleration,//动量轮去饱和加速度
	custom_acceleration,//自定义
	yarkovsky_acceleration
};
*/