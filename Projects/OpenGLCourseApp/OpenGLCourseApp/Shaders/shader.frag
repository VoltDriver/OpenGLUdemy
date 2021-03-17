#version 330

// FRAGMENT SHADER

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 colour;

struct DirectionalLight 
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

// Position of the eye, of the camera
uniform vec3 eyePosition;

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	
	// Calculating diffuse lighting
	
	// Calculating the value between 0 and 1 that gives how much diffuse light there is, as in how dark the surface gets.
	// We normalize so that the dot product ONLY takes into account the cos of the angle between the vectors. Normalizing removes the magnitude of each vector from the equation.
	// And we want no negative values. At minimum, the factor can be 0 (dark).
	float diffuseFactor = max(dot(normalize(normal), normalize(directionalLight.direction)), 0.0f);
	
	vec4 diffuseColor = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	// Initialize a specular color as "null". If used at this point, will do nothing.
	vec4 specularColour = vec4(0, 0, 0, 0);
	
	// If we have diffuse lighting... Then we apply specular lighting. Can't have specular without diffuse.
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragPos);
		// Vector of where the light is reflected around the normal. We want light to bounce off on the other side of the normal.
		// Reflect does this. First argument is what we want to reflect, and the second is what we are reflecting around.
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(normal)));
		
		// Now we find the angle between those 2 vectors. This is why we normalized both of those vectors, so we can get the cos between the two.
		float specularFactor = dot(fragToEye, reflectedVertex);
		
		if(specularFactor > 0.0f)
		{
			// Pow applies a power to a number.
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	colour = texture(theTexture, texCoord) * (ambientColour + diffuseColor + specularColour);
}