#include "kaktus/astor/Ephemeris.h"
#include <cmath>
#ifndef KAKTUS_BODY_H
#define KAKTUS_BODY_H
namespace kaktus
{
	namespace astor
	{
		class Body
		{
		private:
			const double RU, TU, MU, VU, FU;
			int BodyType;
			//getframe(pva,subframe)
			Ephemeris eph;
		public:
			Body(double MU, double DU) :MU(MU), RU(RU), TU(sqrt(pow3(RU) / invG)), VU(RU / TU), FU(VU / TU)
			{

			}
			void getPosition(Eigen::Vector3d &p, double time);
			void getVelocity(Eigen::Vector3d &v, double time);
			//void getForce(Eigen::Vector3d& f, const Eigen::Vector3d& p, const Eigen::Vector3d& v, double time);
		};
	}
}
#endif // !KAKTUS_BODY_H