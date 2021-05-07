#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 2) in vec3 vNormal;

uniform mat4 modelToCamera;
uniform mat4 modelToScreen;
uniform mat4 view;
uniform mat3 normalToCamera;

vec3 LightPosition = vec3(0, 10, 4);

out float NdotL;
out vec3 ReflectVec;
out vec3 ViewVec;

void main()
{
	 vec3 ecPos = vec3(modelToCamera) * vPosition;
	 vec3 tnorm = normalize(normalToCamera * vNormal);
	 vec3 lightVec = normalize(LightPosition - ecPos);
	 ReflectVec = normalize(reflect(-lightVec, tnorm));
	 ViewVec = normalize(-ecPos);
	 NdotL = (dot(lightVec, tnorm) + 1.0) * 0.5;
	 gl_Position = modelToScreen * view * modelToCamera * vec4(vPosition, 1.0f);	 
}