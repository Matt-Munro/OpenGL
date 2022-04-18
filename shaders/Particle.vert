#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; 
layout(location = 2) in vec4 color;
layout(location = 3) in float life;


out vec2 UV;
out vec4 particlecolor;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; 

uniform bool useTextureAtlas;
uniform bool useTexture;
uniform float scale;

void setTextureAtlas();

void main()
{
	float particleSize = xyzs.w;
	particleSize /= scale;


	vec3 particleCenter_worldspace = xyzs.xyz;

	vec3 vertexPosition_worldspace = 
		particleCenter_worldspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);


	UV = squareVertices.xy + vec2(0.5, 0.5);

	particlecolor = color;

	if(useTextureAtlas)
		setTextureAtlas();
	else
		UV = squareVertices.xy + vec2(0.5, 0.5);

}

//Uses life of particle to calculate which section of the texture atlas to use
void setTextureAtlas()
{
	int frameId = int((life - 0) * (63 - 0) / (0.5 - 0) + 0);
	frameId -= 63;
	int rowFrameCount = 8;
	vec2 frameUVsize = vec2(1.0 / rowFrameCount, 0.125);

	vec2 frameOffset = vec2(frameId % rowFrameCount, frameId / rowFrameCount);

	UV += frameOffset;
	UV *= frameUVsize;
}