#if CU_USE_VULKAN

#include "cju/modules/release_stack.h"
#include "common.h"

static CuWindow*	   g_pMainWindow	= CU_NULL; /// currently has only one window support
static CuReleaseStack* g_pReleaseStack	= CU_NULL;
CuVulkanContext		   gCuVulkanContext = {}; /// the global vulkan context

// ======================== Vulkan Helper Functions ========================
static void createVulkanInstance();

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

	g_pMainWindow = CU_PLATFORM_API(cuWindowCreate)(800, 600, "C-JS-UI Renderer Window");

	cuReleaseStackPush(g_pReleaseStack, shutdownWindow, CU_NULL);

	// Setup Vulkan
	{
		createVulkanInstance();
	}
}

b8 cuRendererShouldClose()
{
	return !g_pMainWindow->isRunning;
}

void cuRendererBeginFrame()
{
	CU_PLATFORM_API(cuWindowPollEvents)(g_pMainWindow);
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

	CU_PLATFORM_API(cuWindowDestroy)(g_pMainWindow);
	g_pMainWindow = CU_NULL;

	CU_PLATFORM_API(cuWindowShutdown)();
}

#endif // CU_USE_VULKAN