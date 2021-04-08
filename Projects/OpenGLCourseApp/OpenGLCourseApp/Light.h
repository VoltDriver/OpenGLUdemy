#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
	public:
		Light();
		Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

	protected:
		glm::vec3 colour; // Here, those values dont really represent color. They represent HOW MUCH of each color is shown when the light hits them.
	// e.g. , if I say 0.0f, 1.0f, 1.0f , that means any red the light will hit will not be shown. So bricks would be shown mostly black, because no red gets shown.
		GLfloat ambientIntensity; // Controls how bright the light is.
		GLfloat diffuseIntensity; // The amount of diffuse light in the scene.
};

