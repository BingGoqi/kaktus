#pragma once//¿½±´»æÍ¼
#define SRC_DEBUG 1
#if SRC_DEBUG
#include <stdio.h>
#include <string.h>
#endif
void DDPTR_Copy(__int8* p1, __int8* p2, int len)
{
	while (len-- > 0)
	{
		*(p2++) = *(p1++);
	}
	memcpy(p1, p2, len);
}