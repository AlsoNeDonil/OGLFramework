#include "utils/ImGui/imgui_impl_opengl3.h"
#include "utils/ImGui/imgui_impl_glfw.h"

#include "GLFW/glfw3.h"
#include <GLAD/glad.h>

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "utils/OpenGL/Shader.h"
#include "utils/OpenGL/Camera.h"

#include "utils/Core/Data.h"

using namespace OGLFramework;

GLFWwindow* m_Window;
GLboolean keys[512];

inline void imgui_init(GLFWwindow*);
inline void imgui_start();
inline void imgui_end();

bool InitLibs();
void Input();
GLuint LoadCubeVertices();
GLuint LoadSphereVertices(uint32_t*);

std::shared_ptr<OGLFramework::Camera> m_MainCamera;

int main()
{
	InitLibs();
	imgui_init(m_Window);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	m_MainCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));
	Shader modelShader("assets/shaders/lightDefault.vert.glsl", "assets/shaders/lightDefault.frag.glsl");

	glm::mat4 projection(1.0f);
	glm::mat4 view(1.0f);

	uint32_t sphereIndexCount;
	GLuint figureVao = LoadSphereVertices(&sphereIndexCount);

	glm::mat4 model(1.0);

	while (!glfwWindowShouldClose(m_Window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		view = m_MainCamera->GetViewMatrix();
		projection = glm::perspective(m_MainCamera->m_Zoom, 1280.0f / 720.0f, 0.1f, 100.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(figureVao);

		glm::mat4 model(1.0f);

		modelShader.Use();
		modelShader.SetMat4("u_Model", model);
		modelShader.SetMat4("u_View", view);
		modelShader.SetMat4("u_Projection", projection);

		modelShader.SetVec3("u_Color", glm::vec3(0.8f, 0.5f, 0.2f));
		modelShader.SetVec3("u_ViewPos", m_MainCamera->m_Position);
		modelShader.SetVec3("u_LightPosition", lightPos);
		glDrawElements(GL_TRIANGLE_STRIP, sphereIndexCount, GL_UNSIGNED_INT, nullptr);

		imgui_start();
		{
			ImGui::Begin("Camera Info");

			auto vec = m_MainCamera->m_Position;
			ImGui::Text("Position");
			ImGui::Text("%3f, %3f, %3f", vec.x, vec.y, vec.z);
			ImGui::Separator();

			vec = m_MainCamera->m_Front;
			ImGui::Text("Front");
			ImGui::Text("%3f, %3f, %3f", vec.x, vec.y, vec.z);
			ImGui::Separator();

			vec = m_MainCamera->m_Right;
			ImGui::Text("Right");
			ImGui::Text("%3f, %3f, %3f", vec.x, vec.y, vec.z);
			ImGui::Separator();

			vec = m_MainCamera->m_Up;
			ImGui::Text("Up");
			ImGui::Text("%3f, %3f, %3f", vec.x, vec.y, vec.z);
			ImGui::Separator();

			ImGui::End();
		}
		imgui_end();


		Input();

		glfwPollEvents();
		glfwSwapBuffers(m_Window);

	}

	return 0;
}

void imgui_init(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, false);
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
	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "OGLFramework", nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);

	glfwSetCursorPos(m_Window, lastX, lastY);

	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
			if (key >= 0 && key < 1024)
			{
				if (action == GLFW_PRESS)
					keys[key] = true;
				else if (action == GLFW_RELEASE)
					keys[key] = false;
			}
		});
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) 
		{
			GLfloat xoffset = xpos - lastX;
			GLfloat yoffset = lastY - ypos;

			lastX = xpos;
			lastY = ypos;

			m_MainCamera->ProcessMouseMovement(xoffset, yoffset);
		});
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) 
		{
			glViewport(0, 0, width, height);
		});
	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) 
		{
			m_MainCamera->ProcessMouseScroll(yoffset * deltaTime);
		});

	if (!gladLoadGL())
	{
		spdlog::critical("Failed to initialize GLAD");
	}

	imgui_init(m_Window);
}

void Input()
{
	//movements
	if (keys[GLFW_KEY_W])
		m_MainCamera->ProcessKeyboard(FORWARD, deltaTime);
	else if (keys[GLFW_KEY_S])
		m_MainCamera->ProcessKeyboard(BACKWARD, deltaTime);

	if (keys[GLFW_KEY_A])
		m_MainCamera->ProcessKeyboard(LEFT, deltaTime);
	else if (keys[GLFW_KEY_D])
		m_MainCamera->ProcessKeyboard(RIGHT, deltaTime);

	if (keys[GLFW_KEY_SPACE])
		m_MainCamera->ProcessKeyboard(UP, deltaTime);
	else if (keys[GLFW_KEY_LEFT_SHIFT])
		m_MainCamera->ProcessKeyboard(DOWN, deltaTime);

	//system  
	if (keys[GLFW_KEY_LEFT_ALT])
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (keys[GLFW_KEY_RIGHT_ALT])
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

GLuint LoadCubeVertices()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	GLuint vao, vbo, ebo;

	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glCreateBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));

	glBindVertexArray(0);

	return vao;
}

GLuint LoadSphereVertices(uint32_t* indices)
{
	GLuint sphereVao;
	GLuint sphereVbo;
	GLuint sphereEbo;

	std::vector<glm::vec3> sphereVertices;
	std::vector<uint32_t> sphereIndices;

	const float RADIUS = 1.0f;
	const uint32_t X_SEGMENTS = 64;
	const uint32_t Y_SEGMENTS = 64;
	const float PI = glm::pi<float>();

	for (uint32_t y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (uint32_t x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;

			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			sphereVertices.push_back(glm::vec3(xPos, yPos, zPos));
			sphereVertices.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (size_t y = 0; y < Y_SEGMENTS; y++)
	{
		if (!oddRow)
		{
			for (size_t x = 0; x <= X_SEGMENTS; x++)
			{
				sphereIndices.push_back(y * (X_SEGMENTS + 1) + x);
				sphereIndices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				sphereIndices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				sphereIndices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	*indices = sphereIndices.size();

	glCreateVertexArrays(1, &sphereVao);
	glBindVertexArray(sphereVao);

	glCreateBuffers(1, &sphereVbo);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphereVertices.size() * 3, &sphereVertices[0], GL_STATIC_DRAW);

	glCreateBuffers(1, &sphereEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * sphereIndices.size(), &sphereIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));

	glBindVertexArray(0);

	return sphereVao;
}