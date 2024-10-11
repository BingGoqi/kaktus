#include "kaktus/astor/Ephemeris.h"
#include <cmath>
#include <tudat/astro/basic_astro/orbitalElementConversions.h>
//#include <tudat/astro/ephemerides/ephemeris.h>
namespace kaktus
{
	namespace astor
	{
		class AprxEphemeris :public Ephemeris
		{
		private:
			const double AU, TU, Gm;
			double pt;
			Eigen::Matrix6d sl;
			Eigen::Vector6d nke, cs;
			Eigen::Quaterniond rotationFromOrbitalPlane_;
			//fma(v1, v2, t) + v3 * sin(fma(v4, v5, t));
			//1/sec
		public:
			AprxEphemeris(double AU, double Gm) :AU(AU), Gm(Gm), TU(sqrt(pow3(AU)/Gm)) {
				pt = 0;
				sl = Eigen::Matrix6d::Zero();
				nke = Eigen::Vector6d::Zero();
				cs = Eigen::Vector6d::Zero();
				rotationFromOrbitalPlane_ = Eigen::Quaterniond::Identity();
			};
			void setAprx(int type, const Eigen::Vector6d &val)
			{
				sl.row(type) = val;
			}
			void setAprx(int type, int index, double val)
			{
				sl(type, index) = val;
			}
			void flushKeplerElements(double time)
			{
				time /= TU;
				if (abs(time - pt) < 1e-3)//过滤
				{
					return;
				}
				pt = time;
				for (int i = 0; i < 6; i++)
				{
					nke[i] = 0;
					switch ((int)sl(i,0)&3)
					{
					case 0:
						nke[i] = sl(i,1); break;
					case 1:
						nke[i] = fma(sl(i, 2), time, sl(i, 1)); break;
					case 3:
						nke[i] = fma(sl(i, 2), time, sl(i, 1));
					case 2:
						nke[i] += sl(i, 3) *sin(fma(sl(i, 4),time,sl(i, 5))); break;
					}
				}
				nke(0) *= AU;
				nke(5) = fmod(nke(5), TAU);
				nke(1) = fmod(nke(1), 1);
				nke(5) = tudat::orbital_element_conversions::convertMeanAnomalyToTrueAnomaly(nke(1), nke(5));

				rotationFromOrbitalPlane_ =
					Eigen::AngleAxisd(nke(4), Eigen::Vector3d::UnitZ()) *
					Eigen::AngleAxisd(nke(2), Eigen::Vector3d::UnitX()) *
					Eigen::AngleAxisd(nke(3), Eigen::Vector3d::UnitZ());
			}
			void getCartesianState(double time, Eigen::Vector6d &pv)
			{//改写成相邻时域PCHIP插值
				flushKeplerElements(time);
				const double eccentricity_ = nke(1);
				double cosineOfTrueAnomaly, sineOfTrueAnomaly;
				{
					const double meanAnomalyChange =
						tudat::orbital_element_conversions::convertElapsedTimeToEllipticalMeanAnomalyChange(time, Gm, nke(0));
					const double eccentricAnomaly =
						tudat::orbital_element_conversions::convertMeanAnomalyToEccentricAnomaly(eccentricity_, meanAnomalyChange);
					const double trueAnomaly =
						tudat::orbital_element_conversions::convertEccentricAnomalyToTrueAnomaly(eccentricAnomaly, eccentricity_);
					cosineOfTrueAnomaly = cos(trueAnomaly), sineOfTrueAnomaly = sin(trueAnomaly);
				}
				double semiLatusRectum_ = nke(0) * (1 - pow2(eccentricity_));

				// Definition of position in the perifocal coordinate system.
				pv(0) = semiLatusRectum_ * cosineOfTrueAnomaly
					/ (1.0 + eccentricity_ * cosineOfTrueAnomaly);
				pv(1) = semiLatusRectum_ * sineOfTrueAnomaly
					/ (1.0 + eccentricity_ * cosineOfTrueAnomaly);

				// Definition of velocity in the perifocal coordinate system.
				pv(3) =
					-sqrt(Gm / semiLatusRectum_) * sineOfTrueAnomaly;
				pv(4) =
					sqrt(Gm / semiLatusRectum_)
					* (eccentricity_ + cosineOfTrueAnomaly);
				// Rotate orbital plane to correct orientation.
				pv.segment(0, 3) = rotationFromOrbitalPlane_ *
					pv.segment(0, 3);
				pv.segment(3, 3) = rotationFromOrbitalPlane_ *
					pv.segment(3, 3);
			}
			Eigen::Vector6d getCartesianState(double time)
			{//改写成相邻时域PCHIP插值
				Eigen::Vector6d currentCartesianState = Eigen::Vector6d::Zero();
				getCartesianState(time, currentCartesianState);
				return currentCartesianState;
			}
		};
	}
}