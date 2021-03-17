#pragma once

#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	/// <summary>
	/// Returns the array of keys and their pressed status.
	/// </summary>
	/// <returns>The array of key statuses.</returns>
	bool* getKeys() { return keys; }

	// Oversimplification of resetting the change of the mouse position if the mouse doesnt move.
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024]; // Keeps the status of pressed of each key on the keyboard.

	GLfloat lastX; // Last coordinate of the mouse, X axis
	GLfloat lastY; // Last coordinate of the mouse, Y axis
	GLfloat xChange; // The difference between the current position of the mouse and its last position, X axis
	GLfloat yChange; // The difference between the current position of the mouse and its last position, Y axis
	bool mouseFirstMoved; // Initial movement of the mouse. Check if the movement of the mouse was the first movement. True if has never moved.

	/// <summary>
	/// Binds a function that will be called back when a key is pressed in the window or when the mouse is used
	/// </summary>
	void createCallbacks();

	/// <summary>
	/// Method to be used as callback for handling keys. Has to be static.
	/// Static because GLFW cant use member functions as callback.
	/// </summary>
	/// <param name="window">The window that was targeted with the keypress</param>
	/// <param name="key">Number of the key (GLFW enum)</param>
	/// <param name="code"></param>
	/// <param name="action">Action of the key (press, release etc.)</param>
	/// <param name="mode"></param>
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);

	/// <summary>
	/// Function called in callback when the mouse is moved in the window.
	/// </summary>
	/// <param name="window">The window in focus</param>
	/// <param name="xPos">New coordinate of the mouse, x position</param>
	/// <param name="yPos">New coordinate of the mouse, y position</param>
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

};