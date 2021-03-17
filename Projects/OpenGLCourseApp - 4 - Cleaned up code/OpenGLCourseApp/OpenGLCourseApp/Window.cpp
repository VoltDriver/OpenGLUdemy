#include "Window.h"

Window::Window()
{
    width = 800;
    height = 600;

    bufferWidth = 0;
    bufferHeight = 0;

    mainWindow = 0;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    bufferWidth = 0;
    bufferHeight = 0;

    mainWindow = 0;
}

Window::~Window()
{
    // The window was created at this point, so we have to destroy it.
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int Window::Initialize()
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
    mainWindow = glfwCreateWindow(width, height, "TestWindow", NULL, NULL);

    // Check if window was created successfully
    if (!mainWindow)
    {
        printf("GLFW window creation failed.");
        glfwTerminate();
        return 1;
    }

    /* Get buffer size information. This is like the middle part of the window, the part that can
     contain drawing. This also helps determine our viewport size, where things are drawn. */
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

    // Enable a feature of openGL.
    // here we Enable Depth Test. This allows OpenGL to figure out how to draw things according to depth,
    // i.e. how to draw things that are on top of each other but at different depths.
    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    // Sets the size of the part we can view / draw to.
    // First 2 params are the top left coordinate.
    glViewport(0, 0, bufferWidth, bufferHeight);

    return 0;
}

GLint Window::getBufferWidth()
{
    return bufferWidth;
}

GLint Window::getBufferHeight()
{
    return bufferHeight;
}

bool Window::getShouldClose()
{
    return glfwWindowShouldClose(mainWindow);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(mainWindow);
}
