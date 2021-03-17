#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

    // Creating our VAO. 1- Amount of arrays and then 2- Where to store the ID of the array.
    // This now creates some stuff in the graphics card and its memory.
    glGenVertexArrays(1, &VAO);
    // Binding. Now all our operations that interact with Vertex Array will interact with this array.
    glBindVertexArray(VAO);
    // We now Indent, because this shows that everyting that is indented will work with the array object bound above.

    glGenBuffers(1, // How many buffers to create?
        &IBO
    );
    // This is a buffer that stores elements, or indices. Same thing.
    // Look a bit down to see the definition of each param.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices[0]) * numOfIndices,
        indices,
        GL_STATIC_DRAW
    );

    // Same as above, but for buffers.
    glGenBuffers(1, &VBO);
    // Binding. First choose the target to bind to. VBO has multiple targets it can bind to.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Connect the vertices we created to the VBO
    glBufferData(GL_ARRAY_BUFFER, // Target
        sizeof(vertices[0]) * numOfVertices, // the size of the data we are passing in. could also have said sizeof(GLfloat * numOfVertices)
        vertices, // Our actual array
        GL_STATIC_DRAW // could also be GL_DYNAMIC_DRAW.  Static: Not going to change where the points are in the array.
    );

    glVertexAttribPointer(0, // Location of position attribute. The position attribute and its location are determined inside the Shader code for vertex shader.
        3, // Amount of values passed in to location. In our case, we have 3 values (X,Y,Z)
        GL_FLOAT, // Type of the values.
        GL_FALSE, // Normalize the values or not
        0,  // Stride. This is to tell it to "skip" certain values in our vertices array to read the next vertex info. This is useful
            // if we define the colors and additionnal data of our vertices all in one single array. Then we could say "each vertex is separated by 3 floats inside the array."
            // In our case, we are not doing that.
        0 // Offset. Where the data starts. We could say "ignore first line of array" through this.
    );
    // Enables the usage of our attribute located at position 0, so our position attribute for our vertices.
    glEnableVertexAttribArray(0);

    // Unbind buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind array.
    glBindVertexArray(0);
    // Unbind IBO. Note, unbind IBO AFTER VAO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // And now we are not indented anymore! Because we have unbound our vertex array.
}

void Mesh::RenderMesh()
{
    // We want to work with our created VAO.
    glBindVertexArray(VAO);
    // We indent to show we are working with this VAO from here on.  

    // Binding IBO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Drawing our triangles.
    glDrawElements(GL_TRIANGLES, // What to draw
        indexCount, // Count of indices
        GL_UNSIGNED_INT, // Format of indices
        0 // Point to the indices, but we dont need it because we have IBO already.
    );

    // We unbind the VAO.
    glBindVertexArray(0);
    // Unbinding the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::ClearMesh()
{
    if (IBO != 0)
    {
        // Cleaning the buffers.
        glDeleteBuffers(
            1, // How many to delete?
            &IBO  // Which to delete?
        );
        IBO = 0;
    }

    if (VBO != 0)
    {
        // Cleaning the buffers.
        glDeleteBuffers(
            1, // How many to delete?
            &VBO  // Which to delete?
        );
        VBO = 0;
    }

    if (VAO != 0)
    {
        // Cleaning the array.
        glDeleteVertexArrays(
            1, // How many to delete?
            &VAO  // Which to delete?
        );
        VBO = 0;
    }

    indexCount = 0;
}
