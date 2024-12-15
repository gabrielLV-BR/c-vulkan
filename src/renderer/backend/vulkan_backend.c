#include <stdlib.h>

#include "log.h"
#include "window.h"
#include "utils/strings.h"

#include "vulkan_backend.h"
#include "vulkan_utils.h"

#include <vulkan/vulkan.h>

#include "part/vulkan_backend_instance.inc.c"
#include "part/vulkan_backend_debug_callback.inc.c"
#include "part/vulkan_backend_physical_device.inc.c"
#include "part/vulkan_backend_logical_device.inc.c"

struct vulkan_backend_t {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VkDevice device;
};

vulkan_backend* vulkan_backend_new() {
    return malloc(sizeof(vulkan_backend));
}

bool vulkan_backend_init(vulkan_backend *backend) {
    int extension_count;
    const char **extensions = _vulkan_list_required_extensions(&extension_count);

    int layer_count;
    const char **layers = _vulkan_list_required_validation_layers(&layer_count);

    if (!__vulkan_backend_create_instance(
            &backend->instance, 
            extensions, extension_count, 
            layers, layer_count)
    ) {
        ERROR("Error creating Vulkan instance");
        return false;
    }

#ifndef NDEBUG
    if (!__vulkan_backend_create_debug_callback(backend->instance, &backend->debug_messenger)) {
        ERROR("Error creating debug callback messenger");
        return false;
    }
#endif

    if (!__vulkan_backend_find_physical_device(backend->instance, &backend->physical_device)) {
        ERROR("Error finding physical devices");
        return false;
    }

    if (!__vulkan_backend_create_logical_device(
            backend->instance, 
            backend->physical_device,
            &backend->device,
            layers,
            layer_count)
    ) {
        ERROR("Error creating logical device");
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