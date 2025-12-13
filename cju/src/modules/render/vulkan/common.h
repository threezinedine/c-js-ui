#pragma once

#include "cju/modules/render/render.h"
#include "cju/platform/platform.h"
#include <vulkan/vulkan.h>

typedef struct CuVulkanContext
{
	VkInstance instance;

#if CU_DEBUG
	VkDebugUtilsMessengerEXT debugMessenger; ///< Only used in debug mode for validation layers

#endif // CU_DEBUG

} CuVulkanContext;

extern CuVulkanContext gCuVulkanContext;

const char* gVulkanInstanceExtensions[] = {
	"VK_KHR_surface",
	"VK_EXT_debug_utils",
};

const char* gVulkanInstanceLayers[] = {
	"VK_LAYER_KHRONOS_validation",
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