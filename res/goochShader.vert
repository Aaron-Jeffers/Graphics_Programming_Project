#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat3 normal;

vec3 LightPosition = vec3(0, 10, 4);

out float NdotL;
out vec3 ReflectVec;
out vec3 ViewVec;

void main()
{
	 vec3 ecPos = vec3(model) * vPosition;
	 vec3 tnorm = normalize(normal * vNormal);
	 vec3 lightVec = normalize(LightPosition - ecPos);
	 ReflectVec = normalize(reflect(-lightVec, tnorm));
	 ViewVec = normalize(-ecPos);
	 NdotL = (dot(lightVec, tnorm) + 1.0) * 0.5;
	 gl_Position = projection * view * model * vec4(vPosition, 1.0f);	 
}