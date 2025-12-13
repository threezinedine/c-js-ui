#if CU_USE_VULKAN

#include "cju/modules/release_stack.h"
#include "common.h"

static CuReleaseStack* g_pReleaseStack	= CU_NULL;
CuVulkanContext		   gCuVulkanContext = {};	   /// the global vulkan context
static CuVulkanWindow* g_pMainWindow	= CU_NULL; ///< current support 1 window TODO: multiple windows later

// ======================== Vulkan Helper Functions ========================
static void assertRequiredVulkanExtensions();
static void assertRequiredVulkanLayers();
static void createVulkanInstance();
static void choosePhysicalDevice();
static void createMainVulkanSurface();
static void chooseGraphicsQueueFamily();
static void choosePresentQueueFamily();
static void createLogicalDevice();

#if CU_DEBUG
static void createDebugMessenger();
#endif // CU_DEBUG

// ========================= Remove APIS ========================= //
static void shutdownWindow();

void cuRendererInitialize()
{
	CuLoggerConfig loggerConfig = {};
	loggerConfig.minLogLevel	= CU_LOG_LEVEL_DEBUG;

	CuHandler consoleHandler = {};
	consoleHandler.log		 = cuConsoleLoggerLog;

	cuLoggerInitialize(&loggerConfig);
	cuLoggerAddHandler(&consoleHandler);

	g_pReleaseStack = cuReleaseStackCreate();

	CU_PLATFORM_API(cuWindowInitialize)();

	g_pMainWindow = CU_PLATFORM_API(cuAllocate)(sizeof(CuVulkanWindow));
	CU_ASSERT(g_pMainWindow != CU_NULL);

	g_pMainWindow->pWindow = CU_PLATFORM_API(cuWindowCreate)(800, 600, "C-JS-UI Renderer Window");

	cuReleaseStackPush(g_pReleaseStack, shutdownWindow, CU_NULL);

	// Setup Vulkan
	{
		assertRequiredVulkanExtensions();
		assertRequiredVulkanLayers();

		createVulkanInstance();

#if CU_DEBUG
		createDebugMessenger();
#endif // CU_DEBUG

		choosePhysicalDevice();
		createMainVulkanSurface();
		chooseGraphicsQueueFamily();
		choosePresentQueueFamily();
		createLogicalDevice();
	}
}

b8 cuRendererShouldClose()
{
	return !g_pMainWindow->pWindow->isRunning;
}

void cuRendererBeginFrame()
{
	CU_PLATFORM_API(cuWindowPollEvents)(g_pMainWindow->pWindow);
}

void cuRendererEndFrame()
{
}

void cuRendererRender()
{
}

void cuRendererShutdown()
{
	cuReleaseStackExecuteAndDestroy(g_pReleaseStack);

	cuLoggerShutdown();
}

static void destroyVulkanInstance(void* pUserData);

static void createVulkanInstance()
{
	CU_ASSERT(gCuVulkanContext.instance == VK_NULL_HANDLE);
	CU_ASSERT(g_pReleaseStack != CU_NULL);

	VkApplicationInfo appInfo  = {};
	appInfo.sType			   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext			   = CU_NULL;
	appInfo.pApplicationName   = "C-JS-UI Vulkan Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName		   = "C-JS-UI Engine";
	appInfo.engineVersion	   = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion		   = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceInfo = {};

	instanceInfo.sType			  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext			  = CU_NULL;
	instanceInfo.pApplicationInfo = &appInfo;

	instanceInfo.enabledExtensionCount	 = CU_ARRAY_SIZE(gVulkanInstanceExtensions);
	instanceInfo.ppEnabledExtensionNames = gVulkanInstanceExtensions;

	instanceInfo.enabledLayerCount	 = CU_ARRAY_SIZE(gVulkanInstanceLayers);
	instanceInfo.ppEnabledLayerNames = gVulkanInstanceLayers;

	VK_ASSERT(vkCreateInstance(&instanceInfo, CU_NULL, &gCuVulkanContext.instance));
	CU_LOG_DEBUG("Vulkan instance created successfully.");
	cuReleaseStackPush(g_pReleaseStack, destroyVulkanInstance, CU_NULL);
}

static void destroyVulkanInstance(void* pUserData)
{
	CU_UNUSED(pUserData);
	CU_ASSERT(gCuVulkanContext.instance != VK_NULL_HANDLE);

	vkDestroyInstance(gCuVulkanContext.instance, CU_NULL);
	gCuVulkanContext.instance = VK_NULL_HANDLE;

	CU_LOG_DEBUG("Vulkan instance destroyed successfully.");
}

static void shutdownWindow(void* pUserData)
{
	CU_UNUSED(pUserData);

	CU_PLATFORM_API(cuWindowDestroy)(g_pMainWindow->pWindow);
	g_pMainWindow->pWindow = CU_NULL;

	CU_PLATFORM_API(cuFree)(g_pMainWindow, sizeof(CuVulkanWindow));
	g_pMainWindow = CU_NULL;

	CU_PLATFORM_API(cuWindowShutdown)();
}

#if CU_DEBUG
static VkBool32 VKAPI_PTR debugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT	   messageSeverity,
											   VkDebugUtilsMessageTypeFlagsEXT			   messageTypes,
											   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
											   void*									   pUserData);

static void destroyDebugMessenger(void* pUserData);

static void createDebugMessenger()
{
	CU_ASSERT(gCuVulkanContext.instance != VK_NULL_HANDLE);
	CU_ASSERT(gCuVulkanContext.debugMessenger == VK_NULL_HANDLE);

	VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};

	debugMessengerInfo.sType		   = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugMessengerInfo.pfnUserCallback = debugMessageCallback;
	debugMessengerInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
	debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	PFN_vkCreateDebugUtilsMessengerEXT createCallback = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
		gCuVulkanContext.instance, "vkCreateDebugUtilsMessengerEXT");
	CU_ASSERT(createCallback != CU_NULL);

	PFN_vkDestroyDebugUtilsMessengerEXT destroyCallback = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
		gCuVulkanContext.instance, "vkDestroyDebugUtilsMessengerEXT");
	CU_ASSERT(destroyCallback != CU_NULL);

	VK_ASSERT(
		createCallback(gCuVulkanContext.instance, &debugMessengerInfo, CU_NULL, &gCuVulkanContext.debugMessenger));
	CU_LOG_DEBUG("Vulkan debug messenger created successfully.");
	cuReleaseStackPush(g_pReleaseStack, destroyDebugMessenger, CU_NULL);
}

static VkBool32 VKAPI_PTR debugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT	   messageSeverity,
											   VkDebugUtilsMessageTypeFlagsEXT			   messageTypes,
											   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
											   void*									   pUserData)
{
	CU_UNUSED(messageSeverity);
	CU_UNUSED(messageTypes);
	CU_UNUSED(pUserData);

	CU_LOG_WARN("Vulkan Debug Message: %s", pCallbackData->pMessage);

	return VK_FALSE;
}

static void destroyDebugMessenger(void* pUserData)
{
	CU_UNUSED(pUserData);
	CU_ASSERT(gCuVulkanContext.instance != VK_NULL_HANDLE);
	CU_ASSERT(gCuVulkanContext.debugMessenger != VK_NULL_HANDLE);

	PFN_vkDestroyDebugUtilsMessengerEXT destroyCallback = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
		gCuVulkanContext.instance, "vkDestroyDebugUtilsMessengerEXT");

	destroyCallback(gCuVulkanContext.instance, gCuVulkanContext.debugMessenger, CU_NULL);
	gCuVulkanContext.debugMessenger = VK_NULL_HANDLE;

	CU_LOG_DEBUG("Vulkan debug messenger destroyed successfully.");
}

#endif // CU_DEBUG

static void assertRequiredVulkanExtensions()
{
	u32 extensionsCount = 0;
	vkEnumerateInstanceExtensionProperties(CU_NULL, &extensionsCount, CU_NULL);
	VkExtensionProperties* pAvailableExtensions =
		CU_PLATFORM_API(cuAllocate)(sizeof(VkExtensionProperties) * extensionsCount);
	vkEnumerateInstanceExtensionProperties(CU_NULL, &extensionsCount, pAvailableExtensions);

	u32 instanceRequiredExtensionsCount = CU_ARRAY_SIZE(gVulkanInstanceExtensions);

	for (u32 requiredExtensionIndex = 0u; requiredExtensionIndex < instanceRequiredExtensionsCount;
		 ++requiredExtensionIndex)
	{
		b8			extensionFound		  = CU_FALSE;
		const char* requiredExtensionName = gVulkanInstanceExtensions[requiredExtensionIndex];

		for (u32 availableExtensionIndex = 0u; availableExtensionIndex < extensionsCount; ++availableExtensionIndex)
		{
			const char* availableExtensionName = pAvailableExtensions[availableExtensionIndex].extensionName;

			if (cuStrEqual(requiredExtensionName, availableExtensionName))
			{
				extensionFound = CU_TRUE;
				break;
			}
		}

		CU_ASSERT_MSG(extensionFound, "Required Vulkan instance extension '%s' not found.", requiredExtensionName);
		CU_LOG_DEBUG("Vulkan instance extension '%s' found.", requiredExtensionName);
	}

	CU_PLATFORM_API(cuFree)(pAvailableExtensions, sizeof(VkExtensionProperties) * extensionsCount);
}

static void assertRequiredVulkanLayers()
{
	u32 layersCount = 0;
	vkEnumerateInstanceLayerProperties(&layersCount, CU_NULL);

	VkLayerProperties* pAvailableLayers = CU_PLATFORM_API(cuAllocate)(sizeof(VkLayerProperties) * layersCount);
	vkEnumerateInstanceLayerProperties(&layersCount, pAvailableLayers);

	u32 instanceRequiredLayersCount = CU_ARRAY_SIZE(gVulkanInstanceLayers);

	for (u32 requiredLayerIndex = 0u; requiredLayerIndex < instanceRequiredLayersCount; ++requiredLayerIndex)
	{
		b8			layerFound		  = CU_FALSE;
		const char* requiredLayerName = gVulkanInstanceLayers[requiredLayerIndex];

		for (u32 availableLayerIndex = 0u; availableLayerIndex < layersCount; ++availableLayerIndex)
		{
			const char* availableLayerName = pAvailableLayers[availableLayerIndex].layerName;

			if (cuStrEqual(requiredLayerName, availableLayerName))
			{
				layerFound = CU_TRUE;
				break;
			}
		}

		CU_ASSERT_MSG(layerFound, "Required Vulkan instance layer '%s' not found.", requiredLayerName);
		CU_LOG_DEBUG("Vulkan instance layer '%s' found.", requiredLayerName);
	}

	CU_PLATFORM_API(cuFree)(pAvailableLayers, sizeof(VkLayerProperties) * layersCount);
}

static u32 getPhysicalDeviceScore(VkPhysicalDevice physicalDevice);

static void choosePhysicalDevice()
{
	CU_ASSERT(gCuVulkanContext.instance != VK_NULL_HANDLE);
	CU_ASSERT(gCuVulkanContext.physicalDevice == VK_NULL_HANDLE);

	u32 physicalDevicesCount = 0;
	VK_ASSERT(vkEnumeratePhysicalDevices(gCuVulkanContext.instance, &physicalDevicesCount, CU_NULL));

	VkPhysicalDevice* physicalDevices =
		(VkPhysicalDevice*)CU_PLATFORM_API(cuAllocate)(sizeof(VkPhysicalDevice) * physicalDevicesCount);
	VK_ASSERT(vkEnumeratePhysicalDevices(gCuVulkanContext.instance, &physicalDevicesCount, physicalDevices));

	i32 bestDeviceIndex = -1;
	u32 bestDeviceScore = 0;

	for (i32 deviceIndex = 0; deviceIndex < physicalDevicesCount; ++deviceIndex)
	{
		u32 deviceScore = getPhysicalDeviceScore(physicalDevices[deviceIndex]);

		if (deviceScore > bestDeviceScore)
		{
			bestDeviceScore = deviceScore;
			bestDeviceIndex = deviceIndex;
		}
	}

	CU_ASSERT_MSG(bestDeviceIndex != -1, "Failed to find a suitable Vulkan physical device.");
	CU_ASSERT_MSG(bestDeviceScore > 0, "Failed to find a suitable Vulkan physical device.");
	gCuVulkanContext.physicalDevice = physicalDevices[bestDeviceIndex];

	CU_PLATFORM_API(cuFree)(physicalDevices, sizeof(VkPhysicalDevice) * physicalDevicesCount);
}

static u32 getPhysicalDeviceScore(VkPhysicalDevice physicalDevice)
{
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	u32 finalScore = 0;

	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		finalScore += 100;
	}
	else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
	{
		finalScore += 50;
	}
	else
	{
		return 0; // Other device types are not suitable
	}

	finalScore += properties.limits.maxImageDimension2D;

	return finalScore;
}

static void chooseGraphicsQueueFamily()
{
	CU_ASSERT(gCuVulkanContext.instance != VK_NULL_HANDLE);
	CU_ASSERT(gCuVulkanContext.physicalDevice != VK_NULL_HANDLE);

	u32 queueFamiliesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(gCuVulkanContext.physicalDevice, &queueFamiliesCount, CU_NULL);

	VkQueueFamilyProperties* queueFamiliesProperties =
		(VkQueueFamilyProperties*)CU_PLATFORM_API(cuAllocate)(sizeof(VkQueueFamilyProperties) * queueFamiliesCount);

	for (u32 queueFamilyIndex = 0; queueFamilyIndex < queueFamiliesCount; ++queueFamilyIndex)
	{
		VkQueueFamilyProperties properties = queueFamiliesProperties[queueFamilyIndex];

		if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			gCuVulkanContext.graphicsFamily.familyFound = CU_TRUE;
			gCuVulkanContext.graphicsFamily.familyIndex = queueFamilyIndex;
			break;
		}
	}

	CU_PLATFORM_API(cuFree)(queueFamiliesProperties, sizeof(VkQueueFamilyProperties) * queueFamiliesCount);
}

static void destroyMainVulkanSurface(void* pUserData);
static void createMainVulkanSurface()
{
	CU_ASSERT(gCuVulkanContext.instance != VK_NULL_HANDLE);
	CU_ASSERT(g_pMainWindow != CU_NULL);

	g_pMainWindow->surface =
		CU_PLATFORM_API(cuWindowCreateVulkanSurface)(g_pMainWindow->pWindow, gCuVulkanContext.instance);
	CU_LOG_DEBUG("Vulkan surface for main window created successfully.");
	cuReleaseStackPush(g_pReleaseStack, destroyMainVulkanSurface, CU_NULL);
}

static void destroyMainVulkanSurface(void* pUserData)
{
	CU_UNUSED(pUserData);
	CU_ASSERT(gCuVulkanContext.instance != VK_NULL_HANDLE);
	CU_ASSERT(g_pMainWindow != CU_NULL);
	CU_ASSERT(g_pMainWindow->surface != VK_NULL_HANDLE);

	CU_PLATFORM_API(cuWindowDestroyVulkanSurface)
	(g_pMainWindow->pWindow, gCuVulkanContext.instance, g_pMainWindow->surface);

	g_pMainWindow->surface = VK_NULL_HANDLE;

	CU_LOG_DEBUG("Vulkan surface for main window destroyed successfully.");
}

static void choosePresentQueueFamily()
{
	CU_ASSERT(gCuVulkanContext.instance != VK_NULL_HANDLE);
	CU_ASSERT(gCuVulkanContext.physicalDevice != VK_NULL_HANDLE);
	CU_ASSERT(g_pMainWindow != CU_NULL);

	u32 queueFamilyIndicesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(gCuVulkanContext.physicalDevice, &queueFamilyIndicesCount, CU_NULL);

	VkQueueFamilyProperties* queueFamiliesProperties = (VkQueueFamilyProperties*)CU_PLATFORM_API(cuAllocate)(
		sizeof(VkQueueFamilyProperties) * queueFamilyIndicesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(
		gCuVulkanContext.physicalDevice, &queueFamilyIndicesCount, queueFamiliesProperties);

	for (u32 queueFamilyIndex = 0; queueFamilyIndex < queueFamilyIndicesCount; ++queueFamilyIndex)
	{
		VkBool32 presentSupport = VK_FALSE;
		VK_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(
			gCuVulkanContext.physicalDevice, queueFamilyIndex, g_pMainWindow->surface, &presentSupport));

		if (presentSupport)
		{
			gCuVulkanContext.presentFamily.familyFound = CU_TRUE;
			gCuVulkanContext.presentFamily.familyIndex = queueFamilyIndex;
			break;
		}
	}

	CU_PLATFORM_API(cuFree)(queueFamiliesProperties, sizeof(VkQueueFamilyProperties) * queueFamilyIndicesCount);
}

static void destroyLogicalDevice(void* pUserData);
static void createLogicalDevice()
{
	CU_ASSERT(gCuVulkanContext.physicalDevice != VK_NULL_HANDLE);
	CU_ASSERT(gCuVulkanContext.device == VK_NULL_HANDLE);
	CU_ASSERT(gCuVulkanContext.graphicsFamily.familyFound == CU_TRUE);
	CU_ASSERT(gCuVulkanContext.presentFamily.familyFound == CU_TRUE);

	u32						numberQueueCreateInfos = 0;
	VkDeviceQueueCreateInfo queueCreateInfos[2]	   = {};
	float					queuePriority		   = 1.0f;

	queueCreateInfos[0].sType			 = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfos[0].pNext			 = CU_NULL;
	queueCreateInfos[0].queueFamilyIndex = gCuVulkanContext.graphicsFamily.familyIndex;
	queueCreateInfos[0].queueCount		 = 1;
	queueCreateInfos[0].pQueuePriorities = &queuePriority;
	numberQueueCreateInfos++;

	if (gCuVulkanContext.presentFamily.familyIndex != gCuVulkanContext.graphicsFamily.familyIndex)
	{
		queueCreateInfos[1].sType			 = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfos[1].pNext			 = CU_NULL;
		queueCreateInfos[1].queueFamilyIndex = gCuVulkanContext.presentFamily.familyIndex;
		queueCreateInfos[1].queueCount		 = 1;
		queueCreateInfos[1].pQueuePriorities = &queuePriority;
		numberQueueCreateInfos++;
	}

	VkDeviceCreateInfo deviceCreateInfo		 = {};
	deviceCreateInfo.sType					 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext					 = CU_NULL;
	deviceCreateInfo.enabledExtensionCount	 = CU_ARRAY_SIZE(gVulkanDeviceExtensions);
	deviceCreateInfo.ppEnabledExtensionNames = gVulkanDeviceExtensions;
	deviceCreateInfo.queueCreateInfoCount	 = numberQueueCreateInfos;
	deviceCreateInfo.pQueueCreateInfos		 = queueCreateInfos;

	VK_ASSERT(vkCreateDevice(gCuVulkanContext.physicalDevice, &deviceCreateInfo, CU_NULL, &gCuVulkanContext.device));
	CU_LOG_DEBUG("Vulkan logical device created successfully.");
	cuReleaseStackPush(g_pReleaseStack, destroyLogicalDevice, CU_NULL);
}

static void destroyLogicalDevice(void* pUserData)
{
	CU_UNUSED(pUserData);
	CU_ASSERT(gCuVulkanContext.device != VK_NULL_HANDLE);

	vkDestroyDevice(gCuVulkanContext.device, CU_NULL);
	gCuVulkanContext.device = VK_NULL_HANDLE;

	CU_LOG_DEBUG("Vulkan logical device destroyed successfully.");
}

#endif // CU_USE_VULKAN