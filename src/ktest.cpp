#ifndef KT_H
#define KAKTUS_USE_JOLT
#ifdef KAKTUS_USE_JOLT

#endif // KAKTUS_USE_JOLT

#endif // !KT_H
#include "stdio.h"
int main()
{
	for (int i = 0; i < 65536; i++)
	{
		printf("page:%d\tindex:%d\n",i>>10,(i&0x3ff)<<2);
	}
	return 0;
}