#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); // Here, those values dont really represent color. They represent HOW MUCH of each color is shown when the light hits them.
	// e.g. , if I say 0.0f, 1.0f, 1.0f , that means any red the light will hit will not be shown. So bricks would be shown mostly black, because no red gets shown.
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
	lightProj = glm::mat4(1.0f);
	shadowMap = 0;

	shadowHeight = 0;
	shadowWidth = 0;
}

Light::Light(GLfloat pShadowWidth, GLfloat pShadowHeight,
	GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;

	lightProj = glm::mat4(1.0f);

	this->shadowHeight = pShadowHeight;
	this->shadowWidth = pShadowWidth;

	shadowMap = 0;
}

void Light::InitShadowMap()
{
	if (shadowWidth == 0 ||
		shadowHeight == 0)
	{
		throw std::invalid_argument("Shadow map has no height or width!!!");
	}
	shadowMap = new ShadowMap();

	shadowMap->Init(shadowWidth, shadowHeight);
}

ShadowMap* Light::GetShadowMap()
{
	return shadowMap;
}

Light::~Light() 
{
}
