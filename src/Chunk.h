#pragma once
#include "util.h"

class Block {
public:
	unsigned char blockId;

	Block() {
		blockId = 0;
	}
	Block(unsigned char id) {
		blockId = id;
	}
};

class Chunk {
public:
	int x, z;

	Chunk(int x, int z);

	void render(GLuint shaderProgram);

	void setBlock(unsigned int x, unsigned int y, unsigned int z, unsigned int id);

	void setBlock(glm::ivec3 pos, unsigned int id);
	
	Block getBlock(unsigned int x, unsigned int y, unsigned int z);

	Block getBlock(glm::ivec3 pos);

	void calculate();
	void recalculate();
private:

	Block blocks[16][128][16];
	std::vector<std::vector<glm::vec3>> * cubePositions;
	std::vector<std::vector<unsigned char>> * showBits;

};
