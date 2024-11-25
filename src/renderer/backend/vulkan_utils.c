#include <stdlib.h>
#include <string.h>

#include "log.h"

#include "vulkan_utils.h"

#include <vulkan/vulkan.h>

const char *VALIDATION_LAYERS[] = { 
    "VK_LAYER_KHRONOS_validation" 
};
const int VALIDATION_LAYERS_COUNT = 1;

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

bool __vulkan_check_validation_layer_support(uint layer_count, VkLayerProperties *available_layers) {
    for (int i = 0; i < VALIDATION_LAYERS_COUNT; i++) {
        for (int j = 0; j < layer_count; j++) {
            if (strcmp(available_layers[j].layerName, VALIDATION_LAYERS[i]) == 0) {
                goto NEXT;
            }
        }

        return false;
NEXT:
        DEBUG("Adding validation layer %s", VALIDATION_LAYERS[i]);
    }

    return true;
}

bool _vulkan_add_validation_layers(VkInstanceCreateInfo *instance_create_info) {
    uint layer_count;
    VkLayerProperties *layers;

    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    layers = calloc(layer_count, sizeof(VkLayerProperties));

    vkEnumerateInstanceLayerProperties(&layer_count, layers);

    if (!__vulkan_check_validation_layer_support(layer_count, layers)) {
        ERROR("Requested validation layers are not supported");
        return false;
    }

    instance_create_info->ppEnabledLayerNames = VALIDATION_LAYERS;
    instance_create_info->enabledLayerCount = VALIDATION_LAYERS_COUNT;

    INFO("Validation layers were added succesfully");

    return true;
}

