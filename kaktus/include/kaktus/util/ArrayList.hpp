#ifndef KAKTUS_ALIST_H
#define KAKTUS_ALIST_H
#include "kaktus/util.h"
namespace kaktus
{
	template<typename T, size_t bucket>
	class ArrayList//RN
	{
	private:
		uint size, length,bsize;
		T** llist;
	public:
		&T operator[](int index)
		{
			return *llist[index / bsize][index % bsize];
		}
	};
}
#endif // !KAKTUS_ALIST_H
