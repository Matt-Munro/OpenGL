#version 330 core

in vec2 UV;
in vec4 particlecolor;

out vec4 color;

uniform sampler2D myTextureSampler;

uniform bool useTexture;

void main()
{

	if(useTexture)
	{
		//Get colour from texture. Discard if colour is black
		color = texture(myTextureSampler, UV);
		if(color.r > 0.9 && color.g > 0.9 && color.b > 0.9)
		{
			discard;
		}
	}
	else
	{
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
}