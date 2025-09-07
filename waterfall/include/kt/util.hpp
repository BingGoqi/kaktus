#ifndef KAKTUS_UTIL_HPP
#define KAKTUS_UTIL_HPP
#include <cmath>
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#ifndef Real
#define Real float
#endif // !Real

#ifdef Real 
#define RealMax  3.0000138e38
#else
#define RealMax  1e304
#endif

#ifndef _CMATH_
#define fma(a,b,c) (a)*(b)+(c) 
#endif // _CMATH_

#define cIn const
#define cOut
#define cInOut

//#define ABS(a) ((a)>0?(a):(-a))
//#define MIN(a,b) (a)>(b)?(b):(a)
//#define MAX(a,b) (a)>(b)?(a):(b)
//#define ABS_MIN(a,b) (ABS(a)>ABS(b)?(b):(a))
//#define ABS2_MIN(a,b) ((a) < 0 && (b) < 0)?MAX(a,b):MIN(a,b)

namespace kaktus
{
	//类型
	typedef unsigned long u64;
	typedef signed long i64;
	typedef unsigned int u32;
	typedef signed int i32;
	typedef unsigned short u16;
	typedef signed short i16;
	typedef unsigned char u8;
	typedef signed char i8;
	//常数
	constexpr static double PI = 3.14159265358979323846;
	constexpr static double TAU = 2 * PI;
	constexpr static double G = 6.67e-11;//TODO
	constexpr static double Light = 299792458;
	constexpr static float zzero = -0x1.0da1558p-27;//ln(2.117)-3/4;
	constexpr static double invG = 1 / G;
	//编译器函数
	constexpr Real pow2(Real a) { return a * a; }
	constexpr Real pow3(Real a) { return a * a * a; }
	constexpr Real fma(Real a, Real b, Real c) { return a * b + c; }
	constexpr float sqrt_slow(float a) {
		Real x = a;
		for (int i = 0; i < 8; i++)
			x = (x + a / x) / 2; // 牛顿法迭代
		return x;
	}
	constexpr double sqrt_ss(double a) {
		double x = a;
		for (int i = 0; i < 8; i++)
			x = (x + a / x) / 2; // 牛顿法迭代
		return x;
	}
	constexpr Real hypot2(Real x, Real y) { return fma(x, x, y * y); }
	constexpr Real min(Real a, Real b) { return a > b ? b : a; }
	constexpr Real abs(Real a) { return a > 0 ? a : -a; }
	constexpr Real abs_min(Real a, Real b) { return abs(a) > abs(b) ? b : a; }
	constexpr Real abs2_min(Real a, Real b)
	{
		if(a < 0 && b < 0) return abs(a) > abs(b) ? b : a;
		return min(a, b);
	}
	constexpr Real max(Real a, Real b) { return a < b ? b : a; }
	constexpr Real inv(Real a) { return 1/a; }
	constexpr Real sing(Real a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }
	//常数2
	constexpr static double sqrt2_i_2 = sqrt_ss(2) / 2;


	template<typename T> union vec2t
	{
		T d[2];
		//std::valarray<T> valarray;
		struct { T x, y; };
		struct { T u, v; };
		constexpr vec2t<T>() { x = 0, y = 0; }
		constexpr vec2t<T>(cIn T a, cIn T b) { x = (T)a, y = (T)b; }
		constexpr vec2t<T>(cIn vec2t<T>& a) { x = a.x, y = a.y; }
		constexpr vec2t<T>(cIn vec2t<T>&& a) noexcept { *this = a; }
		//~vec2t<T>() {};
		//重载函数
		constexpr vec2t<T>& operator=(cIn vec2t<T>&& a) noexcept { x = a.x, y = a.y; return *this; }
		constexpr vec2t<T>& operator=(cIn vec2t<T>& a) { x = a.x, y = a.y; return *this; }//无构造运算
		constexpr vec2t<T>& operator+=(cIn vec2t<T>& a) { x += a.x, y += a.y; return *this; }
		constexpr vec2t<T>& operator-=(cIn vec2t<T>& a) { x -= a.x, y -= a.y; return *this; }
		constexpr vec2t<T>& operator*=(cIn vec2t<T>& a) { x *= a.x, y *= a.y; return *this; }
		constexpr vec2t<T>& operator/=(cIn vec2t<T>& a) { x /= a.x, y /= a.y; return *this; }
		constexpr vec2t<T>& operator*=(cIn T a) { x *= a, y *= a; return *this; }
		constexpr vec2t<T>& operator*(cIn T a) { x *= a, y *= a; return *this; }
		constexpr vec2t<T>& operator/=(cIn T a) { x /= a, y /= a; return *this; }
		constexpr T length2() { return hypot2(x, y); }
		constexpr T length() { return sqrt_slow(hypot2(x, y)); }
		constexpr vec2t<T>& normal() { Real l = length(); x /= l, y /= l; return *this; };
		constexpr vec2t<T>& mul(cIn T a) { x *= a, y *= a; return *this; }
	};
	typedef vec2t<Real> vec2;

	constexpr vec2 operator+(cIn vec2& a, cIn vec2& b) { return { a.x + b.x,a.y + b.y }; }//构造运算
	constexpr vec2 operator-(cIn vec2& a, cIn vec2& b) { return { a.x - b.x,a.y - b.y }; }
	constexpr vec2 operator-(cIn vec2& a) { return { -a.x ,-a.y }; }

	constexpr vec2 matmulXY(cIn vec2& xp, cIn vec2& yp, cIn vec2& a) { return { fma(a.x,xp.x,a.y * yp.x),fma(a.x,xp.y,a.y * yp.y) }; }

	template<typename T> union vec3t
	{
		T d[3];
		struct { T x, y, z; };
		struct { T u, v, w; };
		struct { T r, g, b; };
		constexpr vec3t<T>() { x = 0, y = 0; }
		constexpr vec3t<T>(cIn T a, cIn T b, cIn T c) { x = a, y = b, z = c; }
		constexpr vec3t<T>(cIn vec3t<T>& a) { x = a.x, y = a.y, z = a.z; }
		constexpr vec3t<T>(cIn vec3t<T>&& a) noexcept { *this = a; }
		constexpr vec3t<T>& operator=(cIn vec3t<T>&& a) noexcept { x = a.x, y = a.y, z = a.z; return *this; }
		constexpr vec3t<T>& operator=(cIn vec3t<T>& a) { x = a.x, y = a.y, z = a.z; return *this; }//无构造运算
	};

	typedef vec3t<Real> vec3;

	template<typename T> union vec4t
	{
		T d[4];
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T s, t, p, q; };
	};
	typedef vec4t<Real> vec4;

	//静态函数
	/*constexpr void neg(cIn vec2& a, cOut vec2& b) { b = { -a.x,-a.y }; }
	constexpr void neg(cInOut vec2& a) { a = -a; }
	constexpr void add(cInOut vec2& a, cIn vec2& b) { a += b; }
	constexpr void add(cIn vec2& a, cIn vec2& b, cOut vec2& c) { c = a + b; }
	constexpr void sub(cInOut vec2& a, cIn vec2& b) { a.x -= b.x; a.v -= b.v; }
	constexpr void sub(cIn vec2& a, cIn vec2& b, cOut vec2& c) { c = { a.x - b.x,a.v - b.v }; }
	*/
	//静态函数2
	constexpr Real length(cIn vec2& a) { return sqrt_slow(hypot2(a.x, a.y)); }
	constexpr Real length2(cIn vec2& a, cIn vec2& b) { return hypot2(a.x - b.x, a.y - b.y); }
	constexpr Real length(cIn vec2& a, cIn vec2& b) { return sqrt_slow(hypot2(a.x - b.x, a.y - b.y)); }
	constexpr Real det(cIn vec2& a, cIn vec2& b) { return fma(a.x, b.y, -a.y * b.x); };
	constexpr Real dotmul(cIn vec2& a, cIn vec2& b) { return fma(a.x, b.x, a.y * b.y); }
	constexpr Real n_cos(cIn vec2& a, cIn vec2& b) { return dotmul(a,b); }
	constexpr Real n_sin(cIn vec2& a, cIn vec2& b) { return det(a,b); }
	//constexpr Real height(cIn vec2& a, cIn vec2& b, cIn vec2& c){vec2 d1 = b - a, d2 = c - a;return det(d1, d2) / length(d1);}
	//非编译器函数
	const inline Real Rpow2(Real a) { return a * a; }
	const inline Real Rpow3(Real a) { return a * a * a; }
	const inline Real Rfma(Real a, Real b, Real c) { return Rfma(a, b, c); }
	const inline Real Rline(Real a, Real b, Real t) { return Rfma(b-a, t, a); }
	const inline Real RsqueFunc(Real a, Real b, Real c, Real t) { return Rfma(Rfma(a,t,b), t, c); }
};

#undef cIn
#undef cOut
#undef cInOut

#endif // !KAKTUS_UTIL_HPP

//int testFunc()
//{
//	constexpr kaktus::vec2 v1 = { 2.,3. }, v2 = { 3.,7. };
//	constexpr kaktus::vec2 v3 = -v1;
//	constexpr float a = v3.x;
//	return 0;
//}