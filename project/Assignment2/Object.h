#pragma once

#include "wrapper_glfw.h"
#include "glm/glm.hpp"
#include <vector>

class Object
{

public:
	Object();
	~Object();

	void loadFileObj(const char* filePath);
	void drawObject();
	void setColour(glm::vec4 colours);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colours;

	GLuint vertexBuffer;
	GLuint UVbuffer;
	GLuint colourBuffer;
	GLuint normalBuffer;
	GLuint MatrixID;
};

