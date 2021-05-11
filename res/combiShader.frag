#version 330 

uniform vec2 resolution; //Resolution of the texture, higher the number effectively 'zooms in on the shaders'
uniform float timeStep;  //Variable to keep track of time

uniform int octaves;   //Octaves, used to determine level of detail of wave in noise function
uniform float amplitude; //Determines the amplitude of the the noise output
uniform vec2 offset;  //Offset of the 'texture', if zoomed in with a higher resolution can give the illusion the texture is changing over time
uniform float axialRotation; //Adds a rotation to the direction in which colour/noise is applied
uniform int lacunarity;  //Lacunarity value of the noise function

uniform float brightness;  //Initial colour value or in this case brightness
uniform float gradientIntensity;  //Applies stauration to one or both colour gradients before they are mixed 
uniform vec3 colourGradient1;  //First colour to be mixed
uniform vec3 colourGradient2;  //Second colour to be mixed

in VS_OUT {
	float colDifRatio;  //Interface to pass in the colDifRatio
} data;

out vec4 FragColor;   //Colour to be applied to the model

float randomNumber (in vec2 plot)   //Method to return a random floating point number between 0 and 1
{
	return fract(sin(dot(plot.xy, vec2(13.1276,87.279))) * 56326.623);
}

float noise (in vec2 plot)  //Method to return the noise value at a particular point 
{
	vec2 floor = floor(plot);  //Returns lowest nearest in to plot
	vec2 fractal = fract(plot);  //Returns fraction part of plot, i.e. 1 - floor

	//Tiling coordinates for random number value
	float bottomLeft = randomNumber(floor);
	float bottomRight = randomNumber(floor + vec2(1.0,0.0));
	float topLeft = randomNumber(floor + vec2(0.0,1.0));
	float topRight = randomNumber(floor + vec2(1.0,1.0));

	vec2 noise = fractal * fractal * (3.1 - 2.0 * fractal);

	return mix(bottomLeft, bottomRight, noise.x) + (topLeft - bottomLeft) * noise.y * (1.0 - noise.x) + (topRight - bottomRight) * noise.x * noise.y;
}

float fractalBrownianMotion (in vec2 plot) //Method to generate the fractal brownian motion
{
	float fBm;  //Value to be returned
	float amp = amplitude;  //Amplitude
	mat2 textureRotation = mat2(cos(axialRotation), sin(axialRotation), -sin(axialRotation), cos(axialRotation)); //Rotation applied to the function on each iteration

	for (int i = 0; i < octaves; i++)
	{
		fBm += amp * noise(plot);   //multiplies the amplitude by the noise value received
		plot = textureRotation * plot * 2.0 + offset;  //Rotates and offsets the plot for the next iteration
		amp *= amplitude;  //Multiplies amplitude by itself, lessening the effect the function has on the next iteration
	}

	return fBm;
}

void main()
{	
	vec2 coordRatio = (gl_FragCoord.xy / resolution.xy) * lacunarity;  //Plot value used for fbm
	vec3 colour = vec3(brightness,brightness,brightness);  //Initial colour value, or in this case acts as a brightness value

	vec2 g,h,i;  //Functions for fbm

	//First function h
	h.x = fractalBrownianMotion(coordRatio + vec2(1.0, 2.7) * timeStep);
	h.y = fractalBrownianMotion(coordRatio + vec2(0.3, 4.3) * timeStep);

	//Second function g which is a function of h
	g.x = fractalBrownianMotion(coordRatio + h + vec2(9.2, 3.4) * 0.42 * timeStep);
	g.y = fractalBrownianMotion(coordRatio + h + vec2(1.3, 13.4) * 0.37 * timeStep);

	//Third function i which is a function of g
	i.x = fractalBrownianMotion(coordRatio + g + vec2(1.2, 8.4) * 0.14 * timeStep);
	i.y = fractalBrownianMotion(coordRatio + g + vec2(12.3, 5.4) * 0.01 * timeStep);

	//Final function f which is a function of i
	float f = fractalBrownianMotion(coordRatio + i);

	//Colour gradients plus initial pre mix intensity
	vec3 grad1 = clamp(colourGradient1 + gradientIntensity, 0.0, 1.0);
	vec3 grad2 = clamp(colourGradient2 + gradientIntensity, 0.0, 1.0);

	colour += mix(grad1, grad2, clamp((f*f)* 4.0 * data.colDifRatio, 0, 1)); //Mixes colour gradients based on a combination of function f and ratio between two coloured gradient fed in from the vert file. Add to preexisting colour or brightness.

	float fFinal = (pow(f,3) * 0.8) + (pow(f,2) * 0.6) + (pow(f,1) * 0.4) + (pow(f,0.5) * 0.2); //Multiplies f by itself multiple times to get colour variation

	FragColor = vec4(fFinal*colour,1.0); //Multiples colour by new variation on f and sets the colour at this point
}