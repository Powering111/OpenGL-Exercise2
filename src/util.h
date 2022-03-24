#pragma once

#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <stb_image.h>

std::string readFile(const char* filePath);
GLuint loadShader(const char*, const char*);
GLuint loadTexture(const char*);
glm::ivec3 getPointingBlock(glm::vec3 pos, glm::vec3 dir);
glm::ivec3 getPointingBlockSide(glm::vec3 pos, glm::vec3 dir);

int modulus(int a, int b);
int divide(int a, int b);

unsigned char calcShowBit(int x, int y, int z);