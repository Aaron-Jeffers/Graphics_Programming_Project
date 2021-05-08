#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}