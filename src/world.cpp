#include "world.h"
#include "setting.h"
World::World(){

	for (int x = setting::world_start_x; x <= setting::world_end_x; x++) {
		for (int z = 0; z < 16; z++) {
			chunks[x][z] = new Chunk(x, z);
		}
	}

	for (int x = setting::world_start_x; x <= setting::world_end_x; x++) {
		for (int z = setting::world_start_z; z <= setting::world_end_z; z++) {
			chunks[x][z]->calculate();
		}
	}
}

World::~World() {
	for (int x = setting::world_start_x; x <=setting::world_end_x; x++) {
		for (int z = setting::world_start_z; z < setting::world_end_z; z++) {
			delete chunks[x][z];
		}
	}
}

void World::render(GLuint shaderProgram) {
	for (int x = 0; x < 3; x++) {
		for (int z = 0; z < 3; z++) {
			chunks[x][z]->render(shaderProgram);
		}
	}
}

Block World::getBlock(int x, int y, int z) {
	int offsetX = modulus(x, 16);
	int offsetZ = modulus(z, 16);

	int chunkX = divide(x, 16);
	int chunkZ = divide(z, 16);
	if (chunkX<setting::world_start_x || chunkX>setting::world_end_x || chunkZ<setting::world_start_z || chunkZ>setting::world_end_z)return Block(0);

	return chunks[chunkX][chunkZ]->getBlock(offsetX, y, offsetZ);
	
}
Block World::getBlock(glm::ivec3 pos) {
	return getBlock(pos.x, pos.y, pos.z);
}
void World::refreshChunk(int x, int z) {
	if (x<setting::world_start_x || x>setting::world_end_x || z<setting::world_start_z || z>setting::world_end_z)return;
	chunks[x][z]->recalculate();
}

void World::setBlock(int x, int y, int z, int blockId) {

	int offsetX = modulus(x, 16);
	int offsetZ = modulus(z, 16);

	int chunkX = divide(x, 16);
	int chunkZ = divide(z , 16);
	if (chunkX<setting::world_start_x || chunkX>setting::world_end_x || chunkZ<setting::world_start_z || chunkZ>setting::world_end_z)return;

	chunks[chunkX][chunkZ] -> setBlock(offsetX, y, offsetZ, blockId);
	if (offsetX == 0) {
		refreshChunk(chunkX - 1, chunkZ);
	}
	if (offsetX == 15) {
		refreshChunk(chunkX + 1, chunkZ);
	}
	if (offsetZ == 0) {
		refreshChunk(chunkX, chunkZ - 1);
	}
	if (offsetZ == 15) {
		refreshChunk(chunkX, chunkZ + 1);
	}
	
}

void World::setBlock(glm::ivec3 pos, int blockId) {
	setBlock(pos.x, pos.y, pos.z, blockId);
}
