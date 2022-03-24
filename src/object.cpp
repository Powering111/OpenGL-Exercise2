#include "object.h"

CubeRenderer::CubeRenderer(GLuint textureID, bool transparent) {
	this->transparent = transparent;
	textures[0] = textures[1] = textures[2] = textures[3] = textures[4] = textures[5] = textureID;

}
CubeRenderer::CubeRenderer(GLuint texture_up, GLuint texture_down, GLuint texture_side, bool transparent) {
	this->transparent = transparent;
	textures[4] = texture_up;
	textures[5] = texture_down;
	textures[0] = textures[1] = textures[2] = textures[3] = texture_side;

}


void CubeRenderer::init() {
	GLuint posVBO = 0, texCoordVBO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturePos), texturePos, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}

GLuint CubeRenderer::VAO = 0, CubeRenderer::EBO = 0;


void CubeRenderer::render(GLuint model_uniform_location, GLuint chunk_uniform_location, std::vector<glm::vec3>& positions, std::vector<unsigned char>& showBit,glm::vec3 chunk) {
	if (this->transparent) {
		glEnable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
	}
	glUniform3fv(chunk_uniform_location, 1, glm::value_ptr(chunk));
	unsigned char check_side = 1;
	for (int side = 0; side < 6; side++) {
		glBindTexture(GL_TEXTURE_2D, textures[side]);
		for (int i = 0; i < positions.size(); i++) {
			if (showBit[i] & check_side) {
				glm::mat4 model(1.0f);
				model = glm::translate(model, positions[i]);
				glUniformMatrix4fv(model_uniform_location, 1, GL_FALSE, glm::value_ptr(model));

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(side * 6 * sizeof(GLuint)));
			}
		}
		check_side <<= 1;
	}
}

void CubeRenderer::bind() {
	glBindVertexArray(VAO);
}