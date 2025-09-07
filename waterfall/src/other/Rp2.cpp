#include <cstdio>
#include <array>
#include <set>
#define TCONST
#define UR4 0

namespace rp
{
	typedef struct Rp
	{
		float r;
		int type;
		int8_t a, b, c , d;
		bool operator <(const Rp& rp)const
		{
			return r != rp.r ? r < rp.r : type < rp.type;
		}
	} Rp;
	static constexpr float Br(float a, float b)
	{
		return a * b / (a + b);
	}
	static constexpr float Br(float a, float b, float c)
	{
		float bc = b * c;
		return a * bc / (a * (b + c) + bc);
	}
	static constexpr float Br(float a, float b, float c,float d)
	{
		float bcd = b * c * d;
		return a * bcd / (a * (b * (c + d) + c * d) + bcd);
	}
	static constexpr float Brt(float a, float b, float c, float d)
	{
		return 1 / (1 / a + 1 / b + 1 / c + 1 / d);
	}
	static std::set<rp::Rp> Rset;
	static constexpr int s = 12, Rmin = -12, Rmax = 12, len = Rmax - Rmin + 1;
	static constexpr float Rpmin = 0.519, Rpmax = 0.521;
	static TCONST auto InitArray1()
	{
		double q = pow(10, 1. / s);
		double r;
		std::array<float, len> array;
		for (int i = Rmin; i <= Rmax; i++)
		{
			r = pow(q, i);
			array[i - Rmin] =(float)( i > 0 ? round(pow(q, i) * 10) * 0.1 : round(pow(q, i) * 100) * 0.01);
		}
		return array;
	}
	static TCONST std::array<float, len> Rarray = std::to_array<float>({ 0.1,0.12,0.15,0.18,0.22,0.27,0.33,0.39,0.47,0.56,0.68,0.82,1,1.2,1.5,1.8,2.2,2.7,3.3,3.9,4.7,5.6,6.8,8.2,10 });
	float getRi(int i) { return Rarray[i - Rmin]; }
	void addR(float r,int type,int a,int b,int c,int d)
	{
		if (r < Rpmin || r > Rpmax)
			return;
		Rset.emplace(r, type, a, b, c, d);
	}
	void run()
	{
		float a, b, c, d;
		float tmin, tmax;
		for (int i = Rmin; i <= Rmax; i++)
		{
			a = Rarray[i - Rmin];
			addR(a, 01, i, 0, 0, 0);
			for (int j = Rmin; j <= Rmax; j++)
			{
				b = Rarray[j - Rmin];
				addR(a + b, 21, i, j, 0, 0);
				addR(Br(a, b), 22, i, j, 0, 0);
				for (int k = Rmin; k <= Rmax; k++)
				{
					c = Rarray[k - Rmin];
					addR(a + b + c, 31, i, j, k, 0);
					addR(Br(a, b, c), 32, i, j, k, 0);
					addR(Br(a, b) + c, 33, i, j, k, 0);
					addR(Br(a + b, c), 34, i, j, k, 0);
					for (int l = Rmin; l <= Rmax; l++)
					{
						if (!UR4)
							break;
						d = Rarray[l - Rmin];
						tmax = a + b + c + d;
						tmin = Br(a, b, c, d);
						if (tmin > Rpmax || tmax < Rpmin)
							continue;
						addR(tmax, 41, i, j, k, l);
						addR(a + b + Br(c, d), 42, i, j, k, l);
						addR(Br(a + b, c + d), 43, i, j, k, l);
						addR(tmin, 44, i, j, k, l);
						addR(Br(a + b, c, d), 45, i, j, k, l);
						addR(Br(a + b, c) + d, 46, i, j, k, l);
						addR(Br(a, b) + Br(c, d), 47, i, j, k, l);
						addR(Br(a + b, c) + d, 48, i, j, k, l);
						addR(Br(a, b, c) + d, 49, i, j, k, l);
						addR(Br(Br(a, b) + c, d), 410, i, j, k, l);
						addR(Br(a + b + c, d), 411, i, j, k, l);
					}
				}
			}
			printf("Rp:\t%d\n", i);
		}
	}
}//0.520000

int s1()
{
	FILE* fout;
	fopen_s(&fout, "Rp2_0p52_out.csv", "w");
	rp::run();
	auto it = rp::Rset.begin();
	while (it != rp::Rset.end())
	{
		fprintf(fout, "%f,%d,%d,%d,%d,%d\n", it->r, it->type, it->a, it->b, it->c,it->d);
		it++;
	}
	return 0;
}
int s2()
{
	float a = rp::getRi(0),
		b = rp::getRi(2),
		c = rp::getRi(7);
	printf("%f,%f,%f\n", a, b, c);
	printf("%f", rp::Br(a,b,c));
	return 0;
}
int main()
{
	return s1();
}