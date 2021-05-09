#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat3 normal;
uniform vec3 gradientAxis;

//out float colDifRatio;

out VS_OUT {
	float colDifRatio;
} data;

void main()
{
	vec3 tnorm = normalize(normal * VertexNormal);
	data.colDifRatio = (dot(gradientAxis, tnorm) + 1.0) * 0.5;

	gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
}