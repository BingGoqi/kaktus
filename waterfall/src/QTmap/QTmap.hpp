#include <set>
#include <vector>
typedef uint32_t ui32;
typedef uint64_t ui64;
constexpr double mulla = 1/(double)UINT64_MAX;
static double longNomal(ui64 val)
{
	double a = (double)val*mulla;
}
typedef union vec2d
{
	struct
	{
		double x, y;
	};
	struct
	{
		double u, v;
	};
}vec2;
typedef struct QTFullNode
{
	ui32 ptr;
	ui32 len;
	bool operator()(const QTFullNode a, const QTFullNode b) const {
		return a.ptr > b.ptr;
	}
	ui32 getNext() const
	{
		return ptr + len;
	}
}QTFullNode;
typedef union QTNode
{
	struct
	{
		ui64 x0, y0;
		ui32 sup;
		ui32 key;
	};
	struct
	{
		ui32 a, b, c, d;
		ui32 sup, key;
	};
}QTNode;
class QTmap
{
private:
	std::set<QTFullNode,QTFullNode> mapSet;
	std::vector<QTNode> list;
public:
	QTmap()
	{
		mapSet.insert({ 0,0 });
	}
	ui32 addNode()
	{
		auto iter = mapSet.begin();
		QTFullNode node1 = *iter;
		ui32 i = node1.getNext();
		if (mapSet.size() > 1)
		{
			iter++;
			ui32 j = iter->ptr;
			if (i >= j)
			{
				node1.len = iter->getNext() - 1 - node1.ptr;
				mapSet.erase(iter);
			}
		}
		return i;
	}
	ui32 getNext()
	{
		return mapSet.begin()->getNext();
	}
	ui32 addNodeSegment(ui32 size)
	{
		if (mapSet.size() <= 1)
		{
			auto node = *mapSet.begin();
			node.len += size;
			return size;
		}
		else
		{
			auto iter = mapSet.begin();
			QTFullNode node1 = *iter;
			iter++;
			ui32 i = iter->getNext() - node1.getNext();
			if (i <= size)
			{
				node1.len = iter->getNext() - 1 - node1.ptr;
				mapSet.erase(iter);
				return size - i;
			}
			else
			{
				node1.len += i;
				return 0;
			}
		}
	}
};