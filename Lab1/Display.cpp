#include "Display.h"
#include "Windows.h"

Display::Display()
{
	sdlWindow = nullptr; //initialise to generate null access violation for debugging. 
	screenWidth = 2240.0f;
	screenHeight = 1260.0f; 

	//sdlWindow = nullptr; //initialise to generate null access violation for debugging. 
	//HWND h = GetDesktopWindow(); //Gets resolution of desktop  //////Note only tested on windows, not mac or linux////////
	//RECT r; //Reference to desktop window points
	//GetClientRect(h, &r);

	screenWidth = r.right - r.left; //Sets window width equal to that of client's desktop width
	screenHeight = r.bottom - r.top; //Sets window width equal to that of client's desktop height
}

Display::~Display()
{
	SDL_GL_DeleteContext(glContext); // delete context
	SDL_DestroyWindow(sdlWindow); // detete window (make sure to delete the context and the window in the opposite order of creation in initDisplay())
	SDL_Quit();
}

float Display::getWidth() { return screenWidth; } //getters
float Display::getHeight() { return screenHeight; }

void Display::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void Display::swapBuffer()
{
	SDL_GL_SwapWindow(sdlWindow); //swap buffers
}

void Display::clearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

void Display::initDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// set up z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	sdlWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); // create window

	if (sdlWindow == nullptr)
	{
		returnError("window failed to create");
	}

	glContext = SDL_GL_CreateContext(sdlWindow);

	if (glContext == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		returnError("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}