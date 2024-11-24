#include <stdlib.h>

#include "renderer.h"

#include "backend/vulkan_backend.h"

struct renderer_t {
    vulkan_backend *backend
};

renderer* renderer_new() {
    return malloc(sizeof(renderer));
}

bool renderer_init(window *window, renderer *renderer) {
    renderer->backend = vulkan_backend_new();
 
    return vulkan_backend_init(renderer->backend); 
}

void renderer_destroy(renderer *renderer) {}