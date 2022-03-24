#pragma once
#include "util.h"
#include "chunk.h"

class World {
public:
	static constexpr int BLOCKS_PER_CHUNK = 16;
	World();
	~World();

	Block getBlock(int x, int y, int z);
	Block getBlock(glm::ivec3 pos);
	void setBlock(int x, int y, int z, int BlockId);
	void setBlock(glm::ivec3 pos, int blockId);

	void render(GLuint shaderProgram);
	
	void refreshChunk(int x, int z);
private:
	Chunk * chunks[16][16];

	
};