#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 model;  //Transform of the model
uniform mat4 viewProjection; //Camera view projection
uniform mat3 normal; //Inverse of the transform of the model, i.e. the normal
uniform vec3 gradientAxis;  //Vector relative to model along which the colour gradient occurs
uniform float timeStep;  //Variable to keep track of time

out VS_OUT {
	float colDifRatio;  //Interface to pass out the colDifRatio
	float timeStep;
} data;


float turbulence( vec3 p ) {

  float w = 100.0;
  float t = -.5;

  for (float f = 1.0 ; f <= 10.0 ; f++ ){
    float power = pow( 2.0, f );
    t += abs( pnoise( vec3( power * p ), vec3( 10.0, 10.0, 10.0 ) ) / power );
  }

  return t;

}

void main()
{
	float colourBias = 0.5f;
	vec3 tempNorm = normalize(normal * VertexNormal);  //Normalised vector between normal vector from the model to camera(perpendicular to tangent) times the normal of that vertex
	data.colDifRatio = (dot(gradientAxis, tempNorm) + colourBias) * colourBias; //Dot product of the gradientAxis and tnorm, float value to determine strength of colour 1 vs 2 along the gradient


	noise = 10.0 *  -.10 * turbulence( .5 * normal + time );
  float b = 5.0 * pnoise( 0.05 * position + vec3( 2.0 * time ), vec3( 100.0 ) );
  float displacement = - noise + b;

  vec3 newPosition = VertexPosition + normal * displacement;

	gl_Position = viewProjection * model * vec4(newPosition, 1.0f);  //Sets the position of current vertex(gets the position of model relative to camera, multiplies it by the transform of the model, then by the vertex position relative to the model)
}