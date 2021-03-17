#version 330

// FRAGMENT SHADER

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;

out vec4 colour;

struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	
	// Calculating diffuse lighting
	
	// Calculating the value between 0 and 1 that gives how much diffuse light there is, as in how dark the surface gets.
	// We normalize so that the dot product ONLY takes into account the cos of the angle between the vectors. Normalizing removes the magnitude of each vector from the equation.
	// And we want no negative values. At minimum, the factor can be 0 (dark).
	float diffuseFactor = max(dot(normalize(normal), normalize(directionalLight.direction)), 0.0f);
	
	vec4 diffuseColor = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	colour = texture(theTexture, texCoord) * (ambientColour + diffuseColor);
}