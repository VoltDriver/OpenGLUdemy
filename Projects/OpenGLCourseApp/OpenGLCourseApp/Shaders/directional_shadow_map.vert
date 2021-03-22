#version 330

// SHADOW MAP VERTEX SHADER

layout (location = 0) in vec3 pos; // Position of a vertice

uniform mat4 model;
uniform mat4 directionalLightSpaceTransform; // Point of view of the world from the light. This combines projection and view.

void main()
{	
	gl_Position = directionalLightSpaceTransform * model * vec4(pos, 1.0);
}