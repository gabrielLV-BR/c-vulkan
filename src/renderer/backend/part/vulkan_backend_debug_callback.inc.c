#include <vulkan/vulkan.h>

#include "renderer/backend/vulkan_utils.h"

bool __vulkan_backend_create_debug_callback(VkInstance instance, VkDebugUtilsMessengerEXT *debug_messenger) {
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {0};

    debug_messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_info.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_messenger_info.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_messenger_info.pfnUserCallback = _vulkan_debug_callback;
    debug_messenger_info.pUserData = NULL;

    return _vulkan_create_debug_utils_messenger(instance, &debug_messenger_info, NULL, debug_messenger);
}