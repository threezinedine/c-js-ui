#include "cju/platform/platform.h"

#if CU_PLATFORM_WINDOWS
#error "Windows platform is not supported yet."
#elif CU_PLATFORM_UNIX
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#if CU_USE_VULKAN
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#endif // CU_USE_VULKAN

static Display* g_pDisplay	 = CU_NULL;
static i32		g_screen	 = 0;
static Window	g_rootWindow = 0;
static XEvent	g_event;

/**
 * Internal platform-specific window structure for Unix (X11).
 * Holds the X11 Display and Window handles.
 */
typedef struct CuLinuxWindow
{
	Window window;
} CuLinuxWindow;

#elif CU_PLATFORM_WEB
#else
#error "Unsupported platform."
#endif

void cuWindowInitialize_default(void)
{
	CU_ASSERT(g_pDisplay == CU_NULL);
	g_pDisplay = XOpenDisplay(NULL);

	g_screen	 = DefaultScreen(g_pDisplay);
	g_rootWindow = RootWindow(g_pDisplay, g_screen);
}

CuWindow* cuWindowCreate_default(u32 width, u32 height, const char* title)
{
	CuWindow* pWindow = (CuWindow*)CU_PLATFORM_API(cuAllocate)(sizeof(CuWindow));
	CU_ASSERT(pWindow != CU_NULL);
	CU_PLATFORM_API(cuMemorySet)(pWindow, 0, sizeof(CuWindow));

	pWindow->width	   = width;
	pWindow->height	   = height;
	pWindow->title	   = title;
	pWindow->isRunning = CU_TRUE;

#if CU_PLATFORM_UNIX
	CuLinuxWindow* pLinuxWindow = (CuLinuxWindow*)CU_PLATFORM_API(cuAllocate)(sizeof(CuLinuxWindow));
	CU_ASSERT(pLinuxWindow != CU_NULL);
	CU_PLATFORM_API(cuMemorySet)(pLinuxWindow, 0, sizeof(CuLinuxWindow));

	CU_ASSERT(g_pDisplay != CU_NULL);

	pWindow->pPlatformData = pLinuxWindow;

	pLinuxWindow->window = XCreateSimpleWindow(g_pDisplay,
											   g_rootWindow,
											   0,
											   0,
											   width,
											   height,
											   1,
											   BlackPixel(g_pDisplay, g_screen),
											   WhitePixel(g_pDisplay, g_screen));

	XStoreName(g_pDisplay, pLinuxWindow->window, title);

	Atom wmDeleteMessage = XInternAtom(g_pDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(g_pDisplay, pLinuxWindow->window, &wmDeleteMessage, 1);

	XSelectInput(g_pDisplay, pLinuxWindow->window, ExposureMask | KeyPressMask | StructureNotifyMask);

	XMapWindow(g_pDisplay, pLinuxWindow->window);
#endif

	return pWindow;
}

CuWindowEvent cuWindowPollEvents_default(CuWindow* pWindow)
{
	CU_ASSERT(pWindow != CU_NULL);

	CuWindowEvent event;

#if CU_PLATFORM_UNIX
	CU_ASSERT(g_pDisplay != CU_NULL);
	event.type = CU_WINDOW_EVENT_TYPE_NONE;

	CuLinuxWindow* pLinuxWindow = (CuLinuxWindow*)pWindow->pPlatformData;

	XNextEvent(g_pDisplay, &g_event);

	if (g_event.type == ClientMessage)
	{
		Atom wmDeleteMessage = XInternAtom(g_pDisplay, "WM_DELETE_WINDOW", False);
		if ((Atom)g_event.xclient.data.l[0] == wmDeleteMessage)
		{
			event.type		   = CU_WINDOW_EVENT_TYPE_CLOSE_REQUESTED;
			pWindow->isRunning = CU_FALSE;
		}
	}
#endif

	return event;
}

#if CU_USE_VULKAN
VkSurfaceKHR cuWindowCreateVulkanSurface_default(CuWindow* pWindow, VkInstance instance)
{
	CU_ASSERT(pWindow != CU_NULL);
	CU_ASSERT(instance != VK_NULL_HANDLE);

	VkSurfaceKHR surface = VK_NULL_HANDLE;

#if CU_PLATFORM_UNIX
	CU_ASSERT(g_pDisplay != CU_NULL);

	CuLinuxWindow* pLinuxWindow = (CuLinuxWindow*)pWindow->pPlatformData;
	CU_ASSERT(pLinuxWindow != CU_NULL);

	VkXlibSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType						 = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.pNext						 = CU_NULL;
	surfaceCreateInfo.flags						 = 0;
	surfaceCreateInfo.dpy						 = g_pDisplay;
	surfaceCreateInfo.window					 = pLinuxWindow->window;

	VkResult result = vkCreateXlibSurfaceKHR(instance, &surfaceCreateInfo, CU_NULL, &surface);
	CU_ASSERT_MSG(result == VK_SUCCESS, "Failed to create Vulkan Xlib surface. Error code: %d", result);
#endif // CU_PLATFORM_UNIX
	return surface;
}

void cuWindowDestroyVulkanSurface_default(CuWindow* pWindow, VkInstance instance, VkSurfaceKHR surface)
{
	CU_ASSERT(pWindow != CU_NULL);
	CU_ASSERT(instance != VK_NULL_HANDLE);
	CU_ASSERT(surface != VK_NULL_HANDLE);

#if CU_PLATFORM_UNIX
	CU_ASSERT(g_pDisplay != CU_NULL);
	vkDestroySurfaceKHR(instance, surface, CU_NULL);
#endif // CU_PLATFORM_UNIX
}
#endif // CU_USE_VULKAN

void cuWindowDestroy_default(CuWindow* pWindow)
{
	CU_ASSERT(pWindow != CU_NULL);

#if CU_PLATFORM_UNIX
	CU_ASSERT(g_pDisplay != CU_NULL);

	CuLinuxWindow* pLinuxWindow = (CuLinuxWindow*)pWindow->pPlatformData;
	CU_ASSERT(pLinuxWindow != CU_NULL);
	XDestroyWindow(g_pDisplay, pLinuxWindow->window);

	CU_PLATFORM_API(cuFree)(pLinuxWindow, sizeof(CuLinuxWindow));
#endif

	CU_PLATFORM_API(cuFree)(pWindow, sizeof(CuWindow));
}

void cuWindowShutdown_default(void)
{
	CU_ASSERT(g_pDisplay != CU_NULL);
	XCloseDisplay(g_pDisplay);
}