#pragma once

#include <glm/glm.hpp>

#define MAX_VAO_COUNT 20
#define WIDTH 1280
#define HEIGHT 720

glm::vec3 sphereColors[] =
{
	glm::vec3(0.70f, 0.5f, 0.2f),
	glm::vec3(0.72f, 0.5f, 0.2f),
	glm::vec3(0.74f, 0.5f, 0.2f),
	glm::vec3(0.76f, 0.5f, 0.2f),
	glm::vec3(0.78f, 0.5f, 0.2f),

	glm::vec3(0.2f, 0.70f, 0.5f),
	glm::vec3(0.2f, 0.72f, 0.5f),
	glm::vec3(0.2f, 0.74f, 0.5f),
	glm::vec3(0.2f, 0.76f, 0.5f),
	glm::vec3(0.2f, 0.78f, 0.5f),

	glm::vec3(0.2f, 0.5f, 0.70f),
	glm::vec3(0.2f, 0.5f, 0.72f),
	glm::vec3(0.2f, 0.5f, 0.74f),
	glm::vec3(0.2f, 0.5f, 0.76f),
	glm::vec3(0.2f, 0.5f, 0.78f),

	glm::vec3(0.2f, 0.70f, 0.70f),
	glm::vec3(0.2f, 0.72f, 0.72f),
	glm::vec3(0.2f, 0.74f, 0.74f),
	glm::vec3(0.2f, 0.76f, 0.76f),
	glm::vec3(0.2f, 0.78f, 0.78f),

	glm::vec3(0.5f, 0.70f, 0.70f),
	glm::vec3(0.5f, 0.72f, 0.72f),
	glm::vec3(0.5f, 0.74f, 0.74f),
	glm::vec3(0.5f, 0.76f, 0.76f),
	glm::vec3(0.5f, 0.78f, 0.78f),
};

bool firstMouse = true;
double lastX = WIDTH / 2;
double lastY = HEIGHT / 2;
double xofsset = 0;
double yoffset = 0;
float deltaTime = 0;
float lastFrame = 0;
float currentFrame = 0;

glm::vec3 lightPos = { 3.0f, 3.0f, 3.0f };