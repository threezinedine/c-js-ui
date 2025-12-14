#include "cju/modules/render/render.h"

CuRendererContext gCuRendererContext = {};

void cuRendererInitialize()
{
	CU_RENDER_API(cuGraphicsRendererInitialize)(&gCuRendererContext);
}

b8 cuRendererShouldClose()
{
	return CU_RENDER_API(cuGraphicsRendererShouldClose)(&gCuRendererContext);
}

void cuRendererBeginFrame()
{
	CU_RENDER_API(cuGraphicsRendererBeginFrame)(&gCuRendererContext);
}

void cuRendererEndFrame()
{
	CU_RENDER_API(cuGraphicsRendererEndFrame)(&gCuRendererContext);
}

void cuRendererRender()
{
	CU_RENDER_API(cuGraphicsRendererRender)(&gCuRendererContext);
}

void cuRendererShutdown()
{
	CU_RENDER_API(cuGraphicsRendererShutdown)(&gCuRendererContext);
}