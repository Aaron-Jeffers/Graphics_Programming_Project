#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "SkyBox.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void linkFogShader();
	void linkToon();
	void linkRimLighting();
	void linkGeo();
	void linkEmapping(Transform t);
	void linkGooch(Transform t);
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);

	//void playAudio(unsigned int Source, glm::vec3 pos);

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Camera myCamera;
	Shader shader;
	Shader fogShader;
	Shader toonShader;
	Shader rimShader;
	Shader geoShader;
	Shader shaderSkybox;
	Shader eMapping;
	Shader goochShader;
	
	Skybox skybox;

	vector<std::string> faces;
	
	//Audio audioDevice;

	float counter;
	float camMoveVel = 0.2f;
	float camRotVel = 0.04f;
	float deg2rad = 3.14159265358979323846 / 180;
	unsigned int whistle;
	unsigned int backGroundMusic;
};

