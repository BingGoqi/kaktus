#include "kaktus/util.h"
#ifndef KAKTUS_EPHEMERIS_H
#define KAKTUS_EPHEMERIS_H
namespace kaktus
{
	namespace astor
	{
		class Ephemeris
		{
		private:
			double star, end;
			
		public:
			void getPosition(Eigen::Vector3d& p, double time);
			void getVelocity(Eigen::Vector3d& v, double time);
		};
	}
}
#endif // !KAKTUS_EPHEMERIS_H
