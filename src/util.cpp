#include "util.h"
#include "world.h"
extern World world;

std::string readFile(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Can't open file!" << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

GLuint loadTexture(const char* texturePath) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data) {
		if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Texture load failure" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}

GLuint loadShader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::string vertexShaderStr = readFile(vertexShaderPath);
	std::string fragmentShaderStr = readFile(fragmentShaderPath);
	const char* vertexShaderSrc = vertexShaderStr.c_str();
	const char* fragmentShaderSrc = fragmentShaderStr.c_str();

	GLint result = GL_FALSE;
	GLchar errorLog[512];


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "vertex shader compile error: \n" << errorLog << std::endl;
		return false;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "fragment shader compile error: \n" << errorLog << std::endl;
		return false;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, errorLog);
		std::cerr << "Shader program link error : \n" << errorLog << std::endl;
		return false;
	}

	return shaderProgram;
}

glm::ivec3 getPointingBlock(glm::vec3 pos, glm::vec3 dir) {
	int blockX = glm::round(pos.x);
	int blockY = glm::round(pos.y);
	int blockZ = glm::round(pos.z);

	float stepX = dir.x >= 0 ? 1 : -1;
	float stepY = dir.y >= 0 ? 1 : -1;
	float stepZ = dir.z >= 0 ? 1 : -1;

	float tMaxX = glm::abs((blockX + stepX * 0.5f - pos.x) / dir.x);
	float tMaxY = glm::abs((blockY + stepY * 0.5f - pos.y) / dir.y);
	float tMaxZ = glm::abs((blockZ + stepZ * 0.5f - pos.z) / dir.z);

	float tDeltaX = glm::abs(1.0f / dir.x);
	float tDeltaY = glm::abs(1.0f / dir.y);
	float tDeltaZ = glm::abs(1.0f / dir.z);



	while (tMaxX <= 6 || tMaxY <= 6 || tMaxZ <= 6) {
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				tMaxX += tDeltaX;
				blockX += stepX;
			}
			else {
				tMaxZ += tDeltaZ;
				blockZ += stepZ;
			}
		}
		else {
			if (tMaxY < tMaxZ) {
				tMaxY += tDeltaY;
				blockY += stepY;
			}
			else {
				tMaxZ += tDeltaZ;
				blockZ += stepZ;
			}
		}

		if (world.getBlock(blockX, blockY, blockZ).blockId) {
			return glm::ivec3(blockX, blockY, blockZ);
		}
	}
	return glm::ivec3(0, -1, 0);
}
glm::ivec3 getPointingBlockSide(glm::vec3 pos, glm::vec3 dir) {
	int blockX = glm::round(pos.x);
	int blockY = glm::round(pos.y);
	int blockZ = glm::round(pos.z);

	float stepX = dir.x >= 0 ? 1 : -1;
	float stepY = dir.y >= 0 ? 1 : -1;
	float stepZ = dir.z >= 0 ? 1 : -1;

	float tMaxX = glm::abs((blockX + stepX * 0.5f - pos.x) / dir.x);
	float tMaxY = glm::abs((blockY + stepY * 0.5f - pos.y) / dir.y);
	float tMaxZ = glm::abs((blockZ + stepZ * 0.5f - pos.z) / dir.z);

	float tDeltaX = glm::abs(1.0f / dir.x);
	float tDeltaY = glm::abs(1.0f / dir.y);
	float tDeltaZ = glm::abs(1.0f / dir.z);


	int prevBlockX = 0, prevBlockY = -1, prevBlockZ = 0;
	while (tMaxX <= 6 || tMaxY <= 6 || tMaxZ <= 6) {
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				tMaxX += tDeltaX;
				blockX += stepX;
			}
			else {
				tMaxZ += tDeltaZ;
				blockZ += stepZ;
			}
		}
		else {
			if (tMaxY < tMaxZ) {
				tMaxY += tDeltaY;
				blockY += stepY;
			}
			else {
				tMaxZ += tDeltaZ;
				blockZ += stepZ;
			}
		}

		if (world.getBlock(blockX, blockY, blockZ).blockId) {
			return glm::ivec3(prevBlockX, prevBlockY, prevBlockZ);
		}
		prevBlockX = blockX;
		prevBlockY = blockY;
		prevBlockZ = blockZ;
		//spawnChunk.changeBlock(glm::ivec3(blockX, blockY, blockZ), 3);
	}
	return glm::ivec3(0, -1, 0);

}

int modulus(int a, int b) {
	return (a % b + b) % b;
}


unsigned char calcShowBit(int x, int y, int z) {
	unsigned char result=0;
	if (world.getBlock(x, y, z-1).blockId==0) {
		result |= 1;
	}
	if (world.getBlock(x, y, z+1).blockId==0) {
		result |= 2;
	}
	if (world.getBlock(x-1, y, z).blockId==0) {
		result |= 4;
	}
	if (world.getBlock(x+1, y, z).blockId==0) {
		result |= 8;
	}
	if (world.getBlock(x, y+1, z).blockId==0) {
		result |= 16;
	}
	if (world.getBlock(x, y-1, z).blockId==0) {
		result |= 32;
	}

	return result;
}

int divide(int a, int b) {
	return (a - modulus(a, b)) / b;
}