#include "utils/ImGui/imgui_impl_opengl3.h"
#include "utils/ImGui/imgui_impl_glfw.h"

#include "GLFW/glfw3.h"
#include <GLAD/glad.h>

#include "spdlog/spdlog.h"

#include "utils/OpenGL/Shader.h"

#define MAX_VAO_COUNT 20

GLuint vaoArr[MAX_VAO_COUNT];
uint32_t vaoCount = 0;

inline void imgui_init(GLFWwindow*);
inline void imgui_start();
inline void imgui_end();

bool InitLibs();
bool InitBuffers();

GLFWwindow* m_Window;

float vertices[] = {
	-0.5f, -0.5f, 0.5f,
	-0.5f,	0.5f, 0.5f,
	 0.5f,	0.5f, 0.5f,
	 0.5f, -0.5f, 0.5f,
};

uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

int main()
{
	InitLibs();
	InitBuffers();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	OGLFramework::Shader shader;
	shader.Compile("assets/shaders/test.vert.glsl", "assets/shaders/test.frag.glsl");

	while (!glfwWindowShouldClose(m_Window))
	{
		{//Clear
			glClear(GL_COLOR_BUFFER_BIT); 
		}

		{//Render 

			shader.Use();
			glBindVertexArray(vaoArr[vaoCount -1]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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

bool InitLibs()
{
	if (!glfwInit())
	{
		spdlog::critical("Failed to initialize GLFW");
		return 0;
	}
	m_Window = glfwCreateWindow(1000, 1000, "window", nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGL())
	{
		spdlog::critical("Failed to initialize GLAD");
	}

	imgui_init(m_Window);
}

bool InitBuffers()
{
	if (vaoCount < MAX_VAO_COUNT)
	{
		GLuint vbo, ebo;
		glCreateVertexArrays(1, &vaoArr[vaoCount]);
		glBindVertexArray(vaoArr[vaoCount++]);

		glCreateBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glCreateBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		glBindVertexArray(0);
		return 0;
	}

	else
	{
		spdlog::info("Max count for VAO = {0}", MAX_VAO_COUNT);
		return 1;
	}
}