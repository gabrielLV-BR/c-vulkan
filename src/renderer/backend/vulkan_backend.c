#include <stdlib.h>

#include "info.h"
#include "log.h"
#include "window.h"
#include "structs/slice.h"

#include "vulkan_backend.h"
#include "vulkan_utils.h"

#include <vulkan/vulkan.h>

struct vulkan_backend_t {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VkDevice device;
};

vulkan_backend* vulkan_backend_new() {
    return malloc(sizeof(vulkan_backend));
}

static VkApplicationInfo _vulkan_create_application_info() {
    VkApplicationInfo application_info = {0};
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.apiVersion = VK_API_VERSION_1_3;

    application_info.pApplicationName = APP_NAME;
    application_info.applicationVersion = 
        VK_MAKE_VERSION(APP_VERSION.minor, APP_VERSION.minor, APP_VERSION.fix);

    application_info.pEngineName = ENGINE_NAME;
    application_info.engineVersion = 
        VK_MAKE_VERSION(ENGINE_VERSION.minor, ENGINE_VERSION.minor, ENGINE_VERSION.fix);

    return application_info;
}

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

bool __vulkan_backend_create_instance(VkInstance *instance) {
    VkApplicationInfo application_info = _vulkan_create_application_info();
    
    VkInstanceCreateInfo instance_create_info = {0};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &application_info;

    uint enabled_extension_count;
    instance_create_info.ppEnabledExtensionNames = window_get_required_extensions(&enabled_extension_count);
    instance_create_info.enabledExtensionCount = enabled_extension_count;
    
    instance_create_info.enabledLayerCount = 0;

#ifndef NDEBUG
    if (!_vulkan_add_validation_layers(&instance_create_info)) {
        ERROR("Error adding Vulkan validation layers");
        return false;
    }

    if (!_vulkan_add_debug_messaging_extension(&instance_create_info)) {
        ERROR("Error adding Vulkan debug messaging extensions");
        return false;
    }
#endif

#if PLATFORM_APPLE
    //TODO implement str_append
    instance_create_info.ppEnabledExtensionNames = 
        str_append(instance_create_info.ppEnabledExtensionNames, 
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, 
            instance_create_info.enabledExtensionCount);

    instance_create_info.enabledExtensionCount += 1;
    instance_create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

#ifndef NDEBUG
    _vulkan_print_extensions();
#endif
    
    VkResult result = vkCreateInstance(&instance_create_info, NULL, instance);

#if PLATFORM_APPLE
    free(instance_create_info.ppEnabledExtensionNames);
#endif

    return VKASSERT(result);
}

bool __vulkan_backend_find_physical_device(
    VkInstance instance,
    VkPhysicalDevice *physical_device
) {
    int i, device_score, most_suitable_device_score = 0;
    uint physical_device_count;
    VkPhysicalDevice *physical_devices;
    
    vkEnumeratePhysicalDevices(instance, &physical_device_count, NULL);

    if (physical_device_count == 0) {
        ERROR("No physical devices were found");
        return false;
    }

    physical_devices = calloc(physical_device_count, sizeof(VkPhysicalDevice));

    vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices);

    for (i = 0; i < physical_device_count; i++) {
        device_score = _vulkan_get_physical_device_suitability_score(physical_devices[i]);

        if (device_score > most_suitable_device_score) {
            most_suitable_device_score = device_score;
            *physical_device = physical_devices[i];
        }
    }

    if (!most_suitable_device_score) {
        ERROR("No sufficiently suitable device found");
        return false;
    }

    return true;
}

bool vulkan_backend_init(vulkan_backend *backend) {
    if (!__vulkan_backend_create_instance(&backend->instance)) {
        ERROR("Error creating Vulkan instance");
        return false;
    }

#ifndef NDEBUG
    if (!__vulkan_backend_create_debug_callback(backend->instance, &backend->debug_messenger)) {
        ERROR("Error creating debug callback messenger");
        return false;
    }
#endif

    if (__vulkan_backend_find_physical_device(backend->instance, &backend->physical_device)) {
        ERROR("Error finding physical devices");
        return false;
    }

    return true;
}

void vulkan_backend_destroy(vulkan_backend *backend) {
#ifndef NDEBUG
    _vulkan_destroy_debug_utils_messenger(backend->instance, backend->debug_messenger, NULL);
#endif

    vkDestroyInstance(backend->instance, NULL);

    INFO("Vulkan instance destroyed successfully");
}