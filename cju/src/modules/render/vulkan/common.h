#pragma once

#include "cju/modules/render/render.h"
#include "cju/platform/platform.h"
#include <vulkan/vulkan.h>

typedef struct CuQueueFamilyIndex
{
	b8	familyFound; ///< Indicates if a suitable family was found
	i32 familyIndex; ///< The index of the found family
} CuQueueFamilyIndex;

/**
 * Needed to tie a Vulkan surface to a platform window.
 */
typedef struct CuVulkanWindow
{
	CuWindow*	 pWindow;
	VkSurfaceKHR surface; ///< The Vulkan surface associated with the window
} CuVulkanWindow;

typedef struct CuVulkanContext
{
	VkInstance instance;

#if CU_DEBUG
	VkDebugUtilsMessengerEXT debugMessenger; ///< Only used in debug mode for validation layers

#endif // CU_DEBUG

	VkPhysicalDevice physicalDevice;

	/// Queue family indices
	CuQueueFamilyIndex graphicsFamily;
	CuQueueFamilyIndex presentFamily; ///< The queue family index that supports presentation to this surface

	VkDevice device;
} CuVulkanContext;

extern CuVulkanContext gCuVulkanContext;

const char* gVulkanInstanceExtensions[] = {
	"VK_KHR_surface",
	"VK_EXT_debug_utils",

#if CU_USE_VULKAN && CU_PLATFORM_UNIX
	"VK_KHR_xlib_surface",
#endif // CU_USE_VULKAN && CU_PLATFORM_UNIX
};

const char* gVulkanInstanceLayers[] = {
	"VK_LAYER_KHRONOS_validation",
};

const char* gVulkanDeviceExtensions[] = {
	"VK_KHR_swapchain",
};

#if CU_DEBUG
/**
 * Used for asserting Vulkan function call results.
 * If the call does not return VK_SUCCESS, the program will assert in debug mode.
 */
#define VK_ASSERT(call)                                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		VkResult result = call;                                                                                        \
		CU_ASSERT_MSG(result == VK_SUCCESS, "Vulkan call '" #call "' failed with error code %d", result);              \
	} while (CU_FALSE)

#else // CU_DEBUG
#define VK_ASSERT(call) call
#endif // CU_DEBUG