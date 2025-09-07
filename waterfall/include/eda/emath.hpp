#pragma once
#include <kt/util.hpp>
#include <stdio.h>
namespace eda
{
	typedef kaktus::vec2t<float> vec2f;
	typedef struct Npoint
	{
		kaktus::vec2t<float> P, B;
		float m;
	};
	float const inline sqf(float a, float b, float c, float x)
	{
		return fma(fma(a, x, b), x, c);
	}
	void static getPoint(vec2f& A, vec2f& B, vec2f& C, float w, vec2f& out, float t)
	{
		float c = t * t, b = 2 * t, a = 1 + c - b;
		b = -fma(2, c, -b);
		b *= w;

		float ti = 1 / (a + b + c);
		out.x = fma(A.x, a, fma(B.x, b, C.x * c)) * ti;
		out.y = fma(A.y, a, fma(B.y, b, C.y * c)) * ti;
	}
}
int testFunc()
{
	eda::vec2f A(0, 1), B(1, 1), C(1, 0);
	eda::vec2f P(0, 0);
	for (float t = 0; t < 1; t += 0.01)
	{
		eda::getPoint(A, B, C, sqrt(2)/2, P, t);
		printf("(%f,%f),", P.x, P.y);
	}
	return 0;
}