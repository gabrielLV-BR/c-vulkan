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

    free(extensions);
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
        DEBUG("Adding validation layer %s\n", VALIDATION_LAYERS[i]);
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
        free(layers);

        ERROR("Requested validation layers are not supported");

        return false;
    }

    instance_create_info->ppEnabledLayerNames = VALIDATION_LAYERS;
    instance_create_info->enabledLayerCount = VALIDATION_LAYERS_COUNT;

    INFO("Validation layers were added succesfully");

    free(layers);

    return true;
}

void _vulkan_add_debug_messaging_extension(VkInstanceCreateInfo *instance_create_info) {
    int i;
    int debug_layer_name_length = strlen(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    int extension_count = instance_create_info->enabledExtensionCount + 1;
    char **extensions = calloc(extension_count, sizeof(char *));

    // copy existing layers
    for (i = 0; i < instance_create_info->enabledExtensionCount; i++) {
        int extension_name_length = strlen(instance_create_info->ppEnabledExtensionNames[i]);

        extensions[i] = calloc(extension_name_length, sizeof (char));

        strcpy(extensions[i], instance_create_info->ppEnabledExtensionNames[i]);
    }

    // add new layer

    extensions[i] = calloc(debug_layer_name_length, sizeof (char));

    strcpy(extensions[i], VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    instance_create_info->ppEnabledExtensionNames = extensions;
    instance_create_info->enabledExtensionCount = extension_count;
}

int _vulkan_physical_device_find_suitable_queue_family_index(
    VkPhysicalDevice physical_device, 
    VkQueueFlagBits queue_flag_bit
) {
    int i;
    uint queue_family_count;
    VkQueueFamilyProperties *queue_families;

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);

    queue_families = calloc(queue_family_count, sizeof(VkQueueFamilyProperties));

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families);

    for (i = 0; i < queue_family_count; i++) {
        if (queue_families[i].queueFlags & queue_flag_bit) {
            free(queue_families);
            return i;
        }
    }
    
    free(queue_families);

    INFO("No suitable queue family was found for provided flag");
    return -1;
}

int _vulkan_get_physical_device_suitability_score(VkPhysicalDevice physical_device) {
    int score = 0;
    int graphics_family_index;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;

    vkGetPhysicalDeviceProperties(physical_device, &properties);
    vkGetPhysicalDeviceFeatures(physical_device, &features);

    graphics_family_index = 
        _vulkan_physical_device_find_suitable_queue_family_index(
            physical_device,
            VK_QUEUE_GRAPHICS_BIT
        );

    // optional, but great :D
    score += (VK_VERSION_MINOR(properties.apiVersion) == 3) * 5;
    score += (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) * 10;

    // required
    score *= VK_VERSION_MINOR(properties.apiVersion) >= 2;
    score *= features.geometryShader;
    score *= graphics_family_index != -1;

    DEBUG("Device %s scored %d\n", properties.deviceName, score);

    return score;
}

// debug callback
bool _vulkan_create_debug_utils_messenger(
    VkInstance instance, 
    VkDebugUtilsMessengerCreateInfoEXT *create_info,
    VkAllocationCallbacks *allocator, 
    VkDebugUtilsMessengerEXT *debug_messenger
) {
    PFN_vkCreateDebugUtilsMessengerEXT pf_vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    //TODO this is erroring: fix it!
    if (pf_vkCreateDebugUtilsMessengerEXT == NULL) {
        ERROR("Error loading vkCreateDebugUtilsMessengerEXT function");
        return false;
    }

    return VKASSERT(pf_vkCreateDebugUtilsMessengerEXT(instance, create_info, NULL, debug_messenger));
}

bool _vulkan_destroy_debug_utils_messenger(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT debug_messenger,
    VkAllocationCallbacks *allocator
) {
    PFN_vkDestroyDebugUtilsMessengerEXT fn_vkDestroyDebugUtilsMessengerEXT =  
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (fn_vkDestroyDebugUtilsMessengerEXT == NULL) {
        ERROR("Error loading vkDestroyDebugUtilsMessengerEXT function");
        return false;
    }

    fn_vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, allocator);

    return true;
}


VKAPI_ATTR VkBool32 VKAPI_CALL _vulkan_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
) {
    if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        fprintf(stderr, callback_data->pMessage);
    }

    return VK_FALSE;
}