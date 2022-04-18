#version 420

layout(location = 0) in vec3 position;
layout(location = 3) in vec2 texcoord;

uniform mat4 view, projection;

out vec4 fcolour;
out vec2 ftexcoord;

void main()
{
	vec4 pos = projection * view * vec4(position, 1.0);
	gl_Position = pos.xyww;

	ftexcoord = texcoord;
}

