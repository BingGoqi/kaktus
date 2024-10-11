#pragma once
#include <tudat/basics/basicTypedefs.h>
#ifndef KAKTUS_UTIL_H
constexpr static double PI = 3.14159265358979323846;
constexpr static double TAU = 2 * PI;
double inline pow2(double a) { return a * a; }
double inline pow3(double a) { return a * a * a; }
#define KAKTUS_UTIL_H
#endif // !KAKTUS_UTIL_H