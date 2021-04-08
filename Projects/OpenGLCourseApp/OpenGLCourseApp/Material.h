#pragma once

#include <GL/glew.h>


class Material
{
	public:
		Material();
		Material(GLfloat specIntensity, GLfloat shine);

		void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
	private:
		GLfloat specularIntensity; // How bright the light is on the material.
		GLfloat shininess; // How smooth the surface is protrayed as. Smaller = more spread out light (rough surface). Higher = Bright and intense points of light, like metal

};

