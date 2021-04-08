#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f; // Prevents division by 0.
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat near, GLfloat far, // Where the near and far plane are.
	GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight,
		red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;

	farPlane = far;

	float aspect = (float)shadowWidth / (float)shadowHeight;  // Aspect ratio of the shadow. You want the Width and height to be equal, because we use them in a cube...
	lightProj = glm::perspective(glm::radians(90.0f), aspect, near, far); // Only need one projection, we will realign it for each face.

	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, 
	GLfloat positionLocation, 
	GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;

	// +x, -x
	lightMatrices.push_back(lightProj * glm::lookAt(position, 
		position + glm::vec3(1.0, 0.0, 0.0), // Position + a direction (from our 6 directions). Has to MATCH the order in the OmniShadowMap.cpp loop of cube map. Positive X, etc.
		glm::vec3(0.0, -1.0, 0.0) // We put -1.0 because of the way we reverse our normals.
	));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	// +y, -y
	// NOTE: THe Up value is changed here to where "up" is when looking straigth up or down.
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	
	// +z, -z
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightMatrices;
}

GLfloat PointLight::GetFarPlane()
{
	return farPlane;
}

glm::vec3 PointLight::GetPosition()
{
	return position;
}
