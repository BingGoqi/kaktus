#include <stdio.h>
#include <math.h> //需优化掉

#define MaxOrder 40
typedef struct a
{
	char index;
	char using_order;
	char offset;
	char by;
	double *coeff; //[order+1]
} Taylor_Series_1;

inline static Taylor_Series_1 creat_TS1(int index, double *data)
{
	Taylor_Series_1 temp;
	temp.index = index;
	temp.coeff = data;
	return temp;
}
inline static void set_TS1(Taylor_Series_1 *ts, int index, double *data)
{
	ts->index = index;
	ts->coeff = data;
}
inline static void set_coeff(Taylor_Series_1 *f, double val, int order)
{
	f->coeff[order] = val;
}
inline static double get_coeff(Taylor_Series_1 *f, int order)
{
	return f->coeff[order];
}
//--------https://bluescarni.github.io/heyoka/ad_notes.html
inline static double add(Taylor_Series_1 *f, Taylor_Series_1 *g, int order)
{
	return get_coeff(f, order) + get_coeff(g, order);
}
inline static double sub(Taylor_Series_1 *f, Taylor_Series_1 *g, int order)
{
	return get_coeff(f, order) - get_coeff(g, order);
}
inline static double neg(Taylor_Series_1 *f, int order)
{
	return -get_coeff(f, order);
}
inline static double diff(Taylor_Series_1 *f, int order)
{
	return get_coeff(f, order + 1) * (order + 1);
}
inline static double Int(Taylor_Series_1 *f, int order)
{
	return get_coeff(f, order - 1) / order;
}
//--------------------TODO:待优化
inline static double crossmul(Taylor_Series_1 *f, Taylor_Series_1 *g, int k, int j)
{
	return get_coeff(f, k - j) * get_coeff(g, j);
}
double static crossaddmul(Taylor_Series_1 *f, Taylor_Series_1 *g, int start, int end)
{
	double temp = 0.;
	for (int i = start; i <= end; i++)
	{
		temp += crossmul(f, g, end, i); // TODO:SIMD
	}
	return temp;
}
inline static double mul(Taylor_Series_1 *f, Taylor_Series_1 *g, int order)
{
	return crossaddmul(f, g, 0, order);
}
inline static double div(Taylor_Series_1 *f, Taylor_Series_1 *g, Taylor_Series_1 *p, int order)
{
	return (get_coeff(f, order) - crossaddmul(p, g, 1, order)) / get_coeff(g, 0);
}
double static powt(Taylor_Series_1 *f, Taylor_Series_1 *p, double a, int order)
{
	if (0 == order)
		return pow(get_coeff(f, 0), a);
	double temp = 0;
	for (int i = 0; i < order; i++)
	{
		temp += (crossmul(f, p, order, i) * (a * (order - i) - i));
	}
	return temp / (order * get_coeff(f, 0));
}
double static powti(Taylor_Series_1 *f, Taylor_Series_1 *p, int a, int order)
{
	if (0 == order)
		return pow(get_coeff(f, 0), a);
	double temp = 0;
	for (int i = 0; i < order; i++)
	{
		temp += (crossmul(f, p, order, i) * (a * (order - i) - i));
	}
	return temp / (order * get_coeff(f, 0));
}
inline static double pow2(double a) { return a * a; }
inline static double powti2(Taylor_Series_1 *f, int order)
{
	double temp = 0;
	int odd = order & 1;
	int j = (order >> 1) + odd - 1;
	for (int i = 0; i <= j; i++)
	{
		temp += crossmul(f, f, order, i);
	}
	temp *= 2.;
	return odd ? temp : temp + pow2(get_coeff(f, order >> 1));
}
//-----------------------------TODO:以下代码未经测试
double static expt(Taylor_Series_1 *f, Taylor_Series_1 *p, double a, int order)
{
	double temp = 0;
	for (int i = 0; i < order; i++)
	{
		temp += (crossmul(f, p, order, i) * (order - i));
	}
	return temp / order;
}
double static sint(Taylor_Series_1 *f, Taylor_Series_1 *c, double a, int order)
{
	double temp = 0;
	for (int i = 0; i < order; i++)
	{
		temp += (crossmul(f, c, order, i) * (order - i));
	}
	return temp / order;
}
double static cost(Taylor_Series_1 *f, Taylor_Series_1 *s, double a, int order)
{
	double temp = 0;
	for (int i = 0; i < order; i++)
	{
		temp += (crossmul(f, s, order, i) * (order - i));
	}
	return -temp / order;
}
// TODO:pow2,sqrt,sincos
//-------------------
void evail(Taylor_Series_1 *f, int order, double dt)
{
	double temp = get_coeff(f, order);
	while (order-- > 0)
	{
		temp = temp * dt + get_coeff(f, order);
	}
	//return temp;//改掉
}
int getstep(Taylor_Series_1 *f, int *order, double *step) {} // TODO
// model:
enum model_out
{
	x,
	y,
	out_end
};
enum ModelVal
{
	rr = out_end,
	qr,
	ModelVal_end
};
//				V优化后的ode
inline static void kep1(Taylor_Series_1 *TS1, int order, double *out)
{
	double x2 = powti2(&TS1[x], order);
	double y2 = powti2(&TS1[y], order);
	TS1[rr].coeff[order] = x2 + y2;
	//set_coeff(&TS1[rr], x2 + y2, order);							   // rr=x^2+y^2
	set_coeff(&TS1[qr], powt(&TS1[rr], &TS1[qr], -1.5, order), order); // qr = rr^-1.5
	out[x] = -mul(&TS1[x], &TS1[qr], order);						   // ax=x*qr
	out[y] = -mul(&TS1[y], &TS1[qr], order);						   // ay=y*qr
}
// test:
int main()
{
	// init
	double data[ModelVal_end][MaxOrder + 1];
	Taylor_Series_1 ts1[ModelVal_end];
	for (int i = 0; i < ModelVal_end; i++)
	{
		set_TS1(&ts1[i], i, data[i]); // init
	}
	set_coeff(&ts1[x], 0.19999999999999996, 0);
	set_coeff(&ts1[x], 0, 1);
	set_coeff(&ts1[y], 0, 0);
	set_coeff(&ts1[y], 3., 1);
	double out[out_end];
	double ct;
	// init-end
	for (int i = 0; i <= 20-2; i++)
	{
		ct = ((i + 1) * (i + 2));
		ct = 1 / ct;
		kep1(ts1, i, out);
		set_coeff(&ts1[x], out[x] * ct, i + 2);
		set_coeff(&ts1[y], out[y] * ct, i + 2);
		//printf("ax=%e\tay=%e\n", out[x] * ct, out[y] * ct);
		printf("%f,",get_coeff(&ts1[x],i+2) * ct);
	}
	printf("%s","\n");
	for (int i = 0; i <= 20-2; i++)
	{
	printf("%f,",data[x][i]);
	}
	return 0;
}
// orther:
void toDx()
{
	// double ct = ((i + 1) * (i + 2));
	// 	ct = 1 / ct;
	// 	kep1(ts1, i, out);
	// 	set_coeff(&ts1[x], out[x] * ct, i + 2);
	// 	set_coeff(&ts1[y], out[y] * ct, i + 2);
}
/*
void testInt(double star, double end, double esp, int maxorder)
{
	// init同上
	double time;
	double step; // 可能要fp128存储
	int order, wo;
	while (time < end)
	{
		while (wo || order < maxorder)
		{ // 迭代至阶数
			// toDx()
			wo = 1;
			// for (int i = 0; i < out_end; i++){wo &= getstep(&ts1[i],&order,&step,esp...);}
		}
		// for (int i = 0; i < out_end; i++){evail(step));}
		time += step;
	}
}//这个文件作为生成模板，接下来写个程序将ode转换成带积分器的优化过的c源码，再用编译器编译为程序运行。
*/