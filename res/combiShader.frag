#version 330 

uniform vec2 resolution;
uniform float timeStep;

uniform int octaves;
uniform float amplitude;
uniform vec2 offset;
uniform float axialRotation; //radians
uniform int levelOfDetail;

//uniform vec3 baseColour1;
//uniform vec3 baseColour2;
uniform float gradientIntensity;
uniform vec3 colourGradient1;
uniform vec3 colourGradient2;
//uniform vec3 colourGradient3;
//in float colDifRatio;

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
	vec3 colour = vec3(0.3,0.3,0.3);

	vec2 g,h;

	h.x = fractalBrownianMotion(coordRatio + vec2(1.0, 2.7) * timeStep);
	h.y = fractalBrownianMotion(coordRatio + vec2(0.3, 4.3) * timeStep);

	g.x = fractalBrownianMotion(coordRatio + h + vec2(9.2, 3.4) * 0.12 * timeStep);
	g.y = fractalBrownianMotion(coordRatio + h + vec2(1.3, 13.4) * 0.17 * timeStep);

	float f = fractalBrownianMotion(coordRatio + g);

	vec3 grad1 = min(colourGradient1 + gradientIntensity, 1.0);
	vec3 grad2 = min(colourGradient2 + gradientIntensity, 1.0);
	//vec3 grad3 = min(colourGradient3 + gradientIntensity, 1.0);

	//colour = mix(baseColour1, baseColour2, clamp((f*f)*4.0, 0, 1));
	//colour = mix(colour, grad1, clamp(colDifRatio *(f*f)*4.0, 0, 1));
	//colour = mix(colour, grad2, clamp(colDifRatio *(f*f)*4.0, 0, 1));

	colour += mix(grad1, grad2, clamp((f*f)*4.0 * data.colDifRatio, 0, 1)); //before was just colour = mix() and also vec3 colour was set to anything.
	//colour += mix(grad2, grad3, clamp((f*f)*4.0 * data.colDifRatio, 0, 1));

	//colour = mix(colour,mix(grad1, grad2, clamp((f*f)*4.0 * colDifRatio, 0, 1)),1.5f);


	FragColor = vec4((f*f*f+.6*f*f+.5*f)*colour,1.0);
}