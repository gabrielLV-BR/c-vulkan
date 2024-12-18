#include <stdlib.h>

#include <vulkan/vulkan.h>

#include "types.h"

#include "renderer/backend/vulkan_utils.h"

bool __vulkan_backend_create_logical_device(
    VkInstance instance,
    VkPhysicalDevice physical_device,
    VkDevice *device,
    const char **validation_layers,
    int validation_layer_count
) {
    float queue_priority = 1.0f;

    VkDeviceCreateInfo device_create_info = {0};
    VkDeviceQueueCreateInfo *queue_create_infos;

    // kept empty as we're not using any device extension, yet
    VkPhysicalDeviceFeatures physical_device_features = {0};

    queue_create_infos = calloc(sizeof (VkDeviceQueueCreateInfo), QUEUE_FAMILIES_COUNT);

    for (int i = 0; i < QUEUE_FAMILIES_COUNT; i++) {
        int queue_index = _vulkan_physical_device_find_queue_family_index(
            physical_device,
            QUEUE_FAMILIES[i]
        );

        if (queue_index == -1) {
            ERROR("Error finding index of required family queue on logical device creation");
            return false;
        }

        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].queueCount = 1;
        queue_create_infos[i].queueFamilyIndex = (uint) queue_index;
        queue_create_infos[i].pQueuePriorities = &queue_priority;
    }

    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = queue_create_infos;
    device_create_info.queueCreateInfoCount = QUEUE_FAMILIES_COUNT;
    device_create_info.pEnabledFeatures = &physical_device_features;
    device_create_info.ppEnabledLayerNames = validation_layers;
    device_create_info.enabledLayerCount = validation_layer_count;

    VkResult result = vkCreateDevice(physical_device, &device_create_info, NULL, device);

    return VKASSERT(result);
}