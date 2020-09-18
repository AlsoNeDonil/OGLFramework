#include "utils/imgui_impl_opengl3.h"
#include "utils/imgui_impl_glfw.h"
#include "imgui.h"

#include "GLFW/glfw3.h"
#include <GLAD/glad.h>

#include <iostream>

inline void imgui_init(GLFWwindow*);
inline void imgui_start();
inline void imgui_end();

bool Init();

GLFWwindow* m_Window;

int main()
{
	Init();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	while (!glfwWindowShouldClose(m_Window))
	{
		{//Clear
			glClear(GL_COLOR_BUFFER_BIT);
		}

		{//Render 
		
		}

		{//Render ImGui
			imgui_start();

			ImGui::Begin("Test");
			ImGui::Text("Test");
			ImGui::End();

			imgui_end();
		}

		{//Update
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}
	}
	return 0;
}

void imgui_init(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void imgui_start()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void imgui_end()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Init()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return 0;
	}
	m_Window = glfwCreateWindow(1000, 1000, "window", nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	imgui_init(m_Window);
}