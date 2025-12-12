#include "cju/cju.h"

int main()
{
	void* ptr = cuAllocate(3);

	// cuFree(ptr, 3);

	cuMemoryAssertNoLeaks();

	return 0;
}