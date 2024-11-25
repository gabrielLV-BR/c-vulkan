#if !defined(__vulkan_h__)
#define __vulkan_h__

#include "types.h"

typedef struct vulkan_backend_t vulkan_backend;

vulkan_backend* vulkan_backend_new();

bool vulkan_backend_init(vulkan_backend *backend);

/* TODO add in more initialization functions */

void vulkan_backend_destroy(vulkan_backend *backend);

#endif // __vulkan_h__
