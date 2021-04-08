#version 330

// OMNI SHADOW MAP FRAGMENT SHADER

in vec4 fragPos;

uniform vec3 lightPos;
uniform float farPlane; // How far we want to render.

void main()
{	
	float distance = length(fragPos.xyz - lightPos); // distance between the fragment we are trying to render and the light
	distance = distance / farPlane; // distance normalized, according to the farplane. Will output between 0 and 1.
	gl_FragDepth = distance;
}