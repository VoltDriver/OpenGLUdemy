#version 330

// FRAGMENT SHADER

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in vec4 directionalLightSpacePos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3; // Has to be the same value as in the CommonValues.h file.

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base; // Somewhat inheritance of the Light struct.
	vec3 direction;
};

struct PointLight 
{
	Light base; // Somewhat inheritance of the Light struct.
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS]; // Shadow maps for point lights and spotlights

uniform Material material;

// Position of the eye, of the camera
uniform vec3 eyePosition;

// Predefined sample positions to approximate shadow values using the OmniShadowMap. This is PCF.
// This covers all main key directions. Avoids doing too many samples which slows down our app.
vec3 sampleOffsetDirections[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	// Doing a special operation to get the coordinate system we need, to make them between -1 and 1.
	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
	// Converting to a 0 to 1 scale for the shadow map.
	projCoords = (projCoords * 0.5) + 0.5;
	
	float currentDepth = projCoords.z; // How far away the point is from the light, forwards and backwards.
	
	// Setting up the shadow bias, to avoid shadow acne phenomenon
	vec3 newNormal = normalize(normal);
	vec3 lightDir = normalize(directionalLight.direction); // Dunno if it's directionalLight.direction or light.direction.
	
	float bias = max(0.05 * (1.0 - dot(newNormal, lightDir)), 0.0005);
	
	// Doing PCF to make the shadows look smoother
	float shadow = 0.0;
	
	// This gives us the size of 1 texel.
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	// Now we want to move around to get the average of all texels around our point. to do PCF.
	// We are iterating from -1 to 1, with 0 as our middle coordinate.
	// Increasing our min value (-1) for x and y will give higher quality of PCF, but will be exponentially more costly on performance.
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			// So this goes into our shadow map, and takes the texture there at the point we are. But we add to that point our CURRENT x and y coords of the for loop (because we are evaluating points around right?)
			// and we do that for our calculated texel size to get what ONE texel on the shadowmap is.
			// Using orthogonal view from light source, so only XY will work on our texture. .r means first value, could use .x but standard is .r.
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			
			// Adding this texel value to shadow.
			
			// Lifting up the point to slightly above whats on the shadowmap if they are directly on the shadow map.
			shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0; // Here, 1.0 is full shadow. 0.0 is no shadow.
		}
	}
	
	// Doing the average of the pixels we went over in the previous for loop.
	shadow /= 9.0; // 9 because 3 rows (x goes -1, 0, 1) and 3 cols (y goes -1, 0, 1). So 3x3.
	
	if(projCoords.z > 1.0) // If point is beyond the far plane of our frustum
	{
		shadow = 0.0; // No shadow. Dont do any shadows past our far plane.
	}
	
	return shadow;
}

float CalcOmniShadowFactor(PointLight light, int shadowIndex)
{
	vec3 fragToLight = fragPos - light.position; // Vector going from fragment to light
	float currentDepth = length(fragToLight);
	
	float shadow = 0.0;
	float bias = 0.05;
	float samples = 20; // Amount of samples to take in.
	// Calculating disk radius
	// Distance in each direction to go into, using the samples.
	// Calculated using the camera position so that the closer we get, the more blurred we want it.
	float viewDistance = length(eyePosition - fragPos); // distance between camera and frag we are rendering
	float diskRadius = (1.0 + (viewDistance /  omniShadowMaps[shadowIndex].farPlane)) / 25.0; // Scaling the value
	
	for(int i = 0; i < samples; i++)
	{
		float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r; // This samples in the direction by using xyz from our loops.
		closestDepth *= omniShadowMaps[shadowIndex].farPlane; // Reconverting from the 0 to 1 scale to the actual scale according to our far plane. See the omni shadow map code.
		if((currentDepth - bias) > closestDepth)
		{
			shadow += 1.0;
		}
	}
	
	shadow /= float(samples); // Taking the average of the samples we took. Here, we do number of samples cubed.
	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

	// Calculating diffuse lighting
	
	// Calculating the value between 0 and 1 that gives how much diffuse light there is, as in how dark the surface gets.
	// We normalize so that the dot product ONLY takes into account the cos of the angle between the vectors. Normalizing removes the magnitude of each vector from the equation.
	// And we want no negative values. At minimum, the factor can be 0 (dark).
	float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
	
	vec4 diffuseColor = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);
	
	// Initialize a specular color as "null". If used at this point, will do nothing.
	vec4 specularColour = vec4(0, 0, 0, 0);
	
	// If we have diffuse lighting... Then we apply specular lighting. Can't have specular without diffuse.
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragPos);
		// Vector of where the light is reflected around the normal. We want light to bounce off on the other side of the normal.
		// Reflect does this. First argument is what we want to reflect, and the second is what we are reflecting around.
		vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));
		
		// Now we find the angle between those 2 vectors. This is why we normalized both of those vectors, so we can get the cos between the two.
		float specularFactor = dot(fragToEye, reflectedVertex);
		
		if(specularFactor > 0.0f)
		{
			// Pow applies a power to a number.
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	return (ambientColour + (1.0 - shadowFactor) * (diffuseColor + specularColour));
}

vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
	// Getting vector from point light to fragment.
		vec3 direction = fragPos - pLight.position;
		float distance = length(direction);
		direction = normalize(direction);
		
		float shadowFactor = CalcOmniShadowFactor(pLight, shadowIndex);
			
		vec4 colour = CalcLightByDirection(pLight.base, direction, shadowFactor);
		
		// Formula to calculate attenuation. See theory.
		float attenuation = pLight.exponent * distance * distance +
							pLight.linear * distance +
							pLight.constant;
		return (colour / attenuation);
}


vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	
	// Loop over point lights and add them to total colour.
	for(int i =0; i < pointLightCount; i++)
	{
		totalColour += CalcPointLight(pointLights[i], i); // 1 to 1 relation between point light index and shadow index.
	}
	
	return totalColour;
}

void main()
{	
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	
	colour = texture(theTexture, texCoord) * finalColour;
}