#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <stdexcept>

#include "ShadowMap.h"

class Light
{
	public:
		Light();
		Light(GLfloat pShadowWidth, GLfloat pShadowHeight,
			GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
		~Light();

		void InitShadowMap();
		ShadowMap* GetShadowMap();

	protected:
		glm::vec3 colour; // Here, those values dont really represent color. They represent HOW MUCH of each color is shown when the light hits them.
	// e.g. , if I say 0.0f, 1.0f, 1.0f , that means any red the light will hit will not be shown. So bricks would be shown mostly black, because no red gets shown.
		GLfloat ambientIntensity; // Controls how bright the light is.
		GLfloat diffuseIntensity; // The amount of diffuse light in the scene.

		glm::mat4 lightProj; // How the light can see. Projection matrix of the light.

		ShadowMap* shadowMap;
		GLfloat shadowWidth;
		GLfloat shadowHeight;
};

