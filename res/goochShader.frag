#version 330

uniform vec3 vColor;

float DiffuseCool = 0.3;
float DiffuseWarm = 0.3;
vec3 Cool = vec3(0, 0, 0.6);
vec3 Warm = vec3(0.6, 0, 0);

in float NdotL;
in vec3 ReflectVec;
in vec3 ViewVec;

out vec4 result;

void main()
{

	 vec3 kcool = min(Cool + DiffuseCool * vColor, 1.0);
	 vec3 kwarm = min(Warm + DiffuseWarm * vColor, 1.0);
	 vec3 kfinal = mix(kcool, kwarm, NdotL);

	 vec3 nRefl = normalize(ReflectVec);
	 vec3 nview = normalize(ViewVec);

	 float spec = pow(max(dot(nRefl, nview), 0.0), 32.0);

	 if (gl_FrontFacing) 
	 {
		result = vec4(min(kfinal + spec, 1.0), 1.0);
	 } 
	 else 
	 {
		result = vec4(0, 0, 0, 1);
	 }
}