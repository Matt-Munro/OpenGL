#version 420

out vec4 outputColor;
in vec2 ftexcoord;

uniform sampler2D tex1;	

void main()
{
	vec4 texcolour = texture(tex1, ftexcoord);

	outputColor = texcolour;
}