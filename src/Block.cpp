#include "util.h"
#include "Block.h"


BlockType::BlockType() : blockID(0), meshType(NONE), renderer_index(0) {

}
BlockType::BlockType(unsigned int id, MeshType type) : blockID(id), meshType(type), renderer_index(0) {

}

BlockType::BlockType(unsigned int id, MeshType type, unsigned int renderer_index) : blockID(id), meshType(type), renderer_index(renderer_index) {

}


const std::vector<std::string> assets{
		"assets/grass_block_up.png",
		"assets/grass_block_down.png",
		"assets/grass_block_side.png",
		"assets/stone_block.png",
		"assets/crafting_table_up.png",
		"assets/crafting_table_side.png",
		"assets/wood.png",
		"assets/cobblestone.png",
		"assets/coal_ore.png",
		"assets/iron_ore.png",
		"assets/diamond_ore.png",
		"assets/gold_ore.png",
		"assets/emerald_ore.png",
		"assets/amethyst.png",
		"assets/awesomeface.png",
		"assets/wood_block_up.png",
		"assets/wood_block_side.png",
		"assets/leaves.png"

};

const std::vector<BlockType> blockTypes{
	BlockType(0,NONE),
	BlockType(1,CUBE,0),
	BlockType(2,CUBE,1),
	BlockType(3,CUBE,2),
	BlockType(4,CUBE,3),
	BlockType(5,CUBE,4),
	BlockType(6,CUBE,5),
	BlockType(7,CUBE,6),
	BlockType(8,CUBE,7),
	BlockType(9,CUBE,8),
	BlockType(10,CUBE,9),
	BlockType(11,CUBE,10),
	BlockType(12,CUBE,11),
	BlockType(13,CUBE,12),
	BlockType(14,CUBE,13),
	BlockType(15,CUBE,14)
};