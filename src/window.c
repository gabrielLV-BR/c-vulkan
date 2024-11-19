#include "window.h"
#include "log.h"
#include "types.h"

struct window_t {
    GLFWwindow *handle;
    const char *title;
    uint width, height;
};

bool window_init(window *window, uint width, uint height, const char *title) {
    if (!glfwInit()) {
        ERROR("Error initializing GLFW");
        return false;
    }
    
    window->handle = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window->handle == NULL) {
        ERROR("Error creating window");
        return false;
    }

    return true;
}

void window_destroy(window *window) {
    glfwDestroyWindow(window->handle);
}

bool window_running(window *window) {
    return !glfwWindowShouldClose(window->handle);
}

void window_update(window *window) {
    glfwPollEvents();
    glfwSwapBuffers(window->handle);
}