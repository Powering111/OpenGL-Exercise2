#include <iostream>
#include <vector>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util.h"
#include "Camera.hpp"
#include "object.h"
#include "Chunk.h"
#include "World.h"
#include "Block.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static void errorCallback(int errorCode, const char* errDescription);

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
static void framebuffersizeCallback(GLFWwindow* window, int width, int height);

void handleMove(GLFWwindow* window);

unsigned int trianglePositionVBO, triangleColorVBO, triangleTextureVBO;
unsigned int triangleVAO;
unsigned int triangleEBO;


Camera camera(glm::vec3(0.0f, 8.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1200.0f / 800.0f);

std::vector<CubeRenderer> cubeRenderers;
World world;
unsigned int holdingBlockID=1;
GLuint shaderProgram;

int main() {

	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) {
		std::cerr << "GLFW 초기화 실패" << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Minceraft", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetMouseButtonCallback(window, mouseClickCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, framebuffersizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to load OpenGL Procs" << std::endl;
		return -1;
	}

	glfwSwapInterval(1);



	shaderProgram = loadShader("res/vertexShader.glsl", "res/fragmentShader.glsl");



	std::vector<GLuint> textures;
	for (int i = 0; i < assets.size(); i++) {
		textures.push_back(loadTexture(assets[i].c_str()));
	}

	cubeRenderers.push_back(CubeRenderer(textures[0], textures[1], textures[2])); //grass block
	cubeRenderers.push_back(CubeRenderer(textures[3])); // stone block
	cubeRenderers.push_back(CubeRenderer(textures[4], textures[6], textures[5])); // Crafting Table
	cubeRenderers.push_back(CubeRenderer(textures[6])); // wood block
	cubeRenderers.push_back(CubeRenderer(textures[7])); // cobblestone block
	cubeRenderers.push_back(CubeRenderer(textures[8])); // coal ore
	cubeRenderers.push_back(CubeRenderer(textures[9])); // iron ore
	cubeRenderers.push_back(CubeRenderer(textures[10])); // diamond ore
	cubeRenderers.push_back(CubeRenderer(textures[11])); // gold ore
	cubeRenderers.push_back(CubeRenderer(textures[12])); // emerald ore
	cubeRenderers.push_back(CubeRenderer(textures[13])); // amethyst block
	cubeRenderers.push_back(CubeRenderer(textures[1])); // dirt block
	cubeRenderers.push_back(CubeRenderer(textures[14],true)); // awesome block
	cubeRenderers.push_back(CubeRenderer(textures[15], textures[15], textures[16]));
	cubeRenderers.push_back(CubeRenderer(textures[17],true));

	// blending
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec3 up(0.0f, 1.0f, 0.0f);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLuint modelLocation = glGetUniformLocation(shaderProgram, "model");
	GLuint viewLocation = glGetUniformLocation(shaderProgram, "view");
	GLuint projectionLocation = glGetUniformLocation(shaderProgram, "projection");

	CubeRenderer::init();
	while (!glfwWindowShouldClose(window)) {
		handleMove(window);
		
		glClearColor(0.5, 0.2, 0.9, 0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glm::mat4 view = camera.view;
		glm::mat4 projection = camera.projection;

		
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		
		world.render(shaderProgram);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


static void errorCallback(int errorCode, const char* errDescription)
{
	std::cerr << "Error: " << errorCode << std::endl << errDescription << std::endl;
}
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		std::cout << "bye!\n";
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
static void framebuffersizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	
	camera.updateAspect((float)width / height);
}


static void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
	static double lastx, lasty;
	static bool isFirst = true;
	if (isFirst) {
		isFirst = false;
		lastx = xpos;
		lasty = ypos;

	}
	double offsetX = -xpos + lastx;
	double offsetY = -ypos + lasty;
	
	camera.rotate(offsetX, offsetY);

	lastx = xpos;
	lasty = ypos;
	
}

glm::vec3 getRay() {
	glm::mat4 inverse_projection = glm::inverse(camera.projection);
	glm::mat4 inverse_view = glm::inverse(camera.view);

	glm::vec4 ray_eye = inverse_projection * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
	glm::vec4 ray_world_4 = inverse_view * ray_eye;
	glm::vec3 ray_world = glm::vec3(ray_world_4.x, ray_world_4.y, ray_world_4.z);

	ray_world = glm::normalize(ray_world);
	return ray_world;
}




static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glm::ivec3 clickedBlockPos = getPointingBlock(camera.cameraPos, camera.cameraDir);
		if (clickedBlockPos.y != -1) {
			world.setBlock(clickedBlockPos, 0);
		}
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		glm::ivec3 clickedBlockSidePos = getPointingBlockSide(camera.cameraPos, camera.cameraDir);
		if (clickedBlockSidePos.y != -1) {
			world.setBlock(clickedBlockSidePos, holdingBlockID);
		}
	}
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset < 0) {
		holdingBlockID++;
	}
	if (yoffset > 0) {
		holdingBlockID--;
	}
	if (holdingBlockID == 0) {
		holdingBlockID = blockTypes.size()-1;
	}
	else if (holdingBlockID == blockTypes.size()) {
		holdingBlockID = 1;
	}
	std::cout << "Inventory [" << holdingBlockID << "]\n";
}


void handleMove(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera.move(FRONT);
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera.move(LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera.move(BACK);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera.move(RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		camera.move(UP);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
		camera.move(DOWN);
	}
}
