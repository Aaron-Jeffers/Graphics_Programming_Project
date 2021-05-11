#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 model;  //Transform of the model
uniform mat4 viewProjection; //Camera view projection
uniform mat3 normal; //Inverse of the transform of the model, i.e. the normal
uniform vec3 gradientAxis;  //Vector relative to model along which the colour gradient occurs

out VS_OUT {
	float colDifRatio;  //Interface to pass out the colDifRatio
} data;

void main()
{
	vec3 tnorm = normalize(normal * VertexNormal);  //Normalised vector between normal vector from the model to camera(perpendicular to tangent) times the normal of that vertex
	data.colDifRatio = (dot(gradientAxis, tnorm) + 1.0) * 0.5; //Dot product of the gradientAxis and tnorm, float value to determine strength of colour 1 vs 2 along the gradient

	gl_Position = viewProjection * model * vec4(VertexPosition, 1.0f);  //Sets the position of current vertex(gets the position of model relative to camera, multiplies it by the transform of the model, then by the vertex position relative to the model)
}