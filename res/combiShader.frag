#version 330 

uniform vec2 resolution;
uniform float timeStep;

uniform int octaves;
uniform float amplitude;
uniform vec2 offset;
uniform float axialRotation; //radians
uniform int lacunarity;

uniform float brightness;
uniform float gradientIntensity;
uniform vec3 colourGradient1;
uniform vec3 colourGradient2;

in VS_OUT {
	float colDifRatio;
} data;

out vec4 FragColor;

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
	mat2 textureRotation = mat2(cos(axialRotation), sin(axialRotation), -sin(axialRotation), cos(axialRotation));

	for (int i = 0; i < octaves; i++)
	{
		fractal += amp * noise(plot);
		plot = textureRotation * plot * 2.0 + offset;
		amp *= amplitude;
	}

	return fractal;
}

void main()
{	
	vec2 coordRatio = (gl_FragCoord.xy / resolution.xy) * lacunarity;
	vec3 colour = vec3(brightness,brightness,brightness);

	vec2 g,h,i;

	h.x = fractalBrownianMotion(coordRatio + vec2(1.0, 2.7) * timeStep);
	h.y = fractalBrownianMotion(coordRatio + vec2(0.3, 4.3) * timeStep);

	g.x = fractalBrownianMotion(coordRatio + h + vec2(9.2, 3.4) * 0.42 * timeStep);
	g.y = fractalBrownianMotion(coordRatio + h + vec2(1.3, 13.4) * 0.37 * timeStep);

	i.x = fractalBrownianMotion(coordRatio + g + vec2(1.2, 8.4) * 0.14 * timeStep);
	i.y = fractalBrownianMotion(coordRatio + g + vec2(12.3, 5.4) * 0.01 * timeStep);

	float f = fractalBrownianMotion(coordRatio + i);

	vec3 grad1 = min(colourGradient1 + gradientIntensity, 1.0);
	vec3 grad2 = min(colourGradient2 + gradientIntensity, 1.0);

	colour += mix(grad1, grad2, clamp((f*f)* 4.0 * data.colDifRatio, 0, 1)); //before was just colour = mix() and also vec3 colour was set to anything.

	float fFinal = (pow(f,3) * 0.8) + (pow(f,2) * 0.6) + (pow(f,1) * 0.4) + (pow(f,0.5) * 0.2);

	FragColor = vec4(fFinal*colour,1.0);
}