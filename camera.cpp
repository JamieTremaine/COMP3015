#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <iostream>


Camera::Camera() {
	this->eye = glm::vec3(-6.5f, 7.4f, 20.0f);
	this->target = glm::vec3(-0.85f, 4.0f, 14.15f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::update() {
	return glm::lookAt(eye, target, up);
}

void Camera::moveLeft() {
	glm::vec3 right = glm::normalize(glm::cross(target - eye, up));
	eye -= right * cameraSpeed;
	target -= right * cameraSpeed;
}

void Camera::moveRight() {
	glm::vec3 right = glm::normalize(glm::cross(target - eye, up));
	eye += right * cameraSpeed;
	target += right * cameraSpeed;
}

void Camera::moveForward() {
	eye += glm::normalize(target - eye) * cameraSpeed;
	target += glm::normalize(target - eye) * cameraSpeed;
}

void Camera::moveBackward() {
	eye -= glm::normalize(target - eye) * cameraSpeed;
	target -= glm::normalize(target - eye) * cameraSpeed;
}

void Camera::rotateLeft(bool mouse) {
	glm::vec3 diff = eye - target;

	if (mouse == true) {
		eye.x = target.x + diff.x * std::cos(mouseRotationSpeed) - diff.z * std::sin(mouseRotationSpeed);
		eye.z = target.z + diff.x * std::sin(mouseRotationSpeed) + diff.z * std::cos(mouseRotationSpeed);
	} else {
		eye.x = target.x + diff.x * std::cos(rotationSpeed) - diff.z * std::sin(rotationSpeed);
		eye.z = target.z + diff.x * std::sin(rotationSpeed) + diff.z * std::cos(rotationSpeed);
	}

}

void Camera::rotateRight(bool mouse) {
	glm::vec3 diff = eye - target;

	if (mouse == true) {
		eye.x = target.x + diff.x * std::cos(-mouseRotationSpeed) - diff.z * std::sin(-mouseRotationSpeed);
		eye.z = target.z + diff.x * std::sin(-mouseRotationSpeed) + diff.z * std::cos(-mouseRotationSpeed);
	} else {
		eye.x = target.x + diff.x * std::cos(-rotationSpeed) - diff.z * std::sin(-rotationSpeed);
		eye.z = target.z + diff.x * std::sin(-rotationSpeed) + diff.z * std::cos(-rotationSpeed);
	}


}

void Camera::moveUp() {
	eye.y += cameraSpeed;
	target.y += cameraSpeed;
}

void Camera::moveDown() {
	eye.y -= cameraSpeed;
	target.y -= cameraSpeed;
}

void Camera::rotateUp(bool mouse) {
	glm::vec3 direction = glm::normalize(target - eye);
	glm::vec3 axis = glm::normalize(glm::cross(direction, up));

	glm::mat4 rotation;

	if (mouse == true) {
		 rotation = glm::rotate(glm::mat4(1.0f), mouseRotationSpeed, axis);
	} else {
		rotation = glm::rotate(glm::mat4(1.0f), rotationSpeed, axis);
	}

	eye = glm::vec3(rotation * glm::vec4(eye - target, 1.0f)) + target;
}

void Camera::rotateDown(bool mouse) {
	glm::vec3 direction = glm::normalize(target - eye);
	glm::vec3 axis = glm::normalize(glm::cross(direction, up));


	glm::mat4 rotation;

	if (mouse == true) {
		rotation = glm::rotate(glm::mat4(1.0f), -mouseRotationSpeed, axis);
	} else {
		rotation = glm::rotate(glm::mat4(1.0f), -rotationSpeed, axis);
	}

	eye = glm::vec3(rotation * glm::vec4(eye - target, 1.0f)) + target;
}
