#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	uniformDirectionalLight.uniformColour = 0;
	uniformDirectionalLight.uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformDirection = 0;
	uniformDirectionalLight.uniformDiffuseIntensity = 0;
	uniformEyePosition = 0;
	uniformShininess = 0;
	uniformSpecularIntensity = 0;
	uniformPointLightCount = 0;

	pointLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColourLocation()
{
	return uniformDirectionalLight.uniformColour;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::GetTextureLocation()
{
	return uniformTexture;
}

GLuint Shader::GetDirectionalLightTransformLocation()
{
	return uniformDirectionalLightTransform;
}

GLuint Shader::GetDirectionalShadowMapLocation()
{
	return uniformDirectionalShadowMap;
}

GLuint Shader::GetOmniLightPosLocation()
{
	return uniformOmniLightPos;
}

GLuint Shader::GetFarPlaneLocation()
{
	return uniformFarPlane;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);


}

// Texture unit is the STARTING value for texture units for lights.
void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_POINT_LIGHTS)
		lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour, uniformPointLight[i].uniformDiffuseIntensity,
			uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);

		// Setting shadowmap texture values for the texture units.
		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i); // Texture unit 0, 1, 2, ... are all 1 apart.
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i); // Here, we dont put GL_TEXTURE0 because this value doesnt need to be an enum once in our shader file.
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane());
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}


void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void Shader::CompileProgram()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	// Setting uniform variables.
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		// To change the "i" inside a char array, which is used when using the GetUniformLocation methods.
		char locationBuffer[100] = { '\0' }; // Null terminate value, so end of string.

		// Printing to a buffer
		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.colour", i); // base is the base struct in the shader.
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.ambientIntensity", i); // base is the base struct in the shader.
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.diffuseIntensity", i); // base is the base struct in the shader.
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].position", i); // base is the base struct in the shader.
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locationBuffer);
		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].constant", i); // base is the base struct in the shader.
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].linear", i); // base is the base struct in the shader.
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locationBuffer);

		snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].exponent", i); // base is the base struct in the shader.
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locationBuffer);
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");// Because the name of the variable, directionalLightTransform, is the same name across
	// both our shaders (shader.vert and shadow_map.vert), we dont have to do it twice. It will be bound for both of them.
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; i++)
	{
		// To change the "i" inside a char array, which is used when using the GetUniformLocation methods.
		char locationBuffer[100] = { '\0' }; // Null terminate value, so end of string.

		// Printing to a buffer
		snprintf(locationBuffer, sizeof(locationBuffer), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locationBuffer);
	}

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		// To change the "i" inside a char array, which is used when using the GetUniformLocation methods.
		char locationBuffer[100] = { '\0' }; // Null terminate value, so end of string.

		// Printing to a buffer
		snprintf(locationBuffer, sizeof(locationBuffer), "omniShadowMaps[%d].shadowMap", i); 
		uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locationBuffer);

		// Printing to a buffer
		snprintf(locationBuffer, sizeof(locationBuffer), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locationBuffer);
	}
}

Shader::~Shader()
{
	ClearShader();
}
