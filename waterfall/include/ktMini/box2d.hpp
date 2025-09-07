#pragma once
#include <array>
#include <bitset>
#include <stdio.h>
#include <algorithm>
#include<kt/util.hpp>

#define BOX_DEBUG_CONST 0

#if BOX_DEBUG_CONST
#define BOX_CONST constexpr
#define BOX_DEBUG
#else
#define BOX_CONST
#define BOX_DEBUG(...) printf(__VA_ARGS__)
#endif // BOX_DEBUG_CONST

using namespace kaktus;
namespace box
{
	struct ArrayView
	{
		int offset;
		int size;
		constexpr ArrayView(int offset, int size) :offset(offset), size(size) {}
		constexpr int Index(signed int in) const { return offset + ((size + (in % size)) % size); }
		constexpr void next(int size) { offset += this->size, this->size = size; }
	};
	
	typedef struct TouchGrop
	{
		int offset;//point
		int num;
		int psize;
		constexpr TouchGrop(int offset, int num, int size) :offset(offset), num(num), psize(size) {}
		TouchGrop(TouchGrop& perv,int num,int size) :num(num), psize(size)
		{
			this->offset = perv.offset + num * psize;
		}
	}TouchGrop;

	typedef struct aabb
	{
		vec2 X,Y;
		BOX_CONST aabb() { X = { RealMax,-RealMax }, Y = { RealMax,-RealMax }; }
		BOX_CONST aabb(vec2 x, vec2 y) :X(x), Y(y) {}
		BOX_CONST aabb(vec2 center, Real r) : X({ center.x - r,center.x + r }), Y({ center.y - r,center.y + r }) {}
		aabb(Real a, Real b, Real c, Real d) : X({ a,b }), Y({ c, d }) {}
		BOX_CONST void flush(vec2 v)
		{
			X.x = min(X.x, v.x), X.y = max(X.y, v.x), Y.x = min(Y.x, v.y), Y.y = max(Y.y, v.y);
		}
		BOX_CONST void clean()
		{
			X = { RealMax,-RealMax }, Y = { RealMax,-RealMax };
		}
		BOX_CONST bool notcross(aabb in)
		{
			return (in.X.x > X.y || in.X.y < X.x || in.Y.x > Y.y || in.Y.y < Y.x);
		}
		BOX_CONST bool cross(aabb in)
		{
			return not(in.X.x > X.y || in.X.y < X.x || in.Y.x > Y.y || in.Y.y < Y.x);
		}
	}aabb;
	constexpr vec2 height(const vec2& a, const vec2& b, const vec2& c)
	{
		vec2 d1 = b - a, d2 = c - a;
		Real l = dotmul(d1,d2),s = det(d1, d2);
		if(l<0 || l > d1.length2())
			return { min(length(a,c),length(b,c)) ,-s };
		return {s / length(d1),-s };
	}
	constexpr Real heightL(const vec2& a, const vec2& b, const vec2& c)
	{
		vec2 d1 = b - a, d2 = c - a;
		Real l = dotmul(d1, d2);
		if (l<0 || l > d1.length2())
			return min(length(a,c),length(b,c));
		return  det(d1, d2) / length(d1);
	}
	constexpr Real CCW(const vec2& a, const vec2& b, const vec2& c)
	{
		vec2 d1 = b - a, d2 = c - a;
		return det(d2,d1);
	}
	constexpr vec2 offsetin(const vec2& a, const vec2& b, const vec2& c,Real l)
	{
		vec2 d1 = a - b, d2 = c - b;
		d1.normal(), d2.normal();
		vec2 cp = (d1 + d2).normal();
		l /= det(d1, cp);
		return  b + cp * l;
	}
	/*constexpr auto InitAray1{ []() constexpr {
		std::array<vec2, 22> array{};
		for (int i = 0; i < 22; i++)
		{
			array[i] = { i,i };
		}
		return array;
		}()
	};*/
	constexpr static Real sqrt2_i_2r = sqrt_slow(2) / 2;
	constexpr static vec2 xp = { sqrt2_i_2r ,-sqrt2_i_2r }, yp = { sqrt2_i_2r ,sqrt2_i_2r };
	constexpr static TouchGrop tg[] = { {0,8,9},{72,8,7},{128,2,6},{140,8,7},{196,8,4} };
	constexpr static int A0Size = 9, B0Size = 7, C0Size = 6, D0Size = 7, E0Size = 4,BlockNum = 34;
	
	enum TouchID { A = 0, B = A + (A0Size * 8), C = B + (B0Size * 8), D = C + (C0Size * 2), E = D + (D0Size * 8), End = E + (E0Size * 8) };
	constexpr static int eboSize1 = 3 * (End - 2 * BlockNum);
	//A2E's_template
	constexpr static auto A0 = std::to_array<Real>({ 16.5964f,98.6132,27.6161,96.1111,38.2832,92.3818,48.4614,87.4728,58.0208,81.4469,36.6469,53.5913,29.1787,53.5913,17.2623,58.5272,11.9822,63.8073 });
	constexpr static auto B0 = std::to_array<Real>({ 16.1325,54.4053,27.063,49.8822,27.063,29.5586,18.6897,21.1853,8.7753,21.1853,1.7647,28.1959,1.7647,40.0375 });
	constexpr static auto C0 = std::to_array<Real>({ 0.8824,19.4206,8.0443,19.4206,19.4206,8.0433,19.4206,-8.0433,8.0433,-19.4206,0.8824,-19.4206 });
	constexpr static auto D0 = std::to_array<Real>({ -13.0738,99.1417,-4.3583,99.905,4.3905,99.9036,13.1056,99.1375,8.8387,66.9509,0.,75.7896,-8.8358,66.9537 });
	constexpr static auto E0 = std::to_array<Real>({ 0.,70.7982,13.7671,57.0312,0.,43.2641,-13.7671,57.0312 });
	constexpr static auto gbs = std::to_array<int>({ 0,9,18,27,36,45,54,63,72,79,86,93,100,107,114,121,128,134,140,147,154,161,168,175,182,189,196,200,204,208,212,216,220,224,228 });
	
	constexpr static int D1[] = {0,1,5,6};
	constexpr static int D2[] = {1,2,3,4,5};

	BOX_CONST static Real FingerR = 13.;
	BOX_CONST static int touchNum = 0;
	BOX_CONST static int mouseTest = 0;
	BOX_CONST static vec3 touchpoint[11] = {};
	BOX_CONST std::array<Real, BlockNum> R0 = {0};

	BOX_CONST auto InitAray1()
	{
		std::array<vec2, End> array;
		int index = 0, s = A0Size;
		for (int i = 0; i < s; i++)
		{
			array[index++] = { A0[2 * i],A0[2 * i + 1] };
		}
		for (int i = 1; i < 8; i++)
		{
			for (int j = 0; j < s; j++)
			{
				array[index++] = matmulXY(xp, yp, array[index - s]);
			}
		}//A
		s = B0Size;
		for (int i = 0; i < s; i++)
		{
			array[index++] = { B0[2 * i],B0[2 * i + 1] };
		}
		for (int i = 1; i < 8; i++)
		{
			for (int j = 0; j < s; j++)
			{
				array[index++] = matmulXY(xp, yp, array[index - s]);
			}
		}//B
		s = C0Size;
		for (int i = 0; i < s; i++)
		{
			array[index++] = { C0[2 * i],C0[2 * i + 1] };
		}//C
		for (int j = 0; j < s; j++)
		{
			array[index++] = matmulXY({ -1,0 }, { 0,-1 }, array[index - s]);
		}//C
		s = D0Size;
		for (int i = 0; i < s; i++)
		{
			array[index++] = { D0[2 * i],D0[2 * i + 1] };
		}
		for (int i = 1; i < 8; i++)
		{
			for (int j = 0; j < s; j++)
			{
				array[index++] = matmulXY(xp, yp, array[index - s]);
			}
		}//D
		s = E0Size;
		for (int i = 0; i < s; i++)
		{
			array[index++] = { E0[2 * i],E0[2 * i + 1] };
		}
		for (int i = 1; i < 8; i++)
		{
			for (int j = 0; j < s; j++)
			{
				array[index++] = matmulXY(xp, yp, array[index - s]);
			}
		}//E
		return array;
	}
	BOX_CONST static auto AllData = InitAray1();

	BOX_CONST auto ChangeR()
	{
		std::array<vec2, End> array;
		ArrayView view(0, 0);
		int gb = 0;
		for (int g = 0; g < 5; g++)
		{
			for (int b = 0; b < tg[g].num; b++)
			{
				view.next(tg[g].psize);
				for (int i = 0; i < tg[g].psize; i++)
				{
					array[view.Index(i)] = offsetin(AllData[view.Index(i - 1)], AllData[view.Index(i)], AllData[view.Index(i + 1)], R0[gb]);
				}
				gb++;
			}
		}
		return array;
	}
	BOX_CONST auto TAllData = AllData;

	BOX_CONST auto InitAABB()
	{
		std::array<aabb, BlockNum> array;
		aabb ab;
		int index = 0;
		int bi = 0;
		for (int g = 0; g < 5; g++)
		{
			for (int b = 0; b < tg[g].num; b++)
			{
				for (int i = 0; i < tg[g].psize; i++)
				{
					ab.flush(TAllData[index++]);
				}
				array[bi++] = aabb(ab);
				ab.clean();
			}
		}
		return array;
	}
	BOX_CONST static std::array<aabb, BlockNum> AABB = InitAABB();//----------------------

	auto ChangeR(int gb, float dl)
	{
		int start = gbs[gb], end = gbs[gb + 1], l = end - start;
		float r0 =R0[gb] + dl;
		r0 = min(5, r0);
		r0 = max(-1.7, r0);
		//printf("%d:%f->%f = %f\n", gb, R0[gb], R0[gb] + dl, r0);
		R0[gb] = r0;
		ArrayView view(start, l);
		aabb ab;
		for (int i = 0; i < l; i++)
		{
			TAllData[view.Index(i)] = offsetin(AllData[view.Index(i - 1)], AllData[view.Index(i)], AllData[view.Index(i + 1)], R0[gb]);
			ab.flush(TAllData[view.Index(i)]);
		}
		AABB[gb] = ab;
	}

	BOX_CONST static int testpoint(const vec2& in,Real r)
	{
		int gb;
		Real min;
		//std::bitset<BlockNum> bm;
		aabb ab(in, r);
		gb = -1;
		for (int g = 0; g < 5; g++)
		{
			for (int b = 0; b < tg[g].num; b++)
			{
				gb++;//block Index
				if (ab.notcross(AABB[gb]))
					continue;
				int offset = tg[g].offset + tg[g].psize * b;
				min = RealMax;
				if (g == 3)
				{
					Real min1 = min, min2 = min;
					for (int i = 0; i < 4; i++)
					{
						Real lh = heightL(TAllData[offset + D1[i]], TAllData[offset + D1[(i + 1) % 4]], in);
						min1 = abs_min(min1, lh);
					}
					for (int i = 0; i < 5; i++)
					{
						Real lh = heightL(TAllData[offset + D2[i]], TAllData[offset + D2[(i + 1) % 5]], in);
						min2 = abs_min(min2, lh);
					}
					min = kaktus::min(min1, min2);
				}
				else
				{
					for (int i = 0; i < tg[g].psize; i++)
					{
						Real lh = heightL(TAllData[offset + i], TAllData[offset + ((i + 1) % tg[g].psize)], in);
						min = abs_min(min, lh);
					}
				}
				if (min < r)
				{
					//BOX_DEBUG("%d->%c%d=%f\n", 0, g + 'A', b + 1, min);
					return gb + 1;
				}	
			}
		}
		return 0;
	}
	BOX_CONST auto loop()
	{
		int gb;
		Real min,fr;
		vec2 tp;
		std::array<int, BlockNum> bm{};
		for (int in = 1 - mouseTest; in < touchNum + 1; in++)
		{
			tp = { touchpoint[in].x, touchpoint[in].y };
			fr = touchpoint[in].z;
			aabb ab(tp, fr);
			gb = -1;
			for (int g = 0; g < 5; g++)
			{
				for (int b = 0; b < tg[g].num; b++)
				{
					gb++;//block Index
					if (bm[gb] != 0)
						continue;
					if (ab.notcross(AABB[gb]))
						continue;
					int offset = tg[g].offset + tg[g].psize * b;
					min = RealMax;
					if (g == 3)
					{
						Real min1 = min, min2 = min;
						for (int i = 0; i < 4; i++)
						{
							Real lh = heightL(TAllData[offset + D1[i]], TAllData[offset + D1[(i + 1) % 4]], tp);
							min1 = abs_min(min1, lh);
						}
						for (int i = 0; i < 5; i++)
						{
							Real lh = heightL(TAllData[offset + D2[i]], TAllData[offset + D2[(i + 1) % 5]], tp);
							min2 = abs_min(min2, lh);
						}
						min = kaktus::min(min1, min2);
					}
					else
					{
						for (int i = 0; i < tg[g].psize; i++)
						{
							Real lh = heightL(TAllData[offset + i], TAllData[offset + ((i + 1) % tg[g].psize)], tp);
							min = abs_min(min, lh);
						}
					}
					if (min < fr)
					{
						//BOX_DEBUG("%d->%c%d=%f\n", i + 1, g + 'A', b + 1, min);
						bm[gb] = in + 1;
					}
				}
			}
		}
		return bm;
	}
	//BOX_CONST static auto bm = loop();

	BOX_CONST auto EBO1()
	{
		std::array<int, eboSize1> array;
		int pi, pk, tnum = 0;
		ArrayView view(0, 0);
		for (int g = 0; g < 5; g++)
		{
			for (int b = 0; b < tg[g].num; b++)
			{
				pk = 0;
				if (g == 0) pk = 6;
				if (g == 3) pk = 5;
				view.next(tg[g].psize);
				pi = view.Index(pk);
				for (int i = 0; i < tg[g].psize - 2; i++)
				{
					array[tnum++] = pi;
					array[tnum++] = view.Index(pk + i + 1);
					array[tnum++] = view.Index(pk + i + 2);
				}
			}
		}
		return array;
	}
	BOX_CONST static auto ebo1 = EBO1();

	BOX_CONST auto EBO2()
	{
		std::array<int, End * 2> array;
		int tnum = 0;
		ArrayView view(0, 0);
		for (int g = 0; g < 5; g++)
		{
			for (int b = 0; b < tg[g].num; b++)
			{
				view.next(tg[g].psize);
				for (int i = 0; i < tg[g].psize; i++)
				{
					array[tnum++] = view.Index(i);
					array[tnum++] = view.Index(i+1);
				}
			}
		}
		return array;
	}
	BOX_CONST static auto ebo2 = EBO2();
}

//constexpr int getModIndex(cIn int size, cIn int val) { return (size + val % size) % size; }//TODO tqk
int TestFunc()
{
	for (auto v : box::AllData)
		printf("(%f,%f),",v.x, v.y);
	return 0;
}
#undef BOX_CONST
#undef BOX_DEBUG
#undef Real