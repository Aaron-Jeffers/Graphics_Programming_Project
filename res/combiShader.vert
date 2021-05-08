#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
}