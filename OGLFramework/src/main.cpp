#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "imgui.h"

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "Lol glfw is not ok" << std::endl;
		return 0;
	}

	window = glfwCreateWindow(1280, 720, "OGLFramework", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		std::cout << "Lol glad is not ok" << std::endl;
		return 0;
	}

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);

		glClear(GL_COLOR_BUFFER_BIT);
	}

	return 0;
}