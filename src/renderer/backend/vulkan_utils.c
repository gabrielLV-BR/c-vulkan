#include <stdlib.h>

#include "log.h"

#include "vulkan_utils.h"

#include <vulkan/vulkan.h>

static const char *VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

void _vulkan_print_extensions() {
    int i;
    uint extension_count;
    VkExtensionProperties *extensions;

    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);

    extensions = calloc(sizeof(VkExtensionProperties), extension_count);

    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);

    for (i = 0; i < extension_count; i++) {
        DEBUG("Extension name: %s\n", extensions[i].extensionName);
    }
}

bool _vulkan_check_validation_layer_support() {
    uint layer_count;

}

bool _vulkan_add_validation_layers(VkInstanceCreateInfo *instance_create_info) {

    

}