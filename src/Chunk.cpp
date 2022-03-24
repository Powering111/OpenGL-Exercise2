#include "Chunk.h"
#include "object.h"
#include "util.h"
#include "Block.h"
#include "setting.h"

extern std::vector<CubeRenderer> cubeRenderers;

Chunk::Chunk(int x, int z) {
	this->x = x;
	this->z = z;
	//chunk generation
	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			blocks[x][6][z] = Block(1);
			blocks[x][4][z] = blocks[x][5][z] = Block(12);
			blocks[x][3][z] = blocks[x][2][z] = blocks[x][1][z] = blocks[x][0][z] = Block(2);
			for (int y = 7; y <= setting::world_end_y; y++) {
				blocks[x][y][z] = Block(0);
			}
		}
	}
}

void Chunk::render(GLuint shaderProgram) {
	CubeRenderer::bind();
	GLuint modelLocation = glGetUniformLocation(shaderProgram, "model");
	GLuint chunkLocation = glGetUniformLocation(shaderProgram, "chunk");

	for (int i = 0; i < (*cubePositions).size(); i++) {
		cubeRenderers[i].render(modelLocation, chunkLocation, (*cubePositions)[i], (*showBits)[i], glm::vec3(x * 16, 0, z * 16));
	}
}

void Chunk::setBlock(unsigned int x, unsigned int y, unsigned int z, unsigned int id) {
	if (x >=0&& x < 16 && y >= setting::world_start_y && y <= setting::world_end_y && z >= 0 && z < 16) {

		blocks[x][y][z] = Block(id);
		recalculate();
	}
}

void Chunk::setBlock(glm::ivec3 pos, unsigned int id) {
	blocks[pos.x][pos.y][pos.z] = Block(id);
	recalculate();
}

Block Chunk::getBlock(unsigned int x, unsigned int y, unsigned int z) {
	if (x >= 0 && x < 16 && y >= setting::world_start_y && y < setting::world_end_y && z >= 0 && z < 16) {
		return blocks[x][y][z];
	}
	else{
		return Block(0);
	}
}

Block Chunk::getBlock(glm::ivec3 pos) {
	return getBlock(pos.x, pos.y, pos.z);
}

void Chunk::recalculate() {
	delete cubePositions;
	delete showBits;
	calculate();
	
}
void Chunk::calculate() {
	cubePositions = new std::vector<std::vector<glm::vec3>>;
	showBits = new std::vector<std::vector<unsigned char>>;
	for (int i = 0; i < blockTypes.size(); i++) {
		if (blockTypes[i].meshType == CUBE) {
			cubePositions->push_back(std::vector<glm::vec3>());
			showBits->push_back(std::vector<unsigned char>());
		}
	}
	for (int x = 0; x < 16; x++) {
		for (int y = setting::world_start_y; y < setting::world_end_y; y++) {
			for (int z = 0; z < 16; z++) {
				BlockType blockType = blockTypes[blocks[x][y][z].blockId];

				if (blockType.meshType == CUBE) {
					(*cubePositions)[blockType.renderer_index].push_back(glm::vec3(x, y, z));

					(*showBits)[blockType.renderer_index].push_back(calcShowBit((this->x)*16+x, y, (this->z)*16+z));
				}
			}
		}
	}

}

