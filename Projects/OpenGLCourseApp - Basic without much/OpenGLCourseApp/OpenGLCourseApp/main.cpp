
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

// Window Size
const GLint WIDTH = 800, HEIGHT = 600;

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
    // OpenGL Version
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
        // Alpha of 1 is opaque. 0 is invisible.
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        // Pixels have many variables linked to them. We just want to clear their color attribute.
        glClear(GL_COLOR_BUFFER_BIT);

        // Double buffering. We use 2 buffers. Draw on the one that isnt visible, then swap it with the visible one.
        glfwSwapBuffers(mainWindow);
    }
    
    std::cout << "Hello World!\n";
}