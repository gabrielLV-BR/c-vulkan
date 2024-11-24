#include <stdio.h>

#include "renderer/renderer.h"
#include "window.h"
#include "log.h"

#define FAULT 1
#define OK 0

int main() {
	window *window = window_new();
	renderer *renderer = renderer_new();

	INFO("HI");

	if (!window_init(window, 500, 500, "Hello\0")) {
		ERROR("Error initializing window");
		return FAULT;
	}

	if (!renderer_init(window, renderer)) {
		ERROR("Error initializing renderer");
		return FAULT;
	}

	while (window_running(window)) {
		window_update(window);
	}

	window_destroy(window);

	return OK;
}
