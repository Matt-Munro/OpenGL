#pragma once

#include "wrapper_glfw.h"
#include "stb_image.h"

class Texture
{
public: 
	Texture();
	~Texture();

	bool loadTexture(const char* filename, bool bGenMipmaps);
	void useTexture();
	void setUniform(GLuint program, const char* name);

	//GLuint texture;
	GLuint textureID;
};

