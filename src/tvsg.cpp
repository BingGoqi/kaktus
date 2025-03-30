#include "stdio.h"
#define _USE_MATH_DEFINES
#include "math.h"
#define cbit 63
#define fma(x,y,z) (x)*(y)+z
const double d75 = sin(M_PI / 5);
const double a = 4. / sqrt(10 + 2 * sqrt(5));
const double r = a / 2 / sin(M_PI / 5);
const double h = sqrt(a * a - r * r);
typedef unsigned long long ui64;
constexpr double invqt = 1. / (9223372036854775808);//2^-63
constexpr static inline double pow2(double x) { return x * x; }
constexpr static inline ui64 getdl(char ord)
{
	return 1LL << (cbit - ord);
}
typedef struct QTreeData
{
	double x0, y0;
	char ord;
	constexpr  QTreeData(double x0, double y0, char ord) :x0(x0), y0(y0), ord(ord){};
}QTreeData;
class QTreeNode
{
private:
	QTreeData* data = nullptr;
	QTreeNode* n0 = nullptr, * n1 = nullptr, * n2 = nullptr, * n3 = nullptr;
public:
	QTreeNode()
	{
		data = new QTreeData(0., 0., 0);
	};
	QTreeNode(double x0, double y0, char ord)
	{
		data = new QTreeData(x0, y0, ord);
	}
	void addSub()
	{
		if (data == nullptr)return;
		double x0, y0, x1, y1, dl;
		int ord = data->ord + 1;
		dl = invqt * getdl(ord);
		x0 = data->x0;
		y0 = data->y0;
		x1 = x0 + dl;
		y1 = y0 + dl;
		*n0 = QTreeNode(x0, y0, ord);
		*n1 = QTreeNode(x1, y0, ord);
		*n2 = QTreeNode(x0, y1, ord);
		*n3 = QTreeNode(x1, y1, ord);
		delete data;
		data = nullptr;
	}
	void merge()
	{
		if (data != nullptr)return;
		if (n0->data == nullptr)
		{
			n0->merge();
			n1->merge();
			n2->merge();
			n3->merge();
		}
		else
		{
			data = new QTreeData(n0->data->x0, n0->data->y0, n0->data->ord - 1);
		}
	}
	~QTreeNode()
	{
		if (n1 == nullptr) return;
		n0->~QTreeNode();
		n1->~QTreeNode();
		n2->~QTreeNode();
		n3->~QTreeNode();
	}
};
typedef struct
{
	double x, y;
}vec2d;
typedef struct
{
	double x, y, z;
}vec3d;
typedef struct
{
	double x, y, z, w;
}vec4d;
constexpr static inline double Line(double x, double y, double k)
{
	return fma(y - x, k, x);
}
constexpr static void LineT(vec3d& p0, vec3d& p1, vec3d& p2, vec2d& uv, vec3d& out)
{
	out.x = fma(p1.x - p0.x, uv.x, fma(p2.x - p0.x, uv.y, p0.x));
	out.y = fma(p1.y - p0.y, uv.x, fma(p2.y - p0.y, uv.y, p0.y));
	out.z = fma(p1.z - p0.z, uv.x, fma(p2.z - p0.z, uv.y, p0.z));
}
constexpr static void LineQ(vec3d& p0, vec3d& p1, vec3d& p2, vec3d& p3, vec2d& uv, vec3d& out)
{
	if (uv.x + uv.y < 1)
	{
		vec2d tuv = uv;
		LineT(p0, p1, p2, tuv, out);
	}
	else
	{
		vec2d tuv = { 1 - uv.x,1 - uv.y };
		LineT(p3, p2, p1, tuv, out);
	}
}
constexpr static void getCenVec(vec2d& out, QTreeData& in)
{
	double t = invqt * getdl(in.ord + 1);
	out.x = in.x0 + t;
	out.y = in.y0 + t;
}
constexpr static void getSupVec(vec2d& uv, QTreeData& in, vec2d& UV)
{
	double t = invqt * getdl(in.ord);
	UV.x = fma(t, uv.x, in.x0);
	UV.y = fma(t, uv.y, in.y0);
}
constexpr static void getRefVec(int i, QTreeData& in, vec2d& UV)
{
	double t = invqt * getdl(in.ord);
	UV.x = (i & 1) == 0 ? in.x0 : in.x0 + t;
	UV.y = (i & 3) == 0 ? in.y0 : in.y0 + t;
}
static vec3d p20d[12] = { 0. };
constexpr static void init()
{
	if (p20d[0].z == 1.)return;
	int j = 0;
	p20d[0].x = p20d[0].y = p20d[11].x = p20d[11].y = 0;
	p20d[0].z = 1.;
	p20d[11].z = -1.;
	for (int i = 1; i < 6; i++)
	{
		j = 2 * i-2;
		p20d[i].x = r * cos(M_PI / 5. * j);
		p20d[i].y = r * sin(M_PI / 5. * j);
		p20d[i].z = 1-h;
	}
	for (int i = 6; i < 11; i++)
	{
		j = 2 * (i-6)-1;
		p20d[i].x = r * cos(M_PI / 5. * j);
		p20d[i].y = r * sin(M_PI / 5. * j);
		p20d[i].z = -1+h;
	}
}
constexpr static int p20i[] = { 0,1,2,7,0,2,3,8,0,3,4,9,0,4,5,10,0,5,1,6,11,7,6,1,11,8,7,2,11,9,8,3,11,10,9,4,11,6,10,5};
static QTreeNode qtlist[10];
constexpr static void getP20v3(vec3d& out, vec2d& in, int i)
{
	i *= 4;
	vec3d a = p20d[p20i[i]];
	vec3d b = p20d[p20i[i + 1]];
	vec3d c = p20d[p20i[i + 2]];
	vec3d d = p20d[p20i[i + 3]];
	LineQ(a, b, c, d, in, out);
}
static void normal(vec3d v)
{
	double invl = 1 / sqrt(pow2(v.x) + pow2(v.y) + pow2(v.z));
	v.x *= invl;
	v.y *= invl;
	v.z *= invl;
}
/*
int main()
{
	init();
	vec2d uv = { 0.5,0.5 };
	vec3d ot = { 0 };
	int k = 0;
	for (int i = 8; i < 10; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			k = j / 8;
			uv.x = (j % 8)/8.;
			uv.y = k / 8.;
			getP20v3(ot, uv, i);
			printf("(%lf,%lf,%lf),", ot.x, ot.y, ot.z);
		}
		//printf("%s", "\n\n");
	}
	return 0;
}
*/
int main()
{
	init();
	vec2d uv = { 0.5,0.5 };
	vec3d ot = { 0 };
	int k = 0;
	for (int i = 0; i < 12; i++)
	{
		ot = p20d[i];
		printf("(%lf,%lf,%lf),", ot.x, ot.y, ot.z);
	}
	return 0;
}