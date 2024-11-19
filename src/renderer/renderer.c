#include "renderer.h"

#include "backend/vulkan_backend.h"

struct renderer_t {
    vulkan_backend *backend
};

int renderer_init(window *window, renderer *renderer) {
    
}

void renderer_destroy(renderer *renderer) {}