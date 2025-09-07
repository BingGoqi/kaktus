#include <stdio.h>
#include <cmath>
#include <vector>
#include "delaunator.hpp"
#define Number 3000
#define sqrt2 1.4142135623731f
#define pi 2*3.14159265358979323846f
#define min(x,y) x>y?y:x
#define max(x,y) x>y?x:y
#define rand(x) x ^= x << 13;x ^= x >> 17;x ^= x << 5
#define ar 0.04f
constexpr int an = (int)(sqrt2 / ar + 1);
typedef unsigned int ui32;
typedef unsigned short int ui16;
using namespace std;
typedef struct point
{
	float x, y;
}point;
float length(point* a, point* b)
{
	float dx = a->x - b->x,dy = a->y-b->y;
	return sqrtf(fmaf(dx, dx, dy * dy));
}
float length2(point* a, point* b)
{
	float dx = a->x - b->x, dy = a->y - b->y;
	return fmaf(dx, dx, dy * dy);
}
inline ui32 XorShift32()
{
	static ui32 seed = 147653814872;
	ui32 x = seed;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	seed = x;
	return x;
}
inline float Range()
{
	return 1.f*XorShift32()/ 4294967295.f;
}
void randP(point* p,float r)
{
	float t = Range() * pi;
	p->x = Range() * r;
	p->y = p->x * sinf(t);
	p->x *= cosf(t);
}
void randP(point* p, float r1,float r2)
{
	float t = Range() * pi;
	p->x = Range() * (r2-r1)+r1;
	p->y = p->x * sinf(t);
	p->x *= cosf(t);
}
static vector<point> list;
static ui16 map[an][an] = { 0 };
int clamp(int x, int min, int max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}
ui32 mapget(int x, int y,int N)
{
	if (x >= N) return 0;
	if (x < 0) return 0;
	if (y >= N) return 0;
	if (y < 0) return 0;
	return map[x][y];
}
void mapadd(point* p,int N)
{
	int id = list.size();
	int x = (int)(p->x * N), y = (int)(p->y * N);
	if (map[x][y] == 0)
	{
	list.push_back({ p->x,p->y });
	map[x][y] = id;
	}
	else
	{
		printf("aa%f\n",length(p,&list[map[x][y]]));
	}
}
bool check_collision(point* p,int N,float radius2)
{
	int x = (int)(p->x * N), y = (int)(p->y * N);
	int mx, my;
	int ip;
	point cp;
	float r2;
	for (int i = 0; i < 25; i++)
	{
		mx = (i % 5) - 2 + x;
		my = (i / 5) - 2 + y;
		ip = mapget(mx, my, N);
		if (0 == ip)continue;
		cp = list[ip];
		r2 = length2(p, &cp);
		if (r2 < radius2)
		{
			return false;
		}
	}
	return true;
}
int main()
{
	FILE* fout;
	fopen_s(&fout,"out.stl", "w");
	float r = ar,r2 = r*r;
	int N = an;
	//(*map) = new ui32[N * N]{0};
	point tp = { 0,0 };
	list.push_back({ 0.5,0.5 });
	point ip;
	randP(&tp, r * 2);
	tp.x += 0.5;
	tp.y += 0.5;
	mapadd(&tp, N);
	int i = 0;
	while (i < list.size())
	{
		ip = list[i];
		for (int j = 0; j < 100; j++)
		{
			randP(&tp, r, 2*r);
			tp.x += ip.x;
			tp.y += ip.y;
			if (tp.x > 1. || tp.x < 0. || tp.y > 1. || tp.y < 0.) continue;
			if (check_collision(&tp, N, r2))
			{
				mapadd(&tp, N);
				//printf("(%f,%f),",tp.x, tp.y);
				//printf("%f\t%f\t%f\n",tp.x,tp.y,Range());
			}
		}
		i++;
	}
	printf("%d\n", i);
	vector<double> dlist = vector<double>(list.size() * 2);
	for (int i = 0, j = list.size(); i < j; i++)
	{
		dlist[2 * i] = list[i].x;
		dlist[2 * i+1] = list[i].y;
	}
	list.clear();
	delaunator::Delaunator dd(dlist);
	point pa, pb, pc;
	point da, db,ph;
	float S;
	for (std::size_t i = 0; i < dd.triangles.size(); i += 3)
	{
		pa.x = dd.coords[2 * dd.triangles[i]];
		pa.y = dd.coords[2 * dd.triangles[i] + 1];
		pb.x = dd.coords[2 * dd.triangles[i + 1]];
		pb.y = dd.coords[2 * dd.triangles[i + 1] + 1];
		pc.x = dd.coords[2 * dd.triangles[i + 2]];
		pc.y = dd.coords[2 * dd.triangles[i + 2] + 1];
		da.x = pb.x - pa.x;
		da.y = pb.y - pa.y;
		db.x = pc.x - pa.x;
		db.y = pc.y - pa.y;
		ph.x = pa.x + da.x / 4 + db.x / 4;
		ph.y = pa.y + da.y / 4 + db.y / 4;
		S = fabs(da.x * db.y - db.x * da.y)*1000;
		S = sqrtf(S);
		S *= 0.01;
		fprintf(fout, "facet normal %f %f %f\nouter loop\nvertex %f %f %f\nvertex %f %f %f\nvertex %f %f %f\nendloop\nendfacet\n",
			0., 0., 0.,
			pa.x, pa.y, 0.,
			pb.x, pb.y, 0.,
			ph.x, ph.y, S);
		fprintf(fout, "facet normal %f %f %f\nouter loop\nvertex %f %f %f\nvertex %f %f %f\nvertex %f %f %f\nendloop\nendfacet\n",
			0., 0., 0.,
			pb.x, pb.y, 0.,
			pc.x, pc.y, 0.,
			ph.x, ph.y, S);
		fprintf(fout, "facet normal %f %f %f\nouter loop\nvertex %f %f %f\nvertex %f %f %f\nvertex %f %f %f\nendloop\nendfacet\n",
			0., 0., 0.,
			pc.x, pc.y, 0.,
			pa.x, pa.y, 0.,
			ph.x, ph.y, S);
	}
	/*
	for (std::size_t i = 0; i < dd.triangles.size(); i += 3) {
		fprintf(fout,"facet normal %f %f %f\nouter loop\nvertex %f %f %f\nvertex %f %f %f\nvertex %f %f %f\nendloop\nendfacet\n",
			0, 0, 0,
			dd.coords[2 * dd.triangles[i]], dd.coords[2 * dd.triangles[i] + 1], i/100.,//1x,1y
			dd.coords[2 * dd.triangles[i + 1]],dd.coords[2 * dd.triangles[i + 1] + 1], i/100.,//2x,2y
			dd.coords[2 * dd.triangles[i + 2]],dd.coords[2 * dd.triangles[i + 2] + 1], i/100.);//3x,3y
	}*/
	//delete[] * map;
	printf("%d\n", (int)(3 * dd.triangles.size()));
	dlist.clear();
	return 0;
}