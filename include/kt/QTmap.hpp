#pragma 0
#include <vector>
#include <set>
//#include <map>
#define fma(x,y,z) (x)*(y)+z
namespace kv {
	const float d75 = sin(M_PI / 5);
	const float a = 4. / sqrt(10 + 2 * sqrt(5));
	const float r = a / 2 / sin(M_PI / 5);
	const float h = sqrt(a * a - r * r);
	constexpr float invqt = 1. / 8388608;//2^-23
	constexpr static inline float pow2(float x) { return x * x; }
	constexpr static inline uint16 getdl(uint16 ord)
	{
		return 1 << (23 - ord);
	}
	typedef unsigned short uint16;
	/*
	|a|b|c|d|sup|key
	|x0 |y0 |ord|key|ptr
	*/
	typedef union Node
	{
		char data[16];
		struct
		{
			uint16 a, b, c, d;
			uint16 sup;
			uint16 key;
		};
		struct//leaf
		{
			unsigned int x0, y0;
			uint16 ord;
			uint16 key, ptr;
		};
	}Node;
	typedef union texture
	{
		struct
		{
			uint16 key, ptr;
		};
	}texture;
	class textureMap
	{
	private:
		std::vector<texture> textureList;
		uint16 length, free = 0, empty = 0;
	public:
		uint16 addtexture()
		{
			if (empty == 0)
			{
				return length++;
			}
			else
			{
				empty--;
				uint16 t = free;
				free = textureList[free].ptr;
				return t;
			}
		}
		void freeNode(uint16 ID)
		{
			if (ID == length - 1)
			{
				length--;
				textureList.resize(length);
			}
			else
			{
				textureList[ID].ptr = free;
				free = ID;
				empty++;
			}
		}
	};
	class QTmap
	{
	private:
		std::vector<Node> NodeList;
		uint16 length,free=0,empty=0;
	public:
		QTmap()
		{
			NodeList = {};
		}
		void addNoe(Node& in)
		{
			if (empty == 0)
			{
				NodeList[length++] = Node(in);
			}
			else
			{
				empty--;
				uint16 t = free;
				free = NodeList[free].ptr;
				NodeList[t] = Node(in);
			}
		}
		Node& addNode()
		{
			if (empty == 0)
			{
				return NodeList[length++];
			}
			else
			{
				empty--;
				uint16 t = free;
				free = NodeList[free].ptr;
				return NodeList[t];
			}
		}
		void freeNode(uint16 ID)
		{
			if (ID == length - 1)
			{
				length--;
				NodeList.resize(length);
			}
			else
			{
				NodeList[ID].ptr = free;
				free = ID;
				empty++;
			}
		}
	};
	typedef struct
	{
		float x, y;
	}vec2f;
	typedef struct
	{
		float x, y, z;
	}vec3f;
	typedef struct
	{
		float x, y, z, w;
	}vec4f;
}
#undef fma