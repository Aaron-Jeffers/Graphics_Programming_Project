#version 330 

uniform vec2 resolution;
uniform float counter;
uniform int octaves;
uniform float amplitude;
uniform vec2 offset;
uniform float axialRotation; //radians
uniform int levelOfDetail;

float randomNumber (in vec2 plot)
{
	return fract(sin(dot(plot.xy, vec2(1.675,92.365))) * 566.623);
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

	float f,g,h;

	h = fractalBrownianMotion(coordRatio + );
	g = fractalBrownianMotion();
	f = fractalBrownianMotion();
}