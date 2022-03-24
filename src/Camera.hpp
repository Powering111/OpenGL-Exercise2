#pragma once
#include "util.h"

enum Direction {
	FRONT, BACK, UP, DOWN, RIGHT, LEFT
};
class Camera {
public:

	glm::vec3 cameraPos, cameraRight, cameraUp, cameraDir;
	glm::vec3 frontDir;
	glm::mat4 view, projection;
	float speed = 0.1f, sensitivity=0.1f;
	float yaw = 0, pitch = 0, fov = 90.0f;
	float aspect;

	Camera(glm::vec3 position, glm::vec3 direction, float aspect) {
		this->cameraPos= position;
		this->cameraDir = direction;
		this->aspect = aspect;
		updateViewMatrix();
		updateProjectionMatrix();
	}

	void move(int direction) {
		switch (direction) {
		case FRONT:
			cameraPos += frontDir * speed;
			break;
		case LEFT:
			cameraPos -= cameraRight * speed;
			break;
		case BACK:
			cameraPos -= frontDir * speed;
			break;
		case RIGHT:
			cameraPos += cameraRight * speed;
			break;
		case UP:
			cameraPos += glm::vec3(0.0f,1.0f,0.0f) * speed;
			break;
		case DOWN:
			cameraPos -= glm::vec3(0.0f,1.0f,0.0f) * speed;
			break;
		}
		updateViewMatrix();
	}

	void rotate(float x, float y) {
		yaw += -x * sensitivity;
		pitch += y * sensitivity;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}
		glm::vec3 direction(1.0f);
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction = glm::normalize(direction);

		cameraDir = direction;

		updateViewMatrix();
	}

	void updateAspect(float aspect) {
		this->aspect = aspect;
		updateProjectionMatrix();
	}
private:
	void updateViewMatrix() {
		cameraRight = glm::normalize(glm::cross(cameraDir, glm::vec3(0.0f,1.0f,0.0f)));
		cameraUp = glm::normalize(glm::cross(cameraRight, cameraDir));
		frontDir = cameraDir;
		frontDir.y = 0;
		frontDir = glm::normalize(frontDir);
		view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraUp);
	}
	void updateProjectionMatrix() {
		projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 10000.0f);
	}
};