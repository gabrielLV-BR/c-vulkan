#if !defined(__window_h__)
#define __window_h__

#include "GLFW/glfw3.h"

#include "types.h"

typedef struct window_t window;

bool window_init(window *window, uint width, uint height, const char *title);
void window_destroy(window *window);

bool window_running(window *window);
void window_update(window *window);

#endif // __window_h__
