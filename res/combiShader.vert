#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat3 normal;

vec3 lightPos = vec3(10, 0, 10);

out float colDifRatio;

void main()
{
	vec3 ecPos = vec3(model) * VertexPosition;
	vec3 tnorm = normalize(normal * VertexNormal);
	vec3 lightVec = normalize(lightPos - ecPos);
	colDifRatio = (dot(lightVec, tnorm) + 1.0) * 0.5;

	gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
}