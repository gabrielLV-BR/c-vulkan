#include <stdio.h>

#include "window.h"
#include "log.h"

int main() {
	window window = (struct window_t) {
		.width = 500,
		.height = 500,
		.title = "Hello"
	};

	if (!window_init(&window)) {
		ERROR("Error initializing window");
		return 1;
	}

	while (window_running(&window)) {
		window_update(&window);
	}

	window_destroy(&window);

	return 0;
}
