#pragma 0
#define PAGESIZE 4096//4KB
#define OFFSET 4
constexpr int paegNum = 65536*OFFSET/PAGESIZE;//64p
typedef unsigned short sptr;
class shortPool
{
private:
	static shortPool* poollist[];
	char* pagelist[paegNum];
public:
	shortPool()
	{
		pagelist[0] = new char[PAGESIZE];
	}
	void* operator[](sptr i)
	{
		return &pagelist[i>>10]+((i&0x3ff)<<2);
	}
	sptr mcallo(int size)
	{

	}
};