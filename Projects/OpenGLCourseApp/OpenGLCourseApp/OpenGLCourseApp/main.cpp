
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

// Window Size
const GLint WIDTH = 800, HEIGHT = 600;

// The objects we are going to use to draw our single triangle.
// If you would draw multiple objects, you'd need multiple VAOs and VBOs
GLuint VAO, VBO, shader;

// Vertex Shader
// This shader uses GLSL. It looks a lot like C++ code.
// First in the shader, we set the version. Here 330 means 3.3.
// Then, we specify the attribute "pos" for the position of vertices, 
// and set it to location 0. We also say it's an input ("in") and that it's a vector3.
// Then we work with gl_Position, a variable that handles the final position of our vertex. We tell it to use pos.
// It has 4 values inside the vector because it uses the homogeneous system. We use 1.0 as the last value because it's a point.

static const char* vShader = "                              \n\
 #version 330                                               \n\
                                                            \n\
 layout (location = 0) in vec3 pos;                         \n\
                                                            \n\
 void main()                                                \n\
{                                                           \n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);           \n\
}";

// Fragment Shader
// Same as above.
// We first set an output value, which is the color of our fragments. Usually this is the only output. 
// Do note the name of the output value doesnt matter.
// Then we set its color. This is RGB, but in a 0 to 1 scale. So divide an RGB value by 256 and you get this.
// The last value is the Alpha.
static const char* fShader = "                              \n\
 #version 330                                               \n\
                                                            \n\
 out vec4 color;                                            \n\
                                                            \n\
 void main()                                                \n\
{                                                           \n\
    color = vec4(1.0f, 0.0f, 0.0f, 1.0);                    \n\
}";

void CreateTriangle() 
{
    // The center of the screen is 0.0f. It also goes from -1.0 to 1.0.
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // Creating our VAO. 1- Amount of arrays and then 2- Where to store the ID of the array.
    // This now creates some stuff in the graphics card and its memory.
    glGenVertexArrays(1, &VAO);
    // Binding. Now all our operations that interact with Vertex Array will interact with this array.
    glBindVertexArray(VAO);
    // We now Indent, because this shows that everyting that is indented will work with the array object bound above.

        // Same as above, but for buffers.
        glGenBuffers(1, &VBO);
        // Binding. First choose the target to bind to. VBO has multiple targets it can bind to.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Connect the vertices we created to the VBO
        glBufferData(GL_ARRAY_BUFFER, // Target
            sizeof(vertices), // the size of the data we are passing in. could also have said sizeof(GLfloat * 9)
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

    // And now we are not indented anymore! Because we have unbound our vertex array.

}

// This adds a shader to our Shader program. So it takes the code for a shader, and then the type of shader it is, and
// adds that shader to the specified program.
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    // Create the individual shader.
    // The shader will be empty, and of the type we passed.
    // We saved the returned ID.
    GLuint theShader = glCreateShader(shaderType);

    // Creating an array pointer, pointing to the first element.
    // We are going to store the code here.
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    // Defining length of the code.
    GLint codeLength[1];
    // We use strlen to get the exact length of our code.
    codeLength[0] = strlen(shaderCode);

    // Setup the shader source code
    glShaderSource(theShader, // Shader we will be modifying the code for
        1, // This is for multiple strings. We could have a bunch of chunks of code, and combine them. But we only have one file.
        theCode, // Code to be put in our shader
        codeLength // Length of the code to be put in our shader
    );

    // Compile the shader!
    glCompileShader(theShader);

    // Now we need to verify it compiled properly.

    // These are debug variables. We will use these to detect errors that come from the GPU when compiling shaders and stuff.
    GLint result = 0; // This will store the result of our operations
    GLchar eLog[1024] = { 0 }; // This will store the error message. We first initialize it to be empty.

    // Get info on the program to make sure it's linked properly
    glGetShaderiv(theShader, // the program to check 
        GL_COMPILE_STATUS, // Which info we want to gather. Here, we want the compile status, to see if it is compiled successfully or not.
        &result // Where to store the result of our query.
    );

    // We now check if result is false. If it is, it means our program is not compiled correctly.
    if (!result)
    {
        // We fetch the error.
        glGetShaderInfoLog(
            theShader, // Program to get the error from
            sizeof(eLog), // Size of the variable to store the error information in
            NULL,
            eLog // Our error log that will contain the error.
        );

        // Print out the error to the console. So we print the content of eLog, and which type of shader failed.
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    // This will attach the shader, which is now compiled, to the shader program.
    glAttachShader(theProgram, theShader);

    return;
}

void CompileShaders()
{
    // Creating the shader program. This will handle our shaders, created earlier as strings.
    // So now this creates an empty shader program. It has nothing in yet, but we will put in our custom shaders in a moment.
    shader = glCreateProgram();

    // Here we check if we got our program successfully created. If there is nothing in shader, then we have an error.
    if (!shader) {
        printf("Error creating shader program.\n");
        return;
    }

    // Adding our shaders to the program. This method will also compile the shaders.
    // shader is our program, vShader is the code for the vertex shader we created, and the last value is the type of shader it is.
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    // Same as above, but for the fragment shader.
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    // These are debug variables. We will use these to detect errors that come from the GPU when compiling shaders and stuff.
    GLint result = 0; // This will store the result of our operations
    GLchar eLog[1024] = { 0 }; // This will store the error message. We first initialize it to be empty.

    // Create the executable for the shader program and link it correctly to make sure it all works.
    // Essentially, this takes our shader program, links it together like a C++ linker would do, and gives it to the graphics card.
    glLinkProgram(shader);
    // Get info on the program to make sure it's linked properly
    glGetProgramiv(shader, // the program to check 
        GL_LINK_STATUS, // Which info we want to gather. Here, we want the Link status, to see if it is linked or not.
        &result // Where to store the result of our query.
    );
    
    // We now check if result is false. If it is, it means our program is not linked correctly.
    if (!result)
    {
        // We fetch the error.
        glGetProgramInfoLog(
            shader, // Program to get the error from
            sizeof(eLog), // Size of the variable to store the error information in
            NULL,
            eLog // Our error log that will contain the error.
        );

        // Print out the error to the console. So we print the content of eLog.
        printf("Error linking shader program: '%s'\n", eLog);
        return;
    }

    // Check if the shader program we created is valid in the current OpenGL context.
    glValidateProgram(shader);

    // Get info on the program to make sure it's validated properly
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    // Check if it validated properly. Just like above.
    if (!result)
    {
        // We fetch the error.
        glGetProgramInfoLog(shader,sizeof(eLog), NULL, eLog);

        // Print out the error to the console. So we print the content of eLog.
        printf("Error validating shader program: '%s'\n", eLog);
        return;
    }
}

int main()
{
    // Initialize GLFW
    // This initializes GLFW and make it ready to create windows.
    // The If checks if it succeeded.
    if (!glfwInit())
    {
        printf("GLFW Initialization failed.");

        // If GLFW initialized anything, we terminate them to clean up.
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties

    // Setting the OpenGL Version
    // this function sets certain properties of the GLFW window.
    // First you pass an enum, defining the parameter you want to change, then the value you want to change to.
    // Here we are changing the version to 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Profile says to opengl how to treat the code its passed. Core means not backwards compatible, i.e. no deprecated features.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Allows forward compatibility.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Creating window. The 2 nulls are first the monitor to use, and the other one if we want the window to be shared across systems.
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "TestWindow", NULL, NULL);
    // Check if window was created successfully
    if (!mainWindow)
    {
        printf("GLFW window creation failed.");
        glfwTerminate();
        return 1;
    }

    /* Get buffer size information. This is like the middle part of the window, the part that can
     contain drawing. This also helps determine our viewport size, where things are drawn. */
    int bufferWidth, bufferHeight;
    // This gets the values and puts them in our variables.
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use. So we say when stuff gets drawn, it should get drawn to a certain window.
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features.
    // This allows access to extensions for OpenGL.
    glewExperimental = GL_TRUE;

    // Initalizes glue and check for errors
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW Initialization failed.");
        // The window was created at this point, so we have to destroy it.
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup Viewport size
    // Sets the size of the part we can view / draw to.
    // First 2 params are the top left coordinate.
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Create the actual triangle to draw.
    CreateTriangle();
    // Compile the Vertex and Fragment Shaders
    CompileShaders();

    // Loop until window closed
    // This function contains a value that indicates whether the window should close.
    while (!(glfwWindowShouldClose(mainWindow) == GL_TRUE))
    {
        // Get + Handle user input events
        // This function checks if any event has happened (key presses, resizing window (?) , clicks, etc.
        // It checks for the events, and then handles them the way it was programmed to.
        glfwPollEvents();

        // Clear window. Clears the entire screen. Set a color to clear it to.
        // This is in 0 to 1 scale. Take any RGB, divide by 256, and you get your value.
        // Alpha with value of 1 is opaque. 0 is invisible.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Pixels have many variables linked to them. We just want to clear their color attribute.
        glClear(GL_COLOR_BUFFER_BIT);

        // This will tell the graphics card to use the shader program that has the ID specified.
        glUseProgram(shader);
            // We indent here to show that we are using the shader that is above until it is no longer in use.
            
            // We want to work with our created VAO.
            glBindVertexArray(VAO);
                // We indent to show we are working with this VAO from here on.  
                
                // Drawing our triangles.
                glDrawArrays(GL_TRIANGLES, // The mode we are using it in. Defines how to draw our VAO.
                    0, // Where in the array should we start (we want the first value, which is our first point)
                    3 // Amount of points we want to draw.
                );

                // We unbind the VAO.
                glBindVertexArray(0);
            // We stop indenting, because we no longer use the VAO.

            // We stop using the shader.
            glUseProgram(0);
        // We are no longer indented, because we stopped using the shader.

        // Double buffering. We use 2 buffers. Draw on the one that isnt visible, then swap it with the visible one.
        glfwSwapBuffers(mainWindow);
    }
}