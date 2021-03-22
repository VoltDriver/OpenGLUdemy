#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f); // by default, pointing straigth down.
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection)
	: Light(shadowWidth, shadowHeight,
		red, green, blue, aIntensity, dIntensity)
{

	direction = glm::vec3(xDirection, yDirection, zDirection);
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f); // Matches the area our light will be on.
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	// Lightproj is the Projection * View. The model will be supplied in the Shader itself.
	// We use -direction to set the direction the object looks at the light. This will sort of fake the position of the light.
	return lightProj * glm::lookAt(-direction,
		glm::vec3(0.0f, 0.0f, 0.0f), // Looking at the origin
		glm::vec3(0.0f, 1.0f, 0.0f)); // could add a check to make sure this Up is correct if we are looking straight up or straight down.
}

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}