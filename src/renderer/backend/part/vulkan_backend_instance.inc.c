#include <vulkan/vulkan.h>

#include "info.h"
#include "types.h"

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

bool __vulkan_backend_create_instance(
    VkInstance *instance,
    const char **extensions,
    int extension_count,
    const char **layers,
    int layer_count
) {
    VkApplicationInfo application_info = _vulkan_create_application_info();
    
    VkInstanceCreateInfo instance_create_info = {0};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &application_info;

    uint enabled_extension_count;
    instance_create_info.ppEnabledExtensionNames = extensions;
    instance_create_info.enabledExtensionCount = extension_count;
    
    instance_create_info.ppEnabledLayerNames = layers;
    instance_create_info.enabledLayerCount = layer_count;

#if PLATFORM_APPLE
    instance_create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

    VkResult result = vkCreateInstance(&instance_create_info, NULL, instance);

    return VKASSERT(result);
}