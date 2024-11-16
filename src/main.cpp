#include <iostream>

#define GLAD_GL_IMPLEMENTATION

#include "glad/gl.h"
#include "GLFW/glfw3.h"

int main() {
	if (glfwInit() == GLFW_FALSE) {
		std::cerr << "Erro ao inicializar GLFW" << std::endl;
		return -1;
	}

	GLFWwindow *window = glfwCreateWindow(500, 500, "Hello", NULL, NULL);

	if (window == nullptr) {
		std::cerr << "Erro ao criar janela" << std::endl;
		return -1;
	}

	std::cout << "Inicializado com sucesso" << std::endl;

	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cerr << "Erro ao carregar OpenGl" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}
