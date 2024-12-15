#include <vulkan/vulkan.h>

#include "types.h"

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

    free(physical_devices);

    if (!most_suitable_device_score) {
        ERROR("No sufficiently suitable device found");
        return false;
    }

    return true;
}