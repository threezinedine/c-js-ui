#include <cju/cju.h>

int main(void)
{
	CU_PLATFORM_API(cuWindowInitialize)();

	CuWindow* pWindow = CU_PLATFORM_API(cuWindowCreate)(800, 600, "Override Memory Example");

	while (pWindow->isRunning)
	{
		CuWindowEvent event = CU_PLATFORM_API(cuWindowPollEvents)(pWindow);
		CU_UNUSED(event);
	}

	CU_PLATFORM_API(cuWindowDestroy)(pWindow);

	CU_PLATFORM_API(cuWindowShutdown)();
	return 0;
}