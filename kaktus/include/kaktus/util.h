#ifndef KAKTUS_UTIL_H
#define KAKTUS_UTIL_H
#include <Eigen/Core>
constexpr static double PI = 3.14159265358979323846;
constexpr static double TAU = 2 * PI;
constexpr static double G = 6.67e-11;
constexpr static double Light = 299792458;
constexpr static double invG = 1/G;
double inline pow2(double a) { return a * a; }
double inline pow3(double a) { return a * a * a; }
namespace Eigen
{
	typedef Matrix< double, 6, 1 > Vector6d;
	typedef Matrix< double, 6, 6 > Matrix6d;
}
namespace kaktus
{
	typedef struct
	{
		double sin, cos;
	} SinCos;
	class DataBase {};
	typedef struct
	{
		double x, y, z, vx, vy, vz, t;
	};
	typedef std::string string;
}
#endif // !KAKTUS_UTIL_H