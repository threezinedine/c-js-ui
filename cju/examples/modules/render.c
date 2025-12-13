#include <cju/cju.h>

int main(void)
{
	cuRendererInitialize();

	while (!cuRendererShouldClose())
	{
		cuRendererBeginFrame();

		cuRendererEndFrame();
		cuRendererRender();
	}

	cuRendererShutdown();
}