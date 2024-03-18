#pragma once

#include <glm/glm.hpp>

class Camera
{
	public:

		Camera();


		glm::mat4 update();
		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void moveForward();
		void moveBackward();
		void rotateLeft(bool mouse);
		void rotateRight(bool mouse);
		void rotateUp(bool mouse);
		void rotateDown(bool mouse);

	private:
		glm::vec3 eye;
		glm::vec3 target;
		glm::vec3 up;

		const float cameraSpeed = 0.2f;
		const float rotationSpeed = 0.2f;
		const float mouseRotationSpeed = 0.01f;
};

