#include <stdlib.h>

#include "info.h"

#include "window.h"
#include "structs/slice.h"

#include "vulkan_backend.h"
#include "vulkan_utils.h"

#include <vulkan/vulkan.h>

struct vulkan_backend_t {
    VkInstance instance;
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



bool vulkan_backend_init(vulkan_backend *backend) {
    VkApplicationInfo application_info = _vulkan_create_application_info();
    
    VkInstanceCreateInfo instance_create_info = {0};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &application_info;

    uint enabled_extension_count;
    instance_create_info.ppEnabledExtensionNames = window_get_required_extensions(&enabled_extension_count);
    instance_create_info.enabledExtensionCount = enabled_extension_count;

    instance_create_info.enabledLayerCount = 0;

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

    VkResult result = vkCreateInstance(&instance_create_info, NULL, &backend->instance);

#if PLATFORM_APPLE
    free(instance_create_info.ppEnabledExtensionNames);
#endif

    return VKASSERT(result);
}

void vulkan_backend_destroy(vulkan_backend *backend) {
    vkDestroyInstance(backend->instance, NULL);
}