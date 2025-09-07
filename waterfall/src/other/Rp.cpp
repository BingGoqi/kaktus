#include <cstdio>
#include <array>
#include <set>
#define TCONST
namespace rp
{
	typedef struct Rp
	{
		float r;
		int8_t type, a, b, c;
		bool operator <(const Rp& rp)const
		{
			return r < rp.r;
		}
	} Rp;
	static constexpr float Br(float a, float b)
	{
		return a * b / (a + b);
	}
	static constexpr float Br(float a, float b,float c)
	{
		float bc = b * c;
		return a * bc / (a * (b + c) + bc);
	}
	static constexpr float B1(float a)
	{
		return a / (1. + a);
	}
	static constexpr float B1(float a, float b)
	{
		return a * b / (a * (b + 1) + b);
	}
	static constexpr float B1(float a, float b, float c)
	{
		float bc = b * c;
		return a * bc / (a * (b + c) + bc + a * bc);
	}
	static std::set<rp::Rp> Rset;
	static constexpr int s = 12, Rmin = -12, Rmax = 12,len=Rmax-Rmin+1;
	static TCONST auto InitArray1()
	{
		double q = pow(10, 1. / s);
		double r;
		std::array<float, len> array;
		for (int i = Rmin; i <= Rmax; i++)
		{
			r = pow(q, i);
			array[i - Rmin] = i>0?round(pow(q, i)*10)*0.1:round(pow(q, i) * 100) * 0.01;
		}
		return array;
	}
	static TCONST std::array<float, len> Rarray = std::to_array<float>({ 0.1,0.12,0.15,0.18,0.22,0.27,0.33,0.39,0.47,0.56,0.68,0.82,1,1.2,1.5,1.8,2.2,2.7,3.3,3.9,4.7,5.6,6.8,8.2,10});
	void getR1()
	{
		for (int i = 0; i < len; i++)
		{
			Rset.emplace(Rarray[i], 01, i, 0, 0);
		}
	}
	void getR2()
	{
		float r;
		for (int i = 0; i < len; i++)
		{
			r = Rarray[i] + 1.;
			Rset.emplace(r, 21, i, 0, 0);
			Rset.emplace(1./r, 22, i, 0, 0);
		}
	}
	void getR3()
	{
		//1+a+b 31
		//1||a||b 32
		//(1+a)||b-->(1+a)b/(1+a+b) 33
		//1+a||b-->1+ab/(a+b) 34
		//rt
		float r,a,b;
		for (int i = 0; i < len; i++)
		{
			a = Rarray[i];
			for (int j = 0; j < len; j++)
			{
				b = Rarray[j];
				r = 1. + a + b;
				Rset.emplace(r, 31, i, j, 0);
				r = 1. / r;
				Rset.emplace(B1(a,b), 32, i, j, 0);
				Rset.emplace((1+a) * b * r, 33, i, j, 0);
				Rset.emplace((1+a*b)/(a+b), 34, i, j, 0);
			}
		}
	}
	void getR4()
	{
		//1+a+b+c 41
		//1+a+(b||c) 42
		//(1+a)||(b+c) 43
		//1||a||b||c 44
		//(1+a)||b||c 45
		//(1+a)||b+c 46
		//1||a+b+c 47
		//((1+a)||b)+c 48
		//1||a||b+c 49
		//(1||a+b)||c 50
		//(1+a+b)||c 51
		//rt

		float a, b,c;
		for (int i = 0; i < len; i++)
		{
			a = Rarray[i];
			for (int j = 0; j < len; j++)
			{
				b = Rarray[j];
				for (int k = 0; k < len; k++)
				{
					c = Rarray[k];
					Rset.emplace(1. + a + b + c, 41, i, j, k);
					Rset.emplace(1 + a + Br(b, c), 42, i, j, k);
					Rset.emplace(Br(1+a,b+c), 43, i, j, k);
					Rset.emplace(B1(a,b,c), 44, i, j, k);
					Rset.emplace(Br(1+a,b,c), 45, i, j, k);
					Rset.emplace(Br(1+a,b)+c, 46, i, j, k);
					Rset.emplace(B1(a)+b+c, 47, i, j, k);
					Rset.emplace(Br(1+a,b)+c, 48, i, j, k);
					Rset.emplace(B1(a,b)+c, 49, i, j, k);
					Rset.emplace(Br(Br(1,a)+b,c), 50, i, j, k);
					Rset.emplace(Br(1+a+b,c), 51, i, j, k);
				}
			}
			printf("R4:\t%d\n",i);
		}
	}
	void run()
	{
		float a, b, c, d;
		for (int i = 0; i < len; i++)
		{
			a = Rarray[i];
			Rset.emplace(a, 01, i, 0, 0);
			for (int j = 0; j < len; j++)
			{
				b = Rarray[j];
				Rset.emplace(a + b, 21, i, j, 0);
				Rset.emplace(Br(a, b), 22, i, j, 0);
				for (int k = 0; k < len; k++)
				{
					c = Rarray[k];
					for (int l = 0; l < len; l++)
					{
						d = Rarray[l];

					}
				}
			}
		}
	}
}

int s1()
{
	FILE* fout;
	fopen_s(&fout, "Rp_out.csv", "w");
	//rp::Rarray = rp::InitArray1();
	rp::getR1();
	printf("R1_ok");
	rp::getR2();
	printf("R2_ok");
	rp::getR3();
	printf("R3_ok");
	rp::getR4();
	auto it = rp::Rset.begin();
	while (it != rp::Rset.end())
	{
		fprintf(fout, "%f,%d,%d,%d,%d\n", it->r, it->type, it->a + rp::Rmin, it->b + rp::Rmin, it->c + rp::Rmin);
		it++;
	}
	return 0;
}
int s2()
{
	float a = rp::Rarray[1 - rp::Rmin],
		b = rp::Rarray[5 - rp::Rmin],
		c = rp::Rarray[2 - rp::Rmin];
	float f = rp::Br(rp::Br(1, a+b), c);
	return 0;
}
int main()
{
	return s2();
}
//1+a 21
//1||a 22
// 
//1+a+b 31
//1||a||b 32
//(1+a)||b-->(1+a)b/(1+a+b) 33
//1+a||b-->1+ab/(a+b) 34
// 
//1+a+b+c 41
//1+a+(b||c) 42
//(1+a)||(b+c) 43
//1||a||b||c 44
//(1+a)||b||c 45
//(1+a)||b+c 46
//1||a+b+c 47
//((1+a)||b)+c 48
//1||a||b+c 49
//(1||a+b)||c 50
//(1+a+b)||c 51