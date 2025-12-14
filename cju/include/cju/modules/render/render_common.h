#pragma once

/**
 * Definitions for rendering API function pointers and default implementations.
 * Users can override these functions to provide custom rendering behavior.
 * If no custom implementation is provided, default implementations will be used.
 *
 * @example examples/modules/rendering/custom_renderer.c
 * ```c
 * CU_RENDER_API_DEFINE(void, cuCustomRenderFunction, CuRenderTarget* pTarget)
 * // typedef void (*PFN_cuCustomRenderFunction)(CuRenderTarget* pTarget);
 * void cuCustomRenderFunction_default(CuRenderTarget* pTarget);
 * ```
 */
#define CU_RENDERER_API_DEFINE(return, api, ...)                                                                       \
	typedef return (*PFN_##api)(__VA_ARGS__);                                                                          \
	return api##_default(__VA_ARGS__);

typedef struct CuRendererContext
{
	// Future context data can be added here
} CuRendererContext;