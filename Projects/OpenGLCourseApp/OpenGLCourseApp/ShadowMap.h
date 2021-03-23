#pragma once

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap
{
	public:
		ShadowMap();

		virtual bool Init(unsigned int width, unsigned int height);

		virtual void Write();

		virtual void Read(GLenum textureUnit);

		GLuint GetShadowWidth();
		GLuint GetShadowHeight();

		~ShadowMap();

	protected:
		GLuint FBO, // frame buffer object.
			shadowMap; 
		GLuint shadowWidth, shadowHeight; // Needed for matching the viewport dimensions.
};

