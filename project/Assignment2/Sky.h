/*
	Modified example to be used as sky sphere
	normals are explicitly set and direction reversed to point inwards.
*/

/* sphere.h
 Example class to create a generic sphere object with texture coordinates
 Resolution can be controlled by setting number of latitudes and longitudes
 Iain Martin November 2019
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Sky
{
public:
	Sky(bool useTexture = true);
	~Sky();

	void makeSky(GLuint numlats, GLuint numlongs);
	void drawSky();

	// Define vertex buffer object names (e.g as globals)
	GLuint skyBufferObject;
	GLuint skyNormals;
	GLuint skyColours;
	GLuint skyTexCoords;
	GLuint elementbuffer;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;
	GLuint attribute_v_texcoord;

	unsigned int numskyvertices;
	unsigned int numlats;
	unsigned int numlongs;
	unsigned int drawmode;
	bool enableTexture;

private:
	void makeUnitSky(GLfloat* pVertices, GLfloat* pTexCoords);
};
