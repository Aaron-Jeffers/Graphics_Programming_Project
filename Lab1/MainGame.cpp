#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>
#include <windows.h>


Transform geoTransform, eTransform, goochTransform;
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
	
	geometryMesh.loadModel(sphereSmooth);
	environmentMesh.loadModel(sphereSmooth);
	goochMesh.loadModel(sphereSmooth);

	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); 
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag");
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

void MainGame::linkFogShader()
{
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
	geoShader.setFloat("time", counter * 2);
}

void MainGame::linkRimLighting(Transform transform, Mesh mesh)
{
	glm::vec3 camDir;
	camDir = mesh.getSpherePos() - myCamera.getPos();
	camDir = glm::normalize(camDir);
	rimShader.setMat4("u_pm", myCamera.getProjection());
	rimShader.setMat4("u_vm", myCamera.getView());
	rimShader.setMat4("model", transform.GetModel());
	rimShader.setMat4("view", myCamera.getView());
	rimShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkEmapping(Transform transform)
{
	eMapping.setMat4("projection", myCamera.getProjection());
	eMapping.setMat4("view", myCamera.getView());
	eMapping.setMat4("model", transform.GetModel());
	eMapping.setVec3("cameraPos", myCamera.getPos());
}

void MainGame::linkGooch(Transform transform)
{
	goochShader.setMat4("modelToCamera", transform.GetModel());
	goochShader.setMat4("modelToScreen", myCamera.getViewProjection());
	goochShader.setMat3("normalToCamera", glm::mat3((inverse(transform.GetModel()))));
	goochShader.setVec3("vColor", glm::vec3(1.0, 1.0, 1.0));
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour

	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture 
	Texture texture2("..\\res\\water.jpg"); //load texture 

	geoTransform.SetPos(glm::vec3(-10.0, 0.0, 0.0));
	geoTransform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	geoTransform.SetScale(glm::vec3(2, 2, 2));
	
	eTransform.SetPos(glm::vec3(10.0, 0.0, 0.0));
	eTransform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	eTransform.SetScale(glm::vec3(2, 2, 2));

	goochTransform.SetPos(glm::vec3(0.0, 0.0, 0.0));
	goochTransform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	goochTransform.SetScale(glm::vec3(2, 2, 2));

	geoShader.Bind();
	linkGeo();
	geoShader.Update(geoTransform, myCamera);
	geometryMesh.draw();
	geometryMesh.updateSphereData(*geoTransform.GetPos(), 0.62f);

	eMapping.Bind();
	linkEmapping(eTransform);
	eMapping.Update(eTransform, myCamera);
	environmentMesh.draw();
	environmentMesh.updateSphereData(*eTransform.GetPos(), 0.62f);

	goochShader.Bind();
	linkGooch(goochTransform);
	goochShader.Update(goochTransform, myCamera);
	goochMesh.draw();
	goochMesh.updateSphereData(*goochTransform.GetPos(), 0.62f);

	/*shader.Bind();
	shader.Update(transform2, myCamera);
	mesh3.draw();
	mesh3.updateSphereData(*transform2.GetPos(), 0.62f);*/	

	//glActiveTexture(GL_TEXTURE2);
	////glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	//glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, skybox.textureID);

	counter = counter + 0.02f;

	skybox.draw(&myCamera);	
	
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();	
} 