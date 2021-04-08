#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, // Position
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(vertices[0]) * 8, // Skip 8 values as the Stride, because each of our value has 5 values in the array..
		0);
	glEnableVertexAttribArray(0);

	// Now we tell it how to handle the UV coords.
	glVertexAttribPointer(1, // Tex coordinates
		2, // 2 values in our UV coords.
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertices[0]) * 8, // Skip 8 values as the Stride before we reach next uv coord
		(void*)(sizeof(vertices[0]) * 3) // Offset. Where to start for uv coords.
	);
	glEnableVertexAttribArray(1);

	// Now we tell it how to handle the normal coords.
	glVertexAttribPointer(2, // Normal coordinates
		3, // our coordinates have x,y,z
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertices[0]) * 8, // Skip 8 values as the Stride before we reach next normal coord
		(void*)(sizeof(vertices[0]) * 5) // Offset. Where to start for normal coords.
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}


Mesh::~Mesh()
{
	ClearMesh();
}
