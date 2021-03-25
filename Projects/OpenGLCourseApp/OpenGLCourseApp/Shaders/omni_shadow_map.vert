#version 330

// OMNI SHADOW MAP VERTEX SHADER

layout (location = 0) in vec3 pos; // Position of a vertice

uniform mat4 model;

void main()
{	
	gl_Position = model * vec4(pos, 1.0);
}