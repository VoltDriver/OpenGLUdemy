#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

Texture::~Texture()
{
	ClearTexture();
}

void Texture::LoadTexture()
{
	// Loading the texture data in array. Note, array of char is like an array of bytes (each char is a byte)
	unsigned char* texData = stbi_load(fileLocation, // Location of the texture file.
		&height, // Pass by reference. Will be filled with loaded texture height.
		&width, // Pass by reference. Will be filled with loaded texture width.
		&bitDepth, // Pass by reference. Will be filled with loaded texture bitDepth.
		0 // Always 0.
	); 
	if (!texData) // If there is no texData.
	{
		printf("Failed to find: %s\n", fileLocation);
		return;
	}

	glGenTextures(1, &textureID); // Generating a new texture with our id.
	glBindTexture(GL_TEXTURE_2D, textureID); // Binding our texture, with type 2D.

	// Setting texture parameters
	glTexParameteri(GL_TEXTURE_2D, // Type of Texture
		GL_TEXTURE_WRAP_S, // Which parameter to change. Here, we change the way it wraps around the x-axis of the image.
		GL_REPEAT // Repeats the texture.
	);
	glTexParameteri(GL_TEXTURE_2D, // Type of Texture
		GL_TEXTURE_WRAP_T, // Which parameter to change. Here, we change the way it wraps around the x-axis of the image.
		GL_REPEAT // The value. Repeats the texture, in this case.
	);
	glTexParameteri(GL_TEXTURE_2D, // Type of Texture
		GL_TEXTURE_MIN_FILTER, // Which parameter to change. Here, we change the way the texture interacts when we zoom out on it.
		GL_LINEAR // The value. Here we want to blend the texture.
	);
	glTexParameteri(GL_TEXTURE_2D, // Type of Texture
		GL_TEXTURE_MAG_FILTER, // Which parameter to change. Here, we change the way the texture interacts when we zoom in on it.
		GL_LINEAR // The value. Here we want to blend the texture.
	);

	// Loading the texture to graphics card.
	glTexImage2D(GL_TEXTURE_2D, // Texture target, what the texture was bound to.
		0, // Mimap level. This will define the texture for mipmap level 0. Usually, defining for 0 will use same texture across whole range.
		GL_RGBA,  // Format of stored data. If you're not using RGBA, use RGB.
		width, // Width of texture
		height, // Height of texture
		0, // Always 0. Legacy stuff that is not used anymore.
		GL_RGBA, // Format of data being loaded. If you're not using RGBA, use RGB.
		GL_UNSIGNED_BYTE, // Data type of the values.
		texData // The data itself.
	);

	// Generate the mipmaps, automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	// For safety, discarding the raw data.
	stbi_image_free(texData);
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE1 // Texture unit to use. Here, unit 1.
	);
	// Binding our texture to the texture unit above.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Now whenever we draw texture unit 0, we draw our texture.
}

void Texture::ClearTexture()
{
	// Delete texture from memory
	glDeleteTextures(1, &textureID);

	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}
