#pragma once

#include "wrapper_glfw.h"
#include <glm/glm.hpp>


struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; //colour
	float size, angle, weight;
	float life; //remaining life of the particle. if <0 : dead and unused
	float cameradistance;

	bool operator<(const Particle& that) const {
		//sort in reverse order : far particles drawn first
		return this->cameradistance > that.cameradistance;
	}
};

class ParticleSystem
{
public: 
	ParticleSystem();
	~ParticleSystem();

	int findUnusedParticle();
	void sortParticles();
	void makeParticleSystem(GLuint program);
	void drawParticleSystem(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);
	void defineUnions();




	int maxParticles;
	int LastUsedParticle;
	static GLfloat* g_particle_position_size_data;
	static GLubyte* g_particle_color_data;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	Particle ParticlesContainer[10000];
	double lastTime;

	GLuint Texture;
	GLuint TextureID;
	GLuint ViewProjMatrixID;
	GLuint programID;
	GLuint CameraRight_worldspace_ID;
	GLuint CameraUp_worldspace_ID;
	GLuint VertexArrayID;
	GLuint billboard_vertex_buffer;

};

