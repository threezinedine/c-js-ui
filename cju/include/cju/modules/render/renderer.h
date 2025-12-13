#pragma once

// ========================= Global APIS ========================= //
#include "cju/core/core.h"
#include "cju/platform/platform.h"

/**
 * Starting the renderer system.
 */
void cuRendererInitialize();

/**
 * Check if the renderer should close.
 * @return CU_TRUE if the renderer should close, CU_FALSE otherwise.
 */
b8 cuRendererShouldClose();

/**
 * Begin rendering a new frame.
 */
void cuRendererBeginFrame();

/**
 * End rendering the current frame.
 */
void cuRendererEndFrame();

/**
 * Render the current scene.
 */
void cuRendererRender();

/**
 * Shutting down the renderer system.
 */
void cuRendererShutdown();