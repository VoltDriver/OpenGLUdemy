#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); // Here, those values dont really represent color. They represent HOW MUCH of each color is shown when the light hits them.
	// e.g. , if I say 0.0f, 1.0f, 1.0f , that means any red the light will hit will not be shown. So bricks would be shown mostly black, because no red gets shown.
	ambientIntensity = 1.0f;
	direction = glm::vec3(0.0f, -1.0f, 0.0f); // by default, pointing straigth down.
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDirection, yDirection, zDirection);
	diffuseIntensity = dIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
