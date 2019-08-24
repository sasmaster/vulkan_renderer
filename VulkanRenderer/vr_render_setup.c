#include "vr_render_setup.h"




static const char* validationLayersStr[1] = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = true;
#endif

//Every value of the array must be deallocated,then Returned pointer must be deallocated with sb_free
static VKEXtension** GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	VKEXtension** extensions = NULL;
	for (uint32_t i = 0; i < glfwExtensionCount; i++)
	{
		VKEXtension* ext = malloc(sizeof(VKEXtension));
		snprintf(ext->extName, sizeof(ext->extName), "%s", glfwExtensions[i]);
		sb_push(extensions, ext);
	}
	//add this extension in debug mode only
	if (enableValidationLayers)
	{
		VKEXtension* ext = malloc(sizeof(VKEXtension));
		snprintf(ext->extName, sizeof(ext->extName), "%s", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		sb_push(extensions, ext);
	}


	return extensions;
}

static bool CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, NULL);

	VkLayerProperties* availableLayers = NULL;
	sb_add(availableLayers, (int)layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

	const uint32_t numValidationLayers = arrayLength(validationLayersStr);
	for (uint32_t i = 0; i < numValidationLayers; i++)
	{
		const char* layerName = validationLayersStr[i];
		int layerFound = 0;

		const uint32_t numAvailableLayers = sb_count(availableLayers);
		for (uint32_t c = 0; c < numAvailableLayers; c++)
		{
			if (strcmp(layerName, availableLayers[c].layerName) == 0)
			{
				layerFound = 1;
				break;
			}
		}

		if (!layerFound)
		{
			sb_free(availableLayers);
			return false;
		}
	}



	sb_free(availableLayers);

	return true;
}


bool CreateVKInstance(VkInstance* instanceOut)
{
	VkApplicationInfo appInfo =
	{
	 .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	 .pApplicationName = "VulkanEngine",
	 .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
	 .pEngineName = "Engine",
	 .engineVersion = VK_MAKE_VERSION(1, 0, 0),
	 .apiVersion = VK_API_VERSION_1_0,
	};

	VkInstanceCreateInfo createInfo =
	{
	 .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	 .pApplicationInfo = &appInfo
	};

	// Init extension strings //
	VKEXtension** extensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = sb_count(extensions);
	createInfo.ppEnabledExtensionNames = (const char *const*)extensions;

	if (enableValidationLayers)
	{
		if (!CheckValidationLayerSupport())
		{
			printf("Some of requirested validation layers are not supported\n");
			return false;
		}
		createInfo.enabledLayerCount = arrayLength(validationLayersStr);
		createInfo.ppEnabledLayerNames = validationLayersStr;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	uint32_t instanceExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &instanceExtensionCount, NULL);
	printf("%i extensions supported\n", instanceExtensionCount);

	VkExtensionProperties* extensionProps = NULL;
	sb_add(extensionProps, (int)instanceExtensionCount);

	vkEnumerateInstanceExtensionProperties(NULL, &instanceExtensionCount, extensionProps);

	for (uint32_t i = 0; i < instanceExtensionCount; i++)
	{
		printf("%s\n", extensionProps[i].extensionName);
	}
	sb_free(extensionProps);
	if (vkCreateInstance(&createInfo, NULL, instanceOut) != VK_SUCCESS)
	{
		printf("failed to create instance!\n");
		return 0;
	}

	//free dynamic stuff //

	for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++)
	{
		free(extensions[i]);
	}
	sb_free(extensions);
	return true;
}


