#ifndef KAKTUS_ITRF_H
#define KAKTUS_ITRF_H
#include "kaktus/astor/Model.h"
namespace kaktus::astor::model
{
	class Itrf
	{
	private:
		Eigen::Matrix3d mat,imat;
		bool vp;
		//std::function<void(double,Eigen::Vector2d&)> f;
		void setNPlot(const Eigen::Vector3d& np, bool vp);
		void S2I(Eigen::Vector3d& s);
		void I2S(Eigen::Vector3d& i);
	public:
	};
}

#endif // !KAKTUS_ITRF_H
/*

*/