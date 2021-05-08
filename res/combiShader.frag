#version 330 

uniform vec2 resolution;
uniform float timeStep;

uniform int octaves;
uniform float amplitude;
uniform vec2 offset;
uniform float axialRotation; //radians
uniform int levelOfDetail;

uniform vec3 baseColour1;
uniform vec3 baseColour2;
uniform float baseColourMixRatio;

//uniform vec3 midColour;
//uniform float midColourMixRatio;

//uniform vec3 endColour;
//uniform float endColourMixRatio;

in float colDifRatio;
float DiffuseCool = 0.3;
float DiffuseWarm = 0.3;
vec3 Cool = vec3(0, 0, 0.6);
vec3 Warm = vec3(0.6, 0, 0);

float randomNumber (in vec2 plot)
{
	return fract(sin(dot(plot.xy, vec2(13.1276,87.279))) * 56326.623);
}

float noise (in vec2 plot)
{
	vec2 floor = floor(plot);
	vec2 fractal = fract(plot);

	float bottomLeft = randomNumber(floor);
	float bottomRight = randomNumber(floor + vec2(1.0,0.0));
	float topLeft = randomNumber(floor + vec2(0.0,1.0));
	float topRight = randomNumber(floor + vec2(1.0,1.0));

	vec2 noise = fractal * fractal * (3.0 - 2.0 * fractal);

	return mix(bottomLeft, bottomRight, noise.x) + (topLeft - bottomLeft) * noise.y * (1.0 - noise.x) + (topRight - bottomRight) * noise.x * noise.y;
}

float fractalBrownianMotion (in vec2 plot)
{
	float fractal;
	float amp = amplitude;
	mat2 rot = mat2(cos(axialRotation), sin(axialRotation), -sin(axialRotation), cos(axialRotation));

	for (int i = 0; i < octaves; i++)
	{
		fractal += amp * noise(plot);
		plot = rot * plot * 2.0 + offset;
		amp *= amplitude;
	}

	return fractal;
}

void main()
{
	
	vec2 coordRatio = gl_FragCoord.xy / resolution.xy * levelOfDetail;
	vec3 colour;

	vec2 g,h;

	h.x = fractalBrownianMotion(coordRatio + vec2(1.0, 2.7) * timeStep);
	h.y = fractalBrownianMotion(coordRatio + vec2(0.3, 4.3) * timeStep);

	g.x = fractalBrownianMotion(coordRatio + h + vec2(9.2, 3.4) * 0.12 * timeStep);
	g.y = fractalBrownianMotion(coordRatio + h + vec2(1.3, 13.4) * 0.17 * timeStep);

	float f = fractalBrownianMotion(coordRatio + g);

	vec3 kcool = min(Cool + DiffuseCool, 1.0);
	vec3 kwarm = min(Warm + DiffuseWarm, 1.0);
	vec3 kfinal = mix(kcool, kwarm, 0.5f);

	colour = mix(baseColour1, baseColour2, clamp((f*f)*4.0, 0, 1));
	colour = mix(colour, kcool, clamp(length(h), 0, 1));
	colour = mix(colour, kwarm, clamp(colDifRatio, 0, 1));

	gl_FragColor = vec4((f*f*f+.6*f*f+.5*f)*colour,1.0);
}