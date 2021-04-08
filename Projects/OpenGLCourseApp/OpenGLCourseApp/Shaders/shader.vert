#version 330

// VERTEX SHADER

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;
out vec2 texCoord;
out vec3 normal;
// Position of the fragment
out vec3 fragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	texCoord = tex;
	
	// Here, we put model to multiply because if our object moves around, the normal has to move to. Normal is in relation to where the model is.
	// But the normal doesnt really change direction.... unless we rotate or scale an object in one direction. Then, the normal will change and have to be recalculated.
	// Because rotation and scale happens in the first 3x3 matrix inside the mat4, we convert our mat4 to a mat3 to only keep rotation and scale values.
	// But also, the rotation changes our norm in a way we want to, but the scaling changes the normal in a way we dont want to!
	// So we have to avoid scaling, while taking in rotation. The trick to do that is to transpose and inverse the matrix, this reverses the scaling.
	normal = mat3(transpose(inverse(model))) * norm;
	
	// Just gives you where it is in the world, without taking into care the "view" or camera
	// We don't need a vec4 though, we need a vec3. So we use Swizzling (using .xyz at the end).
	// Swizzling will create a vector only using the variables we put in at the end. So will create a vec3(x, y, z).
	// We could also do sampleVector.xyy to get a vec3(x, y, y).
	fragPos = (model * vec4(pos, 1.0)).xyz;
}