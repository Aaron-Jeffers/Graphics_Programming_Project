#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>
#include <windows.h>


Transform transform, transform1, transform2;
bool KEYS[322];

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Shader shader();
	Shader fogShader();
	Shader toonShader();
	Shader rimShader();
	Shader geoShader();
	Shader eMapping();
	//Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\sphereSmooth.obj");
	mesh2.loadModel("..\\res\\torus2.obj");
	mesh3.loadModel("..\\res\\monkey3.obj");
	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	eMapping.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	goochShader.init("..\\res\\goochShader.vert", "..\\res\\goochShader.frag");

	geoShader.initGeo();

	myCamera.initCamera(glm::vec3(0, 0, -10), 90.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	skybox.init(faces);

	//Initialise inputs
	for (int i = 0; i < 322; i++)
	{
		KEYS[i] = false;
	}
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) //get and process events
	{
		switch (event.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				KEYS[event.key.keysym.sym] = true;
				break;

			case SDL_KEYUP:
				KEYS[event.key.keysym.sym] = false;
				break;		
		}
	}

	//Movement control inputs
	if(KEYS[SDLK_d])
	{
		myCamera.MoveRight(camMoveVel);
	}
	if (KEYS[SDLK_a])
	{
		myCamera.MoveRight(-camMoveVel);
	}	
	if (KEYS[SDLK_w])
	{
		myCamera.MoveForward(camMoveVel);
	}
	if (KEYS[SDLK_s])
	{
		myCamera.MoveForward(-camMoveVel);
	}

	//Rotation control inputs
	if (KEYS[SDLK_l])
	{
		myCamera.RotateY(-camRotVel);
	}
	if (KEYS[SDLK_j])
	{
		myCamera.RotateY(camRotVel);
	}

	//Pitch control inputs
	if (KEYS[SDLK_i])
	{
		myCamera.Pitch(camRotVel);
	}
	if (KEYS[SDLK_k])
	{
		myCamera.Pitch(-camRotVel);
	}
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

//void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
//{
//	
//	ALint state; 
//	alGetSourcei(Source, AL_SOURCE_STATE, &state);
//	/*
//	Possible values of state
//	AL_INITIAL
//	AL_STOPPED
//	AL_PLAYING
//	AL_PAUSED
//	*/
//	if (AL_PLAYING != state)
//	{
//		audioDevice.playSound(Source, pos);
//	}
//}

void MainGame::linkFogShader()
{
	//fogShader.setMat4("u_pm", myCamera.getProjection());
	//fogShader.setMat4("u_vm", myCamera.getProjection());
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToon()
{
	toonShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkGeo()
{
	//float randX = ((float)rand() / (RAND_MAX));
	//float randY = ((float)rand() / (RAND_MAX));
	//float randZ = ((float)rand() / (RAND_MAX));
	//// Frag: uniform float randColourX; uniform float randColourY; uniform float randColourZ;
	//geoShader.setFloat("randColourX", randX);
	//geoShader.setFloat("randColourY", randY);
	//geoShader.setFloat("randColourZ", randZ);
	//// Geom: uniform float time;
	geoShader.setFloat("time", counter * 2);
}

void MainGame::linkRimLighting()
{
	glm::vec3 camDir;
	camDir = mesh2.getSpherePos() - myCamera.getPos();
	camDir = glm::normalize(camDir);
	rimShader.setMat4("u_pm", myCamera.getProjection());
	rimShader.setMat4("u_vm", myCamera.getView());
	rimShader.setMat4("model", transform.GetModel());
	rimShader.setMat4("view", myCamera.getView());
	rimShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkEmapping(Transform t)
{
	eMapping.setMat4("projection", myCamera.getProjection());
	eMapping.setMat4("view", myCamera.getView());
	eMapping.setMat4("model", t.GetModel());
	eMapping.setVec3("cameraPos", myCamera.getPos());
}

void MainGame::linkGooch(Transform t)
{
	goochShader.setMat4("modelToCamera", t.GetModel());
	goochShader.setMat4("modelToScreen", myCamera.getViewProjection());
	goochShader.setMat3("normalToCamera", glm::mat3((inverse(t.GetModel()))));
	goochShader.setVec3("vColor", glm::vec3(1.0, 1.0, 1.0));

	//goochShader.setVec3("camDir", myCamera.getPos());
	//goochShader.setVec3("LightPosition", myCamera.getPos());
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour
	
	//linkFogShader();
	//linkToon();
	//linkRimLighting();
	//linkEmapping();

	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture 
	//texture1.Bind(0);

	transform.SetPos(glm::vec3(-5, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(2, 2, 2));

	transform1.SetPos(glm::vec3(5, 0.0, 0.0));
	transform1.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform1.SetScale(glm::vec3(2, 2, 2));

	transform2.SetPos(glm::vec3(0.0, 2.5, 0.0));
	transform2.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform2.SetScale(glm::vec3(0.15, 0.15, 0.15));  //Figure out scale issue

	/*eMapping.Bind();
	linkEmapping(transform);
	eMapping.Update(transform, myCamera);
	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.62f);*/

	goochShader.Bind();
	linkGooch(transform1);
	goochShader.Update(transform1, myCamera);
	mesh2.draw();
	
		
	/*geoShader.Bind();
	linkGeo();
	geoShader.Update(transform1, myCamera);
	mesh2.draw();
	mesh2.updateSphereData(*transform1.GetPos(), 0.62f);*/

	/*shader.Bind();
	shader.Update(transform2, myCamera);
	mesh3.draw();
	mesh3.updateSphereData(*transform2.GetPos(), 0.62f);*/

	//linkEmapping();
	//eMapping.Bind();	

	//glActiveTexture(GL_TEXTURE2);
	////glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	//glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, skybox.textureID);

	counter = counter + 0.02f;

	skybox.draw(&myCamera);	
	
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();	
} 