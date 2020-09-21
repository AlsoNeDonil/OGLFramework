#include "utils/ImGui/imgui_impl_opengl3.h"
#include "utils/ImGui/imgui_impl_glfw.h"

#include "GLFW/glfw3.h"
#include <GLAD/glad.h>

#include "stb_image.h"
#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "utils/OpenGL/Texture.h"
#include "utils/OpenGL/VertexArray.h"
#include "utils/OpenGL/Shader.h"
#include "utils/OpenGL/Camera.h"

#include "utils/Core/Data.h"

using namespace OGLFramework;

GLFWwindow* m_Window;
GLboolean keys[512];

inline void imgui_init(GLFWwindow*);
inline void imgui_start();
inline void imgui_end();

void Input();
bool InitLibs();
void CheckExtensions();

VertexArray LoadCubeVertices();
VertexArray LoadSphereVertices(float radius, uint32_t x_segments, uint32_t y_segments);

std::shared_ptr<OGLFramework::Camera> m_MainCamera;

int main()
{
	InitLibs();
	imgui_init(m_Window);
	CheckExtensions();

	m_MainCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));
	Shader sphereShader("assets/shaders/lightDefault.vert.glsl", "assets/shaders/lightDefault.frag.glsl");
	Shader cubeShader("assets/shaders/cube.vert.glsl", "assets/shaders/cube.frag.glsl");
	Shader texturedCubeShader("assets/shaders/texturedCube.vert.glsl", "assets/shaders/texturedCube.frag.glsl");

	VertexArray cubeVao = LoadCubeVertices();
	VertexArray sphereVao = LoadSphereVertices(1.0f, 64, 64);

	glm::mat4 model(1.0);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

	Texture texture("assets/textures/texture.png");

	//Main Loop
	while (!glfwWindowShouldClose(m_Window))
	{
		//update delta time
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
		}

		glm::mat4 view = m_MainCamera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(m_MainCamera->m_Zoom, 1280.0f / 720.0f, 0.1f, 100.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture.Set(0);

		cubeShader.Use();
		{//render first cube
			glBindVertexArray(cubeVao.GetVAO());

			model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, -3.0f));

			cubeShader.SetMat4("u_Model", model);
			cubeShader.SetMat4("u_View", view);
			cubeShader.SetMat4("u_Projection", projection);

			cubeShader.SetVec3("u_Color", glm::vec3(0.8f, 0.2f, 0.5f));
			cubeShader.SetVec3("u_ViewPos", m_MainCamera->m_Position);
			cubeShader.SetVec3("u_LightPosition", lightPos);
			glDrawArrays(GL_TRIANGLES, 0, cubeVao.GetIndicesCount());

			model = glm::mat4(1.0f);
		}

		{//render second cube
			glBindVertexArray(cubeVao.GetVAO());

			model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, -3.0f));

			texturedCubeShader.Use();
			texturedCubeShader.SetMat4("u_Model", model);
			texturedCubeShader.SetMat4("u_View", view);
			texturedCubeShader.SetMat4("u_Projection", projection);

			texturedCubeShader.SetInt("u_TextureID", 0);
			texturedCubeShader.SetVec3("u_ViewPos", m_MainCamera->m_Position);
			texturedCubeShader.SetVec3("u_LightPosition", lightPos);
			glDrawArrays(GL_TRIANGLES, 0, cubeVao.GetIndicesCount());

			model = glm::mat4(1.0f);
		}

		{ // render sphere
			glBindVertexArray(sphereVao.GetVAO());

			model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));

			sphereShader.Use();
			sphereShader.SetMat4("u_Model", model);
			sphereShader.SetMat4("u_View", view);
			sphereShader.SetMat4("u_Projection", projection);

			sphereShader.SetVec3("u_Color", glm::vec3(0.8f, 0.5f, 0.2f));
			sphereShader.SetVec3("u_ViewPos", m_MainCamera->m_Position);
			sphereShader.SetVec3("u_LightPosition", lightPos);
			glDrawElements(GL_TRIANGLE_STRIP, sphereVao.GetIndicesCount(), GL_UNSIGNED_INT, nullptr);
		}

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

void CheckExtensions()
{
	GLint num;
	glGetIntegerv(GL_NUM_EXTENSIONS, &num);

	for (int i = 0; i < num; i++)
		spdlog::warn(glGetStringi(GL_EXTENSIONS, i));
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
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	glfwSwapInterval(1);

	imgui_init(m_Window);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
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

VertexArray LoadCubeVertices()
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

	std::vector<uint32_t> attribs;
	attribs.push_back(3); attribs.push_back(3); attribs.push_back(2);

	return VertexArray(attribs, vertices, sizeof(vertices), 36);
}

VertexArray LoadSphereVertices(float radius, uint32_t x_segments, uint32_t y_segments)
{
	std::vector<glm::vec3> sphereVertices;
	std::vector<uint32_t> sphereIndices;

	constexpr float PI = glm::pi<float>();

	//generate sphere vertices
	for (uint32_t y = 0; y <= y_segments; ++y)
	{
		for (uint32_t x = 0; x <= x_segments; ++x)
		{
			float xSegment = (float)x / (float)x_segments;
			float ySegment = (float)y / (float)y_segments;

			float xPos = radius * std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = radius * std::cos(ySegment * PI);
			float zPos = radius * std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			sphereVertices.push_back(glm::vec3(xPos, yPos, zPos));
			sphereVertices.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	//generate sphere indices
	bool oddRow = false;
	for (size_t y = 0; y < y_segments; y++)
	{
		if (!oddRow)
		{
			for (size_t x = 0; x <= x_segments; x++)
			{
				sphereIndices.push_back(y * (x_segments + 1) + x);
				sphereIndices.push_back((y + 1) * (x_segments + 1) + x);
			}
		}
		else
		{
			for (int x = x_segments; x >= 0; --x)
			{
				sphereIndices.push_back((y + 1) * (x_segments + 1) + x);
				sphereIndices.push_back(y * (x_segments + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	std::vector<uint32_t> attribs;
	attribs.push_back(3); attribs.push_back(3);

	return VertexArray(attribs, sphereVertices, sphereIndices);
}