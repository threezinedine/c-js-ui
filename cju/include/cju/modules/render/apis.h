#pragma once

/**
 * @file apis.h
 * All the possible rendering API methods which the user can override,
 * such as initializing the renderer, creating render targets, drawing, etc.
 * If the user does not provide an implementation, the default one will be used.
 */

#define CU_RENDER_API_ATTRIBUTE(api) PFN_##api api

/**
 * Structure holding function pointers for rendering API implementations.
 * Users can provide their own implementations for these functions to override
 * the default behavior.
 */
typedef struct CuRenderAPI
{
	// Future rendering API methods can be added here
} CuRenderAPI;

extern CuRenderAPI gCuRenderAPI;

/**
 * Macro to set a custom implementation for a rendering API method.
 * @param api The name of the rendering API method.
 * @param func The function pointer to the custom implementation.
 *
 * @example
 * ```C
 * CU_RENDER_API_SET(cuCustomRenderFunction, myCustomRenderFunction);
 * ```
 */
#define CU_RENDER_API_SET(api, func) (gCuRenderAPI.api = (PFN_##api)func)

/**
 * Macro to access a rendering API method, using the custom implementation
 * if provided, or falling back to the default implementation otherwise.
 * @param api The name of the rendering API method.
 */
#define CU_RENDER_API(api) (gCuRenderAPI.api == CU_NULL ? api##_default : gCuRenderAPI.api)