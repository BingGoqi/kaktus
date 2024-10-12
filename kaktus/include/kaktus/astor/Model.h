#ifndef KAKTUS_MODEL_H
#define KAKTUS_MODEL_H
#include "kaktus/util.h"
namespace kaktus::astor::model
{
	enum ModelType
	{//ÔÝ¶¨
		p2a,
		pt2a,
		v2a,
		pv2a
	};
	template<typename IN,typename OUT>
	class Model
	{
	public:
		virtual void getval(int type, const IN &in, OUT &out);
		virtual std::vector<int> getOutTypes();
	};
	//Àý×Ó£º
	class PointGravityModel :public Model<Eigen::Vector3d, Eigen::Vector3d>
	{
		void getval(int type, const Eigen::Vector3d& p, Eigen::Vector3d& f){}
	};
}
#endif // !KAKTUS_MODEL_H
