#if !defined(__vulkan_utils_h__)
#define __vulkan_utils_h__

#include "types.h"

#include <vulkan/vulkan.h>

#define VKASSERT(x) ((bool)((x) == VK_SUCCESS))

void _vulkan_print_extensions();

bool _vulkan_add_validation_layers(VkInstanceCreateInfo *instance_create_info); 

#endif // __vulkan_utils_h__
