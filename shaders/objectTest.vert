#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 colour;
layout(location = 3) in vec3 normal;

out vec2 UV;
out vec4 fcolour;

uniform mat4 model, view, projection;
uniform uint shininess;
uniform vec3 ulightDirection;

void main()
{
	vec4 diffuseColour = colour;

	vec4 ambient = diffuseColour * 0.3;

	//Diffuse
	mat4 modelView = model;
	vec4 homPosition = vec4(position, 1.0);
	vec3 normalMatrix = mat3(modelView) * normal;
	vec3 normalizedNormal = normalize(normalMatrix);
	vec3 lightDirection = normalize(ulightDirection);
	vec4 diffuse = max(dot(normalizedNormal, lightDirection), 0) * diffuseColour;

	vec4 position_h = vec4(position, 1.0);

	//Specular
	vec4 specular_colour = vec4(1.0, 1.0, 0.6, 1.0);
	vec4 specularPosition = modelView * position_h;
	vec3 V = normalize(-specularPosition.xyz);
	vec3 R = reflect(-lightDirection, normalizedNormal);
	vec4 specular = pow(max(dot(R, V), 0), shininess) * specular_colour;


	//Attenuation
	float attenuation;
	vec3 L = lightDirection - specularPosition.xyz;
	float distanceToLight = length(L);
	attenuation = 1.0 / (1.0 + 0.6 * pow(distanceToLight, 2));

	fcolour = ambient + attenuation * (diffuse + specular);

	gl_Position =  projection * view * model * position_h;
	
	UV = vertexUV;
}