#pragma once

enum MeshType {
	NONE, CUBE
};
class BlockType {
public:
	const unsigned int blockID;
	const MeshType meshType;
	const unsigned int renderer_index;
	BlockType();
	BlockType(unsigned int id, MeshType type);
	BlockType(unsigned int id, MeshType type, unsigned int renderer_index);
};

extern const std::vector<BlockType> blockTypes;

extern const std::vector<std::string> assets;