#pragma once

#include <GL/glew.h>

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
	public:
		Camera();
		Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

		void keyControl(bool* keys, GLfloat deltaTime); // Controls the movement of the camera when keys are pressed
		void mouseControl(GLfloat xChange, GLfloat yChange); // Controls the movement of thge camera when the mouse is moved.

		glm::mat4 calculateViewMatrix();

		~Camera();
	private:
		glm::vec3 position; // Position of the camera
		glm::vec3 front; // Direction vector of the front of the camera
		glm::vec3 up; // The up of the camera
		glm::vec3 right; // The right of the camera, as a vector.
		glm::vec3 worldUp; // Up direction of the world. "the sky".

		GLfloat yaw;
		GLfloat pitch;

		GLfloat movementSpeed;
		GLfloat turnSpeed;

		void update(); // Update our values everytime something changes
};

