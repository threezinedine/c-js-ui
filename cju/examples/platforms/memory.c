#include "cju/cju.h"

int main()
{
	void* ptr = CU_PLATFORM_API(cuAllocate)(3);

	// CU_PLATFORM_API(cuFree)(ptr, 3);

	cuMemoryAssertNoLeaks();

	return 0;
}