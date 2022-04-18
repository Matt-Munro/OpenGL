/*
	Using lab 5 texture materials as base for program

	Object sources:
	terrain: https://free3d.com/3d-model/snowy-terrain-46334.html
	firepit: https://free3d.com/3d-model/copper-fire-pit-v1--269236.html
	fence:	https://free3d.com/3d-model/wooden-post-and-rail-fence-v1--187971.html

	Texture sources:
	space: https://www.eso.org/public/images/eso0932a/
*/

/*
 pl Solution (Texturing)
 Creates a cube and a sphere class with texture coordinates
 Includes rotation, scaling, translation, view and perspective transformations,
 Objects have normals,colours and texture coordinates defined
 for all vertices.
 Iain Martin November 2018
*/

/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "soil.lib")


/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
#include <stack>

   /* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Object.h"
//#include "sphere_tex.h"
#include "Sky.h"
#include "particle_object.h"
//#include "Plane.h"

/* Include the image loader */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLfloat lookX;
GLfloat lookY;

//Fire particle system variables
int fire_numParticles;
float fire_particleSpread;
float fire_scaler;
int fire_previousNum;

//Weather particle system variables
int weather_numParticles;
float weather_particleSpread;
float weather_scaler;
int weather_previousNum;

//Shader programs
GLuint program;		
GLuint programSky;
GLuint programParticle;
GLuint programfirepit;
GLuint programWeather;

GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
					   our buffer objects */


/* Position and view globals */
//GLfloat angle_x, angle_inc_x, x, 
GLfloat scaler, x, z, y;

GLuint numlats, numlongs;	//Define the resolution of the sphere object

/* Uniforms*/
GLuint modelID, viewID, projectionID;
GLuint MatrixID;
GLuint viewIDSky, projectionIDSky;
GLuint shininessLevelObjID;
GLuint shininessLevel;
GLuint shininessLevelID;
glm::vec3 lightDirection;
GLuint lightDirectionID;
GLuint modelObjID;
GLuint viewObjID;
GLuint projectionObjID;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/
GLuint numspherevertices;

//Sphere sphere;
Sky sky;

//Particle systems
particle_object particleObject;
particle_object weather;
GLuint CameraRight_worldspace_ID;
GLuint CameraUp_worldspace_ID;
GLuint ViewProjMatrixID;
GLuint weather_CameraRight_worldspace_ID;
GLuint weather_CameraUp_worldspace_ID;
GLuint weather_ViewProjMatrixID;

//Textures
Texture groundTexture, skyTexture;

//Objects
Object firepit;
Object terrain;
Object fence;

//Camera Controls
GLfloat vx, vy, vz;
GLfloat angle_x, angle_y, angle_z;
GLfloat transX, transY, transZ;
GLfloat speed;

using namespace std;
using namespace glm;


/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper* glw)
{
	//Camera controls
	vx = vy = vz = 0.f;
	angle_x = angle_y = angle_z = 0.f;
	transX = transY = transZ = 0.f;
	speed = 0.12f;

	//Load firepit object and set colour value
	firepit.loadFileObj("..\\..\\objects\\fire_pit.obj");
	vec4 colour = {
			vec4(0.0f, 0.0f, 0.0f, 1.0f),
	};
	firepit.setColour(colour);


	//Load terrain object and set colour value
	terrain.loadFileObj("..\\..\\objects\\SnowTerrain.obj");
	colour = {
	vec4(1.0f, 1.0f, 1.0f, 1.0f),
	};
	terrain.setColour(colour);


	//Load fence object and set colour value
	fence.loadFileObj("..\\..\\objects\\fence.obj");
	colour = {
		vec4(0.5f, 0.25f, 0.0f, 1.0f),
	};
	fence.setColour(colour);

	//Initialise mouse position values
	lookX = lookY = 0.0f;

	//Set number of particles for fire and weather
	//Stored in x_previousNum to check if number has been increased
	//since the last display function
	fire_previousNum = fire_numParticles = 2500;
	weather_previousNum = weather_numParticles = 800;

	//Initialise particle scaler value
	fire_scaler = 8.f;
	weather_scaler = 65.f;

	//Initialise spread of particle systems
	fire_particleSpread = 4.5f;
	weather_particleSpread = 40.f;

	/* Set the object transformation controls to their initial values */
	x = 0.05f;
	y = 0;
	z = 0;
	angle_x = angle_y = angle_z = 0;
	//angle_inc_x = angle_inc_y = angle_inc_z = 0;
	scaler = 1.f;
	aspect_ratio = 1.3333f;

	numlats = 100;		// Number of latitudes in our sphere
	numlongs = 100;		// Number of longitudes in our sphere

	//Shininess for vertex shaders
	//Set for specific programs later
	shininessLevel = 0;

	lightDirection = vec3(0., 10., 10.);

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	//Create sky sphere object
	sky.makeSky(numlats, numlongs);


	/* Load and build the vertex and fragment shaders */
	try
	{
		programSky = glw->LoadShader("..\\..\\shaders\\sky.vert", "..\\..\\shaders\\sky.frag");
		programfirepit = glw->LoadShader("..\\..\\shaders\\objectTest.vert", "..\\..\\shaders\\objectTest.frag");
		programParticle = glw->LoadShader("..\\..\\shaders\\Particle.vert", "..\\..\\shaders\\Particle.frag");
		programWeather = glw->LoadShader("..\\..\\shaders\\Particle.vert", "..\\..\\shaders\\Particle.frag");
	}
	catch (exception & e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	//Particle system uniforms
	CameraRight_worldspace_ID = glGetUniformLocation(programParticle, "CameraRight_worldspace");
	CameraUp_worldspace_ID = glGetUniformLocation(programParticle, "CameraUp_worldspace");
	ViewProjMatrixID = glGetUniformLocation(programParticle, "VP");
	weather_CameraRight_worldspace_ID = glGetUniformLocation(programWeather, "CameraRight_worldspace");
	weather_CameraUp_worldspace_ID = glGetUniformLocation(programWeather, "CameraUp_worldspace");
	weather_ViewProjMatrixID = glGetUniformLocation(programWeather, "VP");

	//Firepit uniform
	MatrixID = glGetUniformLocation(programfirepit, "MVP");
	modelObjID = glGetUniformLocation(programfirepit, "model");
	viewObjID = glGetUniformLocation(programfirepit, "view");
	projectionObjID = glGetUniformLocation(programfirepit, "projection");
	shininessLevelObjID = glGetUniformLocation(programfirepit, "shininess");
	lightDirectionID = glGetUniformLocation(programfirepit, "ulightDirection");

	//Sky uniforms
	viewIDSky = glGetUniformLocation(programSky, "view");
	projectionIDSky = glGetUniformLocation(programSky, "projection");


	//Texture filepath
	const char* filename1 = "..\\..\\images\\space.jpg";


	// This will flip the image so that the texture coordinates defined in
	// the sphere, match the image orientation as loaded by stb_image
	stbi_set_flip_vertically_on_load(true);

	if (!skyTexture.loadTexture(filename1, false))
	{
		cout << "Fatal error loading texture: " << filename1 << endl;
		exit(0);
	}


	//Get uniform variable location
	//and set to programSky
	glUseProgram(programSky);
	int loc1 = glGetUniformLocation(programSky, "tex1");
	if (loc1 >= 0) glUniform1i(loc1, 0);

                          
	   
	glUseProgram(0);

	//Create particle systems
	particleObject.create(programParticle, fire_numParticles, fire_particleSpread, "..\\..\\images\\fire.png", true, true, 0.5f, 8.f);
	weather.create(programWeather, weather_numParticles, weather_particleSpread, "..\\..\\images\\fire.png", false, false, 60.f, 65.f);
}


void display()
{
	//Background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	//Stack to manage matrix's used
	stack<mat4> model;
	model.push(mat4(1.0f));

	model.top() = rotate(model.top(), -radians(angle_x), vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model.top() = rotate(model.top(), -radians(angle_y), vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model.top() = rotate(model.top(), -radians(angle_z), vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

	model.top() = translate(model.top(), vec3(transX, transY, transZ));
	

	mat4 projection = perspective(radians(65.0f), aspect_ratio, 0.1f, 100.0f);

	// Camera matrix
	mat4 view = lookAt(
		vec3(0, 0, 1), // 0,0,1 in world space
		vec3(lookX, -lookY, 0), // looking at origin until mouse is moved
		vec3(0, 1, 0)  // Head is up
	);

	view = rotate(view, -radians(vx), vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	view = rotate(view, -radians(vy), vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	view = rotate(view, -radians(vz), vec3(0, 0, 1));


	//Specify light direction. Later passed to shaders
	lightDirection = vec3(0., 10., 4.);

	
	//Firepit 
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(-1, -0.9, -1));
		model.top() = scale(model.top(), vec3(scaler * 0.6f, scaler * 0.6f, scaler * 0.6f));

		glUseProgram(programfirepit);

		//Set firepit uniform values
		glUniform1ui(shininessLevelObjID, 18);
		glUniformMatrix4fv(viewObjID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionObjID, 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(modelObjID, 1, GL_FALSE, &model.top()[0][0]);
		glUniform3fv(lightDirectionID, 1, &lightDirection[0]);

		firepit.drawObject();
	}
	model.pop();


	//Terrain 
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(1, -1, 0));
		
		glUseProgram(programfirepit);

		//Set terrain uniforms
		glUniformMatrix4fv(viewObjID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionObjID, 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(modelObjID, 1, GL_FALSE, &model.top()[0][0]);
		glUniform3fv(lightDirectionID, 1, &lightDirection[0]);
		glUniform1ui(shininessLevelObjID, 3);

		terrain.drawObject();

	}
	model.pop();


	//Fence 1
	model.push(model.top());
	{
		model.top() = rotate(model.top(), -radians(15.0f), glm::vec3(0, 1, 0));
		model.top() = translate(model.top(), vec3(-2.5, -0.9, -2.2));
		model.top() = scale(model.top(), vec3(scaler * 0.25f, scaler * 0.25f, scaler * 0.25f));

		glUseProgram(programfirepit);

		//Set fence 1 uniforms
		glUniform1ui(shininessLevelObjID, 1);
		glUniformMatrix4fv(viewObjID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionObjID, 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(modelObjID, 1, GL_FALSE, &model.top()[0][0]);

		fence.drawObject();
	}
	model.pop();


	//Fence 2
	model.push(model.top());
	{
		model.top() = rotate(model.top(), radians(75.0f), glm::vec3(0, 1, 0));
		model.top() = translate(model.top(), vec3(-0.45, -0.925, -3.5));
		model.top() = scale(model.top(), vec3(scaler * 0.25f, scaler * 0.25f, scaler * 0.25f));

		glUseProgram(programfirepit);

		//set fence 2 uniforms
		glUniform1ui(shininessLevelObjID, 1);
		glUniformMatrix4fv(viewObjID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projectionObjID, 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(modelObjID, 1, GL_FALSE, &model.top()[0][0]);

		fence.drawObject();
	}
	model.pop();

	//Sky sphere
	model.push(model.top());
	{
		//Changes depth function to use less than or equal instead of less then
		//Stops skysphere being culled
		glDepthFunc(GL_LEQUAL); 

		glUseProgram(programSky);

		model.top() = translate(model.top(), vec3(0, 0, 0));
		model.top() = scale(model.top(), vec3(scaler * 1.f, scaler * 1.f, scaler * 1.f));//scale equally in all axis

		glUniformMatrix4fv(projectionIDSky, 1, GL_FALSE, &projection[0][0]);

		//Remove translation from view 
		view = mat4(mat3(mat4(lookAt(
			vec3(0, 0, 1), 
			vec3(lookX, -lookY, 0), 
			vec3(0, 1, 0)  
		))));
		glUniformMatrix4fv(viewIDSky, 1, GL_FALSE, &view[0][0]);

		skyTexture.useTexture();
		sky.drawSky();

		//Reset depth function
		glDepthFunc(GL_LESS);
	}
	model.pop();
	

	//Fire
	//TODO currently fire moves with user controls. 
	//Future task would be to resolve this.
	model.push(model.top());
	{
		glUseProgram(programParticle);

		model.top() = translate(model.top(), vec3(-0.6, -0.3, -1.2));

		glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);

		//Store model view matrix and pass to particle draw function 
		mat4 ParticleView = view;
		ParticleView *= model.top();
		
		particleObject.drawParticles(projection, ParticleView);

		//Check if user input has changed the number of particles
		//If so, call function
		if (fire_previousNum != fire_numParticles)
		{
			particleObject.setMaxParticles(fire_numParticles);
		}

		//Store number of particles for checking when display is next called
		fire_previousNum = fire_numParticles;
	}
	model.pop();


	//Weather
	model.push(model.top());
	{
		glUseProgram(programWeather);

		model.top() = translate(model.top(), vec3(0.5, 2, 0));

		//Store model view matrix and pass to particle draw function 
		mat4 ParticleView = view;
		ParticleView *= model.top();

		weather.drawParticles(projection, ParticleView);
	
		//Check if user input has changed the number of particles
		//If so, call function
		if (weather_previousNum != weather_numParticles)
		{
			weather.setMaxParticles(weather_numParticles);
		}

		//Store number of particles for checking when display is next calledcu
		weather_previousNum = weather_numParticles;
	}
	model.pop();


	/* Modify our animation variables */
	//angle_x += angle_inc_x;
	//angle_y += angle_inc_y;
	//angle_z += angle_inc_z;

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f * 4.f) / ((float)h / 480.f * 3.f);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);



	if (key == 'W') vx += 1.5f * speed;
	if (key == 'S') vx -= 1.5f * speed;
	if (key == 'A') vz -= 1.5f * speed;
	if (key == 'D') vz += 1.5f * speed;

	if (key == 'Z') angle_x -= 1.5f * speed;
	if (key == 'X') angle_y -= 1.5f * speed;
	if (key == 'C') angle_z += 1.5f * speed;

	if (key == 'V') transX -= 1.5f * speed;
	if (key == 'B') transY -= 1.5f * speed;
	if (key == 'N') transZ -= 1.5f * speed;

	if (key == 'G') transX += 1.5f * speed;
	if (key == 'H') transY += 1.5f * speed;
	if (key == 'J') transZ += 1.5f * speed;


	if (key == '0' && action != GLFW_PRESS)
	{
		fire_numParticles += 100;
		//cout << "fire particles: " << fire_numParticles << endl;
	}

	if (key == '9' && action != GLFW_PRESS)
	{
		if (fire_numParticles > 400)
		{
			fire_numParticles -= 100;
			//cout << "fire particles: " << fire_numParticles << endl;
		}
	}

	if (key == 'P' && action != GLFW_PRESS)
	{
		weather_scaler += 0.5f;
		//cout << "weather scaler: " << weather_scaler << endl;
		weather.setScale(weather_scaler);
	}

	if (key == 'O' && action != GLFW_PRESS)
	{
		if (weather_scaler > 1.f)
		{
			weather_scaler -= 0.5f;
			//cout << "weather scaler: " << weather_scaler << endl;
			weather.setScale(weather_scaler);
		}
	}

	if (key == 'I' && action != GLFW_PRESS)
	{
		fire_scaler += 0.5f;
		//cout << "fire scaler: " << fire_scaler << endl;
		particleObject.setScale(fire_scaler);
	}

	if (key == 'U' && action != GLFW_PRESS)
	{
		if (fire_scaler > 0.5f)
		{
			fire_scaler -= 0.5f;
			//cout << "fire scaler: " << fire_scaler << endl;
			particleObject.setScale(fire_scaler);
		}
	}

	if (key == '8' && action != GLFW_PRESS)
	{
		fire_particleSpread += 0.1f;

		//cout << "fire particle spread: " << fire_particleSpread << endl;
		particleObject.setSpread(fire_particleSpread);
	}

	if (key == '7' && action != GLFW_PRESS)
	{
		if (fire_particleSpread > 0.2f)
			fire_particleSpread -= 0.1f;

		//cout << "fire particle spread: " << fire_particleSpread << endl;
		particleObject.setSpread(fire_particleSpread);
	}

	if (key == '6' && action != GLFW_PRESS)
	{
		weather_numParticles += 100;
		//cout << "weather particles: " << weather_numParticles << endl;
	}

	if (key == '5' && action != GLFW_PRESS)
	{
		if (weather_numParticles > 400)
		{
			weather_numParticles -= 100;
			//cout << "weather particles: " << weather_numParticles << endl;
		}
	}

	if (key == '4' && action != GLFW_PRESS)
	{
		weather_particleSpread += 0.1f;

		//cout << "weather particle spread: " << weather_particleSpread << endl;
		particleObject.setSpread(weather_particleSpread);
	}

	if (key == '3' && action != GLFW_PRESS)
	{
		if (weather_particleSpread > 0.2f)
			weather_particleSpread -= 0.1f;

		//cout << "weather particle spread: " << weather_particleSpread << endl;
		particleObject.setSpread(weather_particleSpread);
	}




}

/* Uses the mouse position to control the view
	Maps mouse x and y position to angle */
static void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	int width;
	int height;

	glfwGetWindowSize(window, &width, &height);

	GLfloat mappedX = (xpos - 0) * (2 - (-2)) / (width - 0) - 2;
	lookX = mappedX;

	GLfloat mappedY = (ypos - 0) * (2 - (-2)) / (height - 0) - 2;
	lookY = mappedY;
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper* glw = new GLWrapper(1024, 768, "Assignment 2");

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);
	glw->setCursorCallback(cursorCallback);

	init(glw);


	//Display instructions on terminal
	std::cout << "_____INPUT____" << std::endl;
	std::cout << "Mouse:" << std::endl;
	std::cout << "Use the mouse to control the camera view" << std::endl;
	std::cout << std::endl;
	std::cout << "Keys:" << std::endl;
	std::cout << "W, S, A, D: Change view angle" << std::endl;
	std::cout << "Z, X, C: Change model angle" << std::endl;
	std::cout << "V, B, N, G, H, J: Move objects" << std::endl;
	std::cout << std::endl;
	std::cout << "0, 9: Increase and decrease fire particles" << std::endl;
	std::cout << "8, 7: Increase and decrease fire particle speed" << std::endl;
	std::cout << "6, 5: Increase and decrease weather particles" << std::endl;
	std::cout << "4, 3: Increase and decrease weather particle spread" << std::endl;
	std::cout << "P, O: Increase and decrease weather particle scale" << std::endl;
	std::cout << std::endl;
	std::cout << "______________" << std::endl;

	glw->eventLoop();

	delete(glw);
	return 0;
}


