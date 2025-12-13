#pragma once

#include "common.h"

typedef struct CuWindow
{
	void* pPlatformData; // Pointer to platform-specific window data

	u32			width;
	u32			height;
	const char* title;
	b8			isRunning; ///< Indicates if the window is running
} CuWindow;

/**
 * Starting the windowing system (if needed by the platform).
 */
CU_PLATFORM_API_DEFINE(void, cuWindowInitialize)

/**
 * Create a window with the specified width, height, and title.
 * @param width The width of the window.
 * @param height The height of the window.
 * @param title The title of the window.
 * @return A pointer to the created CuWindow instance.
 */
CU_PLATFORM_API_DEFINE(CuWindow*, cuWindowCreate, u32 width, u32 height, const char* title);

/**
 * All possible window event types.
 */
typedef enum CuWindowEventType
{
	CU_WINDOW_EVENT_TYPE_NONE = 0,
	CU_WINDOW_EVENT_TYPE_CLOSE_REQUESTED,
	// Future event types can be added here
} CuWindowEventType;

/**
 * Structure representing a window event.
 */
typedef struct CuWindowEvent
{
	CuWindowEventType type;
	// Future event data can be added here
} CuWindowEvent;

/**
 * Poll for window events (e.g., input, close events).
 * @param window A pointer to the CuWindow instance.
 */
CU_PLATFORM_API_DEFINE(CuWindowEvent, cuWindowPollEvents, CuWindow* pWindow);

/**
 * Destroy the specified window and free its resources.
 * @param window A pointer to the CuWindow instance to be destroyed.
 */
CU_PLATFORM_API_DEFINE(void, cuWindowDestroy, CuWindow* pWindow);

/**
 * Shutdown the windowing system (if needed by the platform).
 */
CU_PLATFORM_API_DEFINE(void, cuWindowShutdown)