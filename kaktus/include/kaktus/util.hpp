#ifndef KAKTUS_UTIL_HPP
#define KAKTUS_UTIL_HPP
#include <Eigen/Core>
#include <limits>
#define Real float
#define fReal float
#define dReal double
#define ddReal double
#define iTime float
#define xTime Real
namespace kaktus
{
	typedef unsigned long u64;
	typedef signed long i64;
	typedef unsigned int u32;
	typedef signed int i32;
	typedef unsigned short u16;
	typedef signed short i16;
	typedef unsigned char u8;
	typedef signed char i8;
	constexpr static double PI = 3.14159265358979323846;
	constexpr static double TAU = 2 * PI;
	constexpr static double G = 6.67e-11;//TODO
	constexpr static double Light = 299792458;
	constexpr static float zzero = -0x1.0da1558p-27;//ln(2.117)-3/4;
	constexpr static double invG = 1 / G;
	inline double pow2(double a) { return a * a; }
	inline double pow3(double a) { return a * a * a; }
	inline float pow2(float a) { return a * a; }
	inline float pow3(float a) { return a * a * a; }
	inline int pow2(int a) { return a * a; }
	inline int pow3(int a) { return a * a * a; }
};
namespace Eigen
{
	typedef Matrix< double, 6, 1 > Vector6d;
	typedef Matrix< double, 6, 6 > Matrix6d;
}
namespace kaktus
{
	template<typename T> bool ulp(T a, T b, int n)
	{
		const T m = std::min(std::fabs(a), std::fabs(b));
		const T exp = m < std::numeric_limits<T>::min()
			? std::numeric_limits<T>::min_exponent - 1
			: std::ilogb(m);
		return std::fabs(a - b) <= n * std::ldexp(std::numeric_limits<T>::epsilon(), exp);
	}
	typedef struct
	{
		double sin, cos;
	} SinCos;
	typedef struct
	{
		float sin, cos;
	} SinCosf;
	typedef std::string string;
	typedef struct kDouble
	{
		double a, b;
		kDouble(double a, double b)
		{
			this->a = a;
			this->b = b;
		}
		void format()
		{
			double c = a + b;
			b -= c - a;
			a = c;
		}
	}DDouble;
	template<typename T>
	class StepLinkList
	{

	private:
		struct sstNode
		{
			double step, start;
			int lenth;
			T* ptr;
			stNode* prv, next;
		};
		//link list temp_list<time,val>
	public:
		T& getNext(xTime t);
		T& getPrive(xTime t);
	};
}
#endif // !KAKTUS_UTIL_HPP