#pragma once

#include "render_common.h"

/**
 * Start the graphics renderer module.
 *
 * @param pContext Pointer to a CuRendererContext structure containing
 */
CU_RENDERER_API_DEFINE(void, cuGraphicsRendererInitialize, const CuRendererContext* pContext);

/**
 * Check if the graphics renderer should close.
 * @param pContext Pointer to a CuRendererContext structure containing
 * @return CU_TRUE if the graphics renderer should close, CU_FALSE otherwise.
 */
CU_RENDERER_API_DEFINE(b8, cuGraphicsRendererShouldClose, const CuRendererContext* pContext);

/**
 * Begin rendering a new frame in the graphics renderer module.
 * @param pContext Pointer to a CuRendererContext structure containing
 */
CU_RENDERER_API_DEFINE(void, cuGraphicsRendererBeginFrame, const CuRendererContext* pContext);

/**
 * End rendering the current frame in the graphics renderer module.
 * @param pContext Pointer to a CuRendererContext structure containing
 */
CU_RENDERER_API_DEFINE(void, cuGraphicsRendererEndFrame, const CuRendererContext* pContext);

/**
 * Render the current scene in the graphics renderer module.
 */
CU_RENDERER_API_DEFINE(void, cuGraphicsRendererRender, const CuRendererContext* pContext);

/**
 * Release resources used by the graphics renderer module.
 */
CU_RENDERER_API_DEFINE(void, cuGraphicsRendererShutdown, const CuRendererContext* pContext);