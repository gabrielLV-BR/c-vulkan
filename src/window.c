#include <stdlib.h>

#include "window.h"
#include "log.h"
#include "types.h"

struct window_t {
    GLFWwindow *handle;
    const char *title;
    uint width, height;
};

window* window_new() {
    return malloc(sizeof(window));
}

bool window_init(window *window, uint width, uint height, const char *title) {
    if (!glfwInit()) {
        ERROR("Error initializing GLFW");
        return false;
    }
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window->handle = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window->handle == NULL) {
        ERROR("Error creating window");
        return false;
    }

    return true;
}

void window_destroy(window *window) {
    glfwDestroyWindow(window->handle);
    glfwTerminate();
}

bool window_running(window *window) {
    return !glfwWindowShouldClose(window->handle);
}

void window_update(window *window) {
    glfwPollEvents();
    glfwSwapBuffers(window->handle);
}

const char** window_get_required_extensions(uint *count) {
    return glfwGetRequiredInstanceExtensions(count);
}