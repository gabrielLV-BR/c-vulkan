#if !defined(__vulkan_utils_h__)
#define __vulkan_utils_h__

#include "types.h"

#include <vulkan/vulkan.h>

#define VKASSERT(x) ((bool)((x) == VK_SUCCESS))

extern const char *VALIDATION_LAYERS[];
extern const int VALIDATION_LAYERS_COUNT;

extern const VkQueueFlagBits *QUEUE_FAMILIES[];
extern const int QUEUE_FAMILIES_COUNT;

void _vulkan_print_extensions();

bool _vulkan_check_validation_layers();

const char** _vulkan_list_required_validation_layers(int *validation_layers_count);
const char** _vulkan_list_required_extensions(int *extensions_count);

int _vulkan_get_physical_device_suitability_score(
    VkPhysicalDevice physical_device
);

int _vulkan_physical_device_find_suitable_queue_family_index(
    VkPhysicalDevice physical_device,
    VkQueueFlagBits queue_flag_bit
);

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

VKAPI_ATTR VkBool32 VKAPI_CALL _vulkan_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
);

#endif // __vulkan_utils_h__
