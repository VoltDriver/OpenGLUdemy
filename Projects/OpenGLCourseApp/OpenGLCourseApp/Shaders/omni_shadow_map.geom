#version 330

// OMNI SHADOW MAP GEOMETRY SHADER

layout (triangles) in; // Type of primitive coming in.
layout (triangle_strip, max_vertices=18) out; // Triangle strip : Like multiple triangles, but always reuse the last 2 points (of the last triangle) to make the new triangle. They are all stuck together.
// We define this as what is coming out, with a max of 18 vertices.

uniform mat4 lightMatrices[6]; // Combination of view and projection matrices of light source.

out vec4 fragPos;

void main()
{	
	for(int face = 0; face < 6; face++) // Going through each side of our cube.
	{
		gl_Layer = face; // gl_Layer specifies which of the 6 textures of the cubemap we want to output to. So now we will draw to that face with EmitVertex.
		for(int i =0; i < 3; i++) // Going through each vertices of the triangles we've been passed.
		{
			fragPos = gl_in[i].gl_Position; // gl_in is our triangles.
			gl_Position = lightMatrices[face] * fragPos; // So we get the fragment position in relation to the view from the light.
			EmitVertex(); // Emits this vertex at this position on the specified layer.
		}		
		EndPrimitive(); // Finished drawing our triangle, going to another one.
	}
}