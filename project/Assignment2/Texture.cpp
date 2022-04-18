//Adapted load texture function from lab example to be within a class

#include "Texture.h"
#include <iostream>

Texture::Texture()
{}

Texture::~Texture()
{}

bool Texture::loadTexture(const char* filename, bool bGenMipmaps)
{
	glGenTextures(1, &textureID);
	// local image parameters
	int width, height, nrChannels;

	/* load an image file using stb_image */
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	//unsigned char* data = SOIL_load_image(filename, &width, &height, &nrChannels, 0);

	// check for an error during the load process
	if (data)
	{
		// Note: this is not a full check of all pixel format types, just the most common two!
		int pixel_format = 0;
		if (nrChannels == 3)
			pixel_format = GL_RGB;
		else
			pixel_format = GL_RGBA;

		// Bind the texture ID before the call to create the texture.
			// texID[i] will now be the identifier for this specific texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Create the texture, passing in the pointer to the loaded image pixel data
		glTexImage2D(GL_TEXTURE_2D, 0, pixel_format, width, height, 0, pixel_format, GL_UNSIGNED_BYTE, data);

		// Generate Mip Maps
		if (bGenMipmaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			// If mipmaps are not used then ensure that the min filter is defined
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		printf("success ");
	}
	else
	{
		printf("stb_image  loading error: filename=%s", filename);
		return false;
	}

	std::cout << "texID: " << textureID << std::endl;

	stbi_image_free(data);

	return true;
}

void Texture::useTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//glUniform1i(textureID, 0);
}

void Texture::setUniform(GLuint program, const char* name)
{
	int loc = glGetUniformLocation(program, name);
	if (loc >= 0) glUniform1i(loc, 0);
}