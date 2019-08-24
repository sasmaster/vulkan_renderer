#ifndef VR_RENDER_SETUP_H
#define VR_RENDER_SETUP_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include "glfw/glfw3.h"

#include "vulkan/vulkan.h"
#include "stretchy_buffer.h"


#define arrayLength(a) (sizeof((a))/sizeof((a)[0]))



#ifdef __cplusplus
extern "C" {
#endif

	typedef struct VKEXtension
	{
		char extName[256];

	}VKEXtension;

	 
	bool CreateVKInstance(VkInstance* instanceOut);

	bool DisposeVKInstance(VkInstance instance);

#ifdef __cplusplus
}
#endif


#endif // !VR_RENDER_SETUP_H
