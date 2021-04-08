#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	// using the Delta time so that faster CPUS dont make the camera move faster!
	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		// Since position is where our camera is, and front is the direction vector of the front of the camera, this is how we move!
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	// Because we are not changing any angles, we dont need to call update.
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	// Modifying our change by the speed we turn.
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	// Updating our angles
	yaw += xChange;
	pitch += yChange;

	// We limit the pitch, because we dont want our "head" to be able to look up and up and up and then so much up that we look backwards. 
	// Vice versa for backwards.
	// The breakpoint for weird things is 90 degrees.
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update(); // Recalculating the new camera angles and how to display the view.
}

glm::mat4 Camera::calculateViewMatrix()
{
	// This is designed for cameras, to calculate a matrix that applies all correct transformations to look at something from a given position.
	return glm::lookAt(position, // Where we are looking at from
		position + front, // Target, what to look at
		up // Where the up is.
	);
}

Camera::~Camera()
{
}

void Camera::update()
{
	// Setting the initial view direction vector. THe math is explained in the theory of Udemy course.
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	// Setting the initial Right and Up. Again, theory in Udemy course.
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
