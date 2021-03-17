#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
	public:

		Window();
		Window(GLint windowWidth, GLint windowHeight);
		~Window();

		int Initialize();

		GLint getBufferWidth();
		GLint getBufferHeight();

		bool getShouldClose();

		void swapBuffers();


	private:
		GLFWwindow	*mainWindow;
		GLint width, height;
		GLint bufferWidth, bufferHeight;
};

