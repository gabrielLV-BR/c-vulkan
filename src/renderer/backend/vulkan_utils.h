#if !defined(__vulkan_utils_h__)
#define __vulkan_utils_h__

#include "types.h"

#include <vulkan/vulkan.h>

#define VKASSERT(x) ((bool)((x) == VK_SUCCESS))

void _vulkan_print_extensions();

bool _vulkan_add_validation_layers(VkInstanceCreateInfo *instance_create_info); 
bool _vulkan_add_debug_messaging_extension(VkInstanceCreateInfo *instance_create_info);

// Vulkan debug callback

bool _vulkan_create_debug_utils_messenger(
    VkInstance instance,
    VkDebugUtilsMessengerCreateInfoEXT *create_info,
    VkAllocationCallbacks *allocator, 
    VkDebugUtilsMessengerEXT *debug_messenger);

bool _vulkan_destroy_debug_utils_messenger(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT debug_messenger,
    VkAllocationCallbacks *allocator
);

int _vulkan_get_physical_device_suitability_score(
    VkPhysicalDevice physical_device
);

VKAPI_ATTR VkBool32 VKAPI_CALL _vulkan_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
);

#endif // __vulkan_utils_h__
