#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
    FBO = 0;
    shadowMap = 0;
}

bool ShadowMap::Init(GLuint width, GLuint height)
{
    shadowWidth = width;
    shadowHeight = height;

    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Setting texture parameters
	glTexParameteri(GL_TEXTURE_2D, // Type of Texture
		GL_TEXTURE_WRAP_S, // Which parameter to change. Here, we change the way it wraps around the x-axis of the image.
		GL_CLAMP_TO_BORDER // 
	);
	glTexParameteri(GL_TEXTURE_2D, // Type of Texture
		GL_TEXTURE_WRAP_T, // Which parameter to change. Here, we change the way it wraps around the x-axis of the image.
		GL_CLAMP_TO_BORDER // The value. 
	);

	// Because we are clamping the shadow map to a border, we need to define the border.
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Setting to define the color of the border. "All 1 because as far away from the light position as possible"
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, // Type of Texture
		GL_TEXTURE_MIN_FILTER, // Which parameter to change. Here, we change the way the texture interacts when we zoom out on it.
		GL_NEAREST // The value.  Here, we could also use GL_LINEAR. It's personnal preference.
	);
	glTexParameteri(GL_TEXTURE_2D, // Type of Texture
		GL_TEXTURE_MAG_FILTER, // Which parameter to change. Here, we change the way the texture interacts when we zoom in on it.
		GL_NEAREST // The value. Here, we could also use GL_LINEAR. It's personnal preference.
	);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// Connects the frame buffer to the texture, so that when the frame buffer is updated it is rendered in the texture.
	glFramebufferTexture2D(GL_FRAMEBUFFER, // Target
						   GL_DEPTH_ATTACHMENT, // Which part we should attach and write to the texture
						   GL_TEXTURE_2D, // Type to write to.
						   shadowMap, // ID of the texture to write to
						   0 // Mipmap level
		);

	// Disabling reading colors.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	// Checking all went well
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Shadowmap Framebuffer Error: %i\n", status);
	}

	// Unbinding buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO); // Binding buffer
}

void ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}

GLuint ShadowMap::GetShadowWidth()
{
    return shadowWidth;
}

GLuint ShadowMap::GetShadowHeight()
{
    return shadowHeight;
}

ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}
	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}
