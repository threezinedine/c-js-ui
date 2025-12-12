#include "cju/platform/platform.h"
#include <string.h>

b8 cuStrEqual(const char* pStrA, const char* pStrB)
{
	return (b8)(strcmp(pStrA, pStrB) == 0);
}