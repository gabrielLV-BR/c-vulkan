#if !defined(__renderer_h__)
#define __renderer_h__

#include "window.h"
#include "backend/vulkan_backend.h"

typedef struct renderer_t renderer;

int renderer_init(window *window, renderer *renderer);

void renderer_destroy(renderer *renderer);

#endif // __renderer_h__
