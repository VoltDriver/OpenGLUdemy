#pragma once
#include <GL/glew.h>

class Mesh
{
	public:
		Mesh();
		~Mesh();

		// Create the mesh.
		void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
		// Draw it on screen.
		void RenderMesh();
		// Clear it from the GPU.
		void ClearMesh();


	private:
		GLuint VAO, VBO, IBO;
		GLsizei indexCount; // Just an integer, but recognized by openGL to represent a size.
};

