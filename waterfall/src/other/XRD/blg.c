#include "stdio.h"
#include "math.h"
inline float pow2(float x) { return x * x; }
typedef struct { float x, y; } gpoint;
typedef struct { float phi, L; }lpoint;
const float pi = 3.14159265358979323864f;
const float pi2 = 2 * 3.14159265358979323864f;
typedef union
{
	struct { float x, y; };
	gpoint gp;
	lpoint lp;
} dpoint;
float dot(dpoint *x, dpoint *y)
{
	return fmaf(x->x, y->x, x->y * y->y);
}
void add(dpoint* x, dpoint* y, dpoint* out)
{
	out->x = x->x + y->x;
	out->y = x->y + y->y;
}
float len(dpoint* p)
{
	return sqrtf(fmaf(p->x, p->x, pow2(p->y)));
}
void normal(dpoint* p)
{
	float t = 1/sqrtf(fmaf(p->x, p->x, pow2(p->y)));
	p->x *= t;
	p->y *= t;
}
int pw=100, ph=100;
//dpoint plist[10000];
void print(dpoint* p)
{
	printf("p.x:%f\tp.y:%f\tl:%f\n",p->x,p->y,len(p));
}
void printl(dpoint* p)
{
	printf("(%f,%f),", p->x, p->y);
}
void learnp(dpoint* p0,dpoint* x, dpoint* y, float a, float b, dpoint* out)
{
	out->x = fmaf(x->x, a, fmaf(y->x, b, p0->x));
	out->y = fmaf(x->y, a, fmaf(y->y, b, p0->y));
}
void learn(dpoint* x, dpoint* y, float a, float b, dpoint* out)
{
	out->x = fmaf(x->x, a, y->x * b);
	out->y = fmaf(x->y, a, y->y * b);
}
void fly(dpoint* op, int w, int h, float dlR, float dlL, float doR, float doL)
{
	int max = w * h;
	float t = 0,k;
	float pil = 0;
	dpoint pt2 = { 0,0 };
	for (int i = 0; i < w; i++)
	{
		t = i * dlL;
		pil = i * doL;
		for (int j = 0; j < h; j++)
		{
			t += dlR;
			pil += doR;
			k = (t+pil) / pi2;
			pt2.x = 0.1*cosf(k);
			pt2.y = 0.1*sinf(k);
			add(op, &pt2, op);
		}
	}
}
void testblg(dpoint*R, dpoint*L)
{
	float t, l;
	dpoint p = { 0,0 }, op = { 0,0 }, lp = { 0.866,-0.5 };
	normal(&lp);
	for (int i = 0; i <= 360; i++)
	{
		t = (float)i/180.;
		t *= pi;
		op.x = cosf(t);
		op.y = sinf(t);
		p.x = op.x;
		p.y = op.y;
		fly(&p, pw, ph, dot(&lp, R), dot(&lp, L), dot(&op, R), dot(&op, L));
		l = logf(len(&p) + 1);
		printf("(%f,%f),", op.x * l, op.y * l);
	}
}
void testrdx(dpoint* R, dpoint* L)
{
	float t, l;
	dpoint p = { 0,0 }, op = { 0,0 }, lp = {0,0};
	normal(&lp);
	for (int i = 0; i <= 360; i++)
	{
		t = (float)i / 180.;
		t *= pi;
		op.x = cosf(t);
		op.y = sinf(t);
		lp.x = op.x;
		lp.y = -op.y;
		p.x = op.x;
		p.y = op.y;
		fly(&p, pw, ph, 0, 0, dot(&op, R), dot(&op, L));
		l = logf(len(&p) + 1);
		printf("(%f,%f),", op.x * l, op.y * l);
	}
}
void testlg(dpoint* R, dpoint* L,int pw,int ph)
{
	dpoint p = { 0,0 }, p0 = { 0,0 };
	learn(L, R, -pw / 2., -ph / 2., &p0);
	for (int i = 0; i <= pw; i++)
	{
		for (int j = 0; j <= ph; j++)
		{
			learnp(&p0, L, R, i, j, &p);
			printf("(%f,%f),", p.x, p.y);
		}
	}
}
int main()
{
	dpoint R = { 0,1 }, L = { 1,0 };
	testrdx(&R, &L);
	//testlg(&R, &L, pw / 10, ph / 10);
	return 0;
}