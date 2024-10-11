#pragma once
#include <Eigen/Core>
#ifndef KAKTUS_UTIL_H
constexpr static double PI = 3.14159265358979323846;
constexpr static double TAU = 2 * PI;
constexpr static double G = 6.67e-11;
constexpr static double invG = 1.49925e10;
double inline pow2(double a) { return a * a; }
double inline pow3(double a) { return a * a * a; }
namespace Eigen
{
	typedef Matrix< double, 6, 1 > Vector6d;
	typedef Matrix< double, 6, 6 > Matrix6d;
}
#define KAKTUS_UTIL_H
#endif // !KAKTUS_UTIL_H