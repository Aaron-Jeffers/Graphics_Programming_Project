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
	void linkRimLighting(Transform transform, Mesh mesh);
	void linkGeo();
	void linkEmapping(Transform transform);
	void linkCombi(Transform transform);

	Display _gameDisplay;
	GameState _gameState;
	Transform geoTransform, eTransform, combiTransform;
	Mesh geometryMesh, environmentMesh, combiMesh;
	Shader shader, fogShader, toonShader, rimShader, geoShader, shaderSkybox, eMapping, combiShader;
	Skybox skybox;
	Camera myCamera;
	vector<std::string> faces;

	//Models
	string cube = "..\\res\\cube.obj";
	string sphere = "..\\res\\sphere.obj";
	string sphereSmooth = "..\\res\\sphereSmooth.obj";
	string sphereSuperSmooth = "..\\res\\sphereSuperSmooth.obj";
	string torus = "..\\res\\torus.obj";
	string torusSmooth = "..\\res\\torusSmooth.obj";
	string torusSuperSmooth = "..\\res\\torusSuperSmooth.obj";

	//Textures
	string bricks = "..\\res\\bricks.jpg";
	string water = "..\\res\\water.jpg";
	string badCompanySmileyITriedToMakeInPaint = "..\\res\\BadCompanySmiley.jpg";

	bool KEYS[322];
	float counter;
	float camMoveVel = 0.2f;
	float camRotVel = 0.04f;
	float deg2rad = 3.14159265358979323846 / 180;
};

