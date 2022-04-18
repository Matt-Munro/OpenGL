#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 fcolour;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
//uniform sampler2D myTextureSampler;

void main(){

	// Output color = color of the texture at the specified UV
	//color = texture( myTextureSampler, UV ).rgb;
	//color = vec3(1.0f, 1.0f, 1.0f);

	color = fcolour.rgb;
}