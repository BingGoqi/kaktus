#ifndef KAKTUS_INTERPOLATORS_H
#define KAKTUS_INTERPOLATORS_H
#include "kaktus/util.h"
#ifndef KAKTUS_INTERPOLATORS_ABOUT_TYPE
#define KAKTUS_INTERPOLATORS_ABOUT_TYPE fReal
#endif
#define IPA KAKTUS_INTERPOLATORS_ABOUT_TYPE
namespace kaktus::Interpolators//这波是每集代码风格都不一样
{
	struct mulCom
	{
		IPA a, b;
		struct mulCom()
		{
			a = b = 1.;
		}
		struct mulCom(IPA minval)
		{
			a = 1 / minval;
			b = minval;
		}
		mulCom operator *=(IPA y)
		{
			if (abs(y) <= abs(b))
			{
				a *= b;
				b = y;
			}
			else
			{
				a *= y;
			}
		}
		double operator /(IPA y)
		{
			if (abs(y) <= abs(b))
			{
				return (b / y) * a;
			}
			else
			{
				return (a / y) * b;
			}
		}
	};
	template<typename T>
	class mapList
	{
	private:
		std::shared_ptr<T> sptr;
		T* list;
		std::function<int(int)>& map;
	public:
		mapList(std::shared_ptr<T> pm,int start ,int lenth):sptr(pm), list(pm[start]) {}
		T& getVal(int index)
		{
			return *list[map(index)];
		}
		T operator[](int index)
		{
			return list[map(index)];
		}
	};
	template<int l,typename T>
	class LL
	{
	private:
		struct mulCom mcom;
		IPA minstep;
		IPA* clist;
		//int l, w;
		//init:clist[i] = in[i]/PI(inT[i] - inT[j],j!=i)//需优化展开
		//out: foreach(mcom*=(t - inT[i]))
		//out = sum(mcom / (t - inT[i]) * clist[i],i)
	public:
		IPA getval(iTime t,int i);
		void move(int step);
	};
	class Neville
	{

	};
	class Newton {

	};

}
#undef IPA
#endif // !KAKTUS_INTERPOLATORS_H