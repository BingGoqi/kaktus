#ifndef KAKTUS_MODEL_H
#define KAKTUS_MODEL_H
#include "kaktus/util.h"
namespace kaktus::astor::model
{
	enum ModelType
	{//�ݶ�
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
	//���ӣ�
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
	undefined_acceleration,//��
	point_mass_gravity,//�ʵ�
	aerodynamic,//����
	cannon_ball_radiation_pressure,// ���η���
	spherical_harmonic_gravity,//��г
	mutual_spherical_harmonic_gravity,//����г
	polyhedron_gravity,//������
	ring_gravity,//����
	third_body_point_mass_gravity,//�ǹ�����
	third_body_spherical_harmonic_gravity,
	third_body_mutual_spherical_harmonic_gravity,
	third_body_polyhedron_gravity,
	third_body_ring_gravity,
	thrust_acceleration,//������
	relativistic_correction_acceleration,//����۽���
	empirical_acceleration,//����
	direct_tidal_dissipation_in_central_body_acceleration,//��ϫ
	direct_tidal_dissipation_in_orbiting_body_acceleration,
	radiation_pressure,//����
	momentum_wheel_desaturation_acceleration,//������ȥ���ͼ��ٶ�
	custom_acceleration,//�Զ���
	yarkovsky_acceleration
};
*/