
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window Size
const GLint WIDTH = 800, HEIGHT = 600;
// Value to be able to convert degrees to radians. If we multiply a degree by this, it will output it in radians.
const float toRadians = 3.14159265f / 180.0f;

// The objects we are going to use
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

bool direction = true; // Direction parameter (for left or right). Here True is right, False is left.
float triOffset = 0.0f; // Offset of the triangle
float triMaxOffset = 0.7f; // Maximum offset it can move.
float triIncrement = 0.005f; // How much to move the offset.

float curAngle = 0.0f; // Current angle. used to rotate continuously.

bool sizeDirection = true; // Sets the size to increasing or decreasing. True for increasing.
float curSize = 0.4f; // Current size. Used to size up or down continuously.
float maxSize = 0.8f; // Maximum size allowed
float minSize = 0.2f; // Minimum size allowed.

// Vertex Shader path
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader path
static const char* fShader = "Shaders/shader.frag";

// Here we are actually drawing a pyramid now.
void CreateObjects() 
{
    // Creating the order of draws for indexed draws.
    // This uses the position of the vertices in vertices[]. By specifying them in order, we define the drawing order of those points.
    // So 0,3,1 means draw point at index 0, then at index 3, then at index 1.
    // Note we are specifying them in counter clockwise order. This is to say to opengl that we are drawing front faces.
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2 // Base of the pyramid.
    };

    // The center of the screen is 0.0f. It also goes from -1.0 to 1.0.
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // Create a pointer so that the object is not destroyed when exiting function.
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj2);
}

// Creates the shader program, combining Vertex shader and Fragment shader/
void CreateShader() {
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(shader1);
}

int main()
{
    Window mainWindow = Window(WIDTH, HEIGHT);
    mainWindow.Initialize();

    // Create the actual objects to draw.
    CreateObjects();
    // Create the shaders.
    CreateShader();

    GLuint uniformModel = 0; // a uniform variable that will be used to move the object.
    GLuint uniformProjection = 0; // a uniform variable to move vertices according to how they are viewed. Project them so they look like they have depth.

    // Creating the projection matrix. For now, this is outside the while loop because it will never change.
    // If we wanted it to change (such as with a moving camera) it would have to be further down.
    glm::mat4 projection = glm::perspective(
        45.0f, // FOV in the Y direction.
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), // Aspect. Width of window divided by height of window. If we use SDL (like we are), we need to use our buffer dimensions. Also, has to be GL_FLOAT.
        0.1f, // Near view plane distance
        100.0f
     );

    // Loop until window closed
    // This function contains a value that indicates whether the window should close.
    while (!(mainWindow.getShouldClose() == GL_TRUE))
    {
        // Get + Handle user input events
        // This function checks if any event has happened (key presses, resizing window (?) , clicks, etc.
        // It checks for the events, and then handles them the way it was programmed to.
        glfwPollEvents();

        // If we are going to the right
        if (direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        // If we are at the max offset, either left or right, we switch direction.
        if (abs(triOffset) >= triMaxOffset)
        {
            direction = !direction;
        }

        // Rotating continuously.
        curAngle += 0.1f;
        if (curAngle >= 360)
        {
            curAngle = 0;
        }

        // Increasing and decreasing dimensions continuously.
        if (sizeDirection)
        {
            curSize += 0.001f;
        }
        else
        {
            curSize -= 0.001f;
        }

        // If we are at max or min size, switch the scale direction.
        if (curSize >= maxSize || curSize <= minSize)
        {
            sizeDirection = !sizeDirection;
        }

        // Clear window. Clears the entire screen. Set a color to clear it to.
        // This is in 0 to 1 scale. Take any RGB, divide by 256, and you get your value.
        // Alpha with value of 1 is opaque. 0 is invisible.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Pixels have many variables linked to them. We just want to clear their color attribute, and their depth attribute.
        // We use the Bitwise OR Operator (|) to compine these 2 features of clear.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // This will tell the graphics card to use the shader program that has the ID specified.
        shaderList[0]->UseShader();
            uniformModel = shaderList[0]->GetModelLocation();
            uniformProjection = shaderList[0]->GetProjectionLocation();
            // We indent here to show that we are using the shader that is above until it is no longer in use.
            
            glm::mat4 model(1.0f); // creating a 4x4 matrix, to identity 

            // The translation and Rotation happen in REVERSE order than listed in the code. So rotate will be first.

            // Calculating the translation
            // We apply a translation to the identity matrix. We want to move the pyramid away from the camera.
             model = glm::translate(
                model, // Matrix we want to perform the calculations on
                glm::vec3(triOffset, triOffset, -2.5f) // Passing a vec3 with the 3 values we want to make the transformation be
            );

            // Rotating our model
                model = glm::rotate(
                model, // What we are applying the rotation to
                curAngle * toRadians, // The value to rotate by, in radians
                glm::vec3(0.0f, 1.0f, 0.0f) // Axis of rotation. We rotate around y now. Basically, the vector is a line that starts from origin. It goes outwards on the Z axis, away from camera.
                // We rotate around that line.
                // The rotation will take place at the origin relative to the object.
                );

            // Scaling up or down our model.
            model = glm::scale(
                model, // what we are scaling
                glm::vec3(0.4f, 0.4f, 1.0f) // the axises we are scaling, and by how much. Here we say 2x, 2y, 1z (which keeps scale)
            );

            // Reassigning the uniform variable. So now we want to assign a matrix, 4x4, with float values.
            glUniformMatrix4fv(uniformModel, // Value to change
                1, // How many matrices to pass
                GL_FALSE, // Transpose?
                glm::value_ptr(model)); // Our value. Can't pass our value directly. We need to use a pointer.

            // Reassigning the uniform variable. So now we want to assign a matrix, 4x4, with float values.
            glUniformMatrix4fv(uniformProjection, // Value to change
                1, // How many matrices to pass
                GL_FALSE, // Transpose?
                glm::value_ptr(projection)); // Our value. Can't pass our value directly. We need to use a pointer.

            glUniform1f(uniformModel, triOffset);

            meshList[0]->RenderMesh();

            model = glm::mat4(1.0f);

            model = glm::translate(
                model, // Matrix we want to perform the calculations on
                glm::vec3(-triOffset, 1.0f, -2.5f) // Passing a vec3 with the 3 values we want to make the transformation be
            );

            model = glm::scale(
                model, // what we are scaling
                glm::vec3(0.4f, 0.4f, 1.0f) // the axises we are scaling, and by how much. Here we say 2x, 2y, 1z (which keeps scale)
            );

            glUniformMatrix4fv(uniformModel, // Value to change
                1, // How many matrices to pass
                GL_FALSE, // Transpose?
                glm::value_ptr(model)); // Our value. Can't pass our value directly. We need to use a pointer.

            meshList[1]->RenderMesh();


            // We stop indenting, because we no longer use the VAO.

            // We stop using the shader.
            glUseProgram(0);
        // We are no longer indented, because we stopped using the shader.

        // Double buffering. We use 2 buffers. Draw on the one that isnt visible, then swap it with the visible one.
        mainWindow.swapBuffers();
    }
}