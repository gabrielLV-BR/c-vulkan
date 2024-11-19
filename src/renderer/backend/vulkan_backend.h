#if !defined(__vulkan_h__)
#define __vulkan_h__

typedef struct vulkan_backend_t vulkan_backend;

int vulkan_backend_init(vulkan_backend *backend);

int vulkan_create_device(vulkan_backend *backend);
int vulkan_create_swapchain(vulkan_backend *backend);
/* TODO add in more initialization functions */

void vulkan_backend_destroy(vulkan_backend *backend);

#endif // __vulkan_h__
