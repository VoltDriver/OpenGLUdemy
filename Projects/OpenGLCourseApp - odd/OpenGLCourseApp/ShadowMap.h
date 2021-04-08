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

		virtual GLuint GetShadowWidth();
		virtual GLuint GetShadowHeight();

		~ShadowMap();

	protected:
		GLuint FBO, // frame buffer object.
			shadowMap; 
		GLuint shadowWidth, shadowHeight; // Needed for matching the viewport dimensions.
};

