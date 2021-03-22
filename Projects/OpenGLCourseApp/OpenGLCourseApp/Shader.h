#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetEyePositionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetTextureLocation();
	GLuint GetDirectionalLightTransformLocation();
	GLuint GetDirectionalShadowMapLocation();

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount; // With multiple lights, now each light has its own values at different locations in the GPU...

	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		uniformEyePosition,
		uniformSpecularIntensity, uniformShininess,
		uniformTexture,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap;

	// Has no name, but is an instance referenced by variable name "uniformDirectionalLight"
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS]; // We want to handle multiple point lights.

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

