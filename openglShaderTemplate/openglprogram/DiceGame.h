#ifndef DICEGAME_H
#define DICEGAME_H

#include "Globals.h"
#include "GL\glut.h"
#include "GridBase.h"
#include "Camera.h"
#include "Cube.h"
#include "Player.h"
#include "freetype.h"
#include <sstream>
#include "Terrain.h"
#include "ParticleEmitter.h"
#include "Skybox.h"

class DiceGame
{
private:
	GridBase* mpGridBase;
	Camera* mpCamera;

	freetype::font_data* mpFontScore;
	freetype::font_data* mpFontWin;

	Cube* mpArrCubes[NUM_CUBES];
	glm::vec3 mArrAvailablePositions[GRID_SIZE*GRID_SIZE];

	Player* mpPlayer1;
	Player* mpPlayer2;

	int mnScreenWidth;
	int mnScreenHeight;
	int mnCubesSpawned;
	int mnCubeSpawnDelay;

	glm::uvec2 mvMousePos;
	bool mbGameOver;
	float mfGameOverTimer;
	Terrain* mpTerrain;
	Skybox* mpSkybox;
	ParticleEmitter* mpParticleEmitter;
	GLuint mnTextureIdSkyBox;

	Image* mImageSkyBox;


	// Singleton Implementation
	DiceGame();
	DiceGame(DiceGame const&);        
    void operator=(DiceGame const&);

public:
	~DiceGame();	

	void Init();
	void Run();	
	void Shutdown();

	void InitCubes(bool _bReset=false);
	void UpdateCubes();
	void DrawCubes();
	void InitPlayers(bool _bReset=false);
	void DrawPlayers();
	void SinkAllCubes();
	void ThrowAwayCubes();
	void DrawText(std::string _strText, int _screenX, int _screenY);
	void DrawResult(std::string _strText, int _screenX, int _screenY);
	void ResetGame();

	static DiceGame& GetInstance();
	static void HandleUpdate(int elapsed);
	static void HandleDraw();
	static void HandleKeypress(unsigned char key, int x, int y);
	static void HandleSpecialKeypress(int key, int x, int y);
	static void HandleResize(int w, int h);
	static void HandleMouseClick(int button, int state, int x, int y);
	static void HandleMouseMotion(int x, int y);

};

#endif