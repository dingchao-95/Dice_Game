#include "DiceGame.h"

DiceGame::DiceGame()
{	
	// Initialise GLUT
	int argc = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(mnScreenWidth = SCREEN_WIDTH, mnScreenHeight = SCREEN_HEIGHT);

	// Create window
	glutCreateWindow("Dice Game");

	// Set graphics settings
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	// Set handler functions
	glutKeyboardFunc(HandleKeypress);
	glutSpecialFunc(HandleSpecialKeypress);
	glutReshapeFunc(HandleResize);
	glutDisplayFunc(HandleDraw);
    glutTimerFunc(25, HandleUpdate, 0);
    glutMouseFunc(HandleMouseClick);
    glutMotionFunc(HandleMouseMotion);

	// Set graphics settings
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	// Set light settings
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	

}

DiceGame::~DiceGame()
{
	if (mpGridBase) delete mpGridBase;
	if (mpCamera) delete mpCamera;
	ThrowAwayCubes();
	if (mpPlayer1) delete mpPlayer1;
	if (mpPlayer2) delete mpPlayer2;
	if (mpFontScore) 
	{
		mpFontScore->clean();
		delete mpFontScore;
	}
	if (mpFontWin) 
	{
		mpFontWin->clean();
		delete mpFontWin;
	}
}

void DiceGame::Init()
{
	mpGridBase = new GridBase(GRID_SIZE, CUBE_SIZE);
	mpCamera = new Camera();
	mpFontScore = new freetype::font_data();
	mpFontScore->init("Doctor Soos Bold.ttf", 64);
	mpFontWin = new freetype::font_data();
	mpFontWin->init("Doctor Soos Bold.ttf", 133);
	mvMousePos.x = mvMousePos.y = 0;
	mbGameOver = false;
	mfGameOverTimer = 0.0f;

	mpTerrain  = new Terrain("scalloped_with_square_centre_256.bmp", 90.0f );

	mpSkybox = new Skybox("in_the_night.bmp");

	mpParticleEmitter = new ParticleEmitter(glm::vec3(-150.0f, 85.0f, -230.0f),
			   				    glm::vec3(0.0f, 1.0f, 0.0f),
							    glm::vec3(0.4f, 0.0f, 0.0f),
							    glm::vec3(0.0f, 0.0f, 0.0f),
							    130.0f,
							    65.0f,
							    false,
							    "circle.bmp", 
							    "star_alpha.bmp",
							    mpCamera);


	InitCubes();
	InitPlayers();

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	DiceGame::GetInstance().mpCamera->Focus(60.0f, (float)mnScreenWidth / mnScreenHeight, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void DiceGame::InitCubes(bool _bReset)
{
	int nNumCubes = 0;
	int x = 0, z = 0;

	// Generate the full list of positions
	int offset = GRID_SIZE>>1;
	for (int x=0; x<GRID_SIZE; x++)
	{
		for (int z=0; z<GRID_SIZE; z++)
		{
			mArrAvailablePositions[x*GRID_SIZE+z].x = (float)x-offset;
			mArrAvailablePositions[x*GRID_SIZE+z].z = (float)z-offset;
		}
	}

	// Now randomize the positions
	int swapIdx;
	glm::vec3 temp;
	for (int idx=0; idx<GRID_SIZE*GRID_SIZE; idx++)
	{
		swapIdx = rand()%(GRID_SIZE*GRID_SIZE);
		temp = mArrAvailablePositions[swapIdx];
		mArrAvailablePositions[swapIdx] = mArrAvailablePositions[idx];
		mArrAvailablePositions[idx] = temp;
	}
			
	// Create all the cubes but do not init first
	for (int i=0; i<NUM_CUBES;i++)
	{
		if (!_bReset) mpArrCubes[i] = new Cube();	
		mpArrCubes[i]->Init(mArrAvailablePositions[i]);
	}
	mnCubesSpawned = 0;
	mnCubeSpawnDelay = 0;
}

void DiceGame::InitPlayers(bool _bReset)
{
	if (!_bReset) mpPlayer1 = new Player();
	mpArrCubes[0]->Spawn();
	mpPlayer1->Init(1, mpArrCubes[0]->GetPosition(), glm::uvec4(1.0f, 0.3f, 0.3f, 1.0f), mpArrCubes[0]); 	

	if (!_bReset) mpPlayer2 = new Player();
	mpArrCubes[1]->Spawn();
	mpPlayer2->Init(2, mpArrCubes[1]->GetPosition(), glm::uvec4(0.3f, 0.3f, 1.0f, 1.0f), mpArrCubes[1]); 

	mnCubesSpawned = 2;
}

void DiceGame::Run()
{
	glutMainLoop();
}

void DiceGame::HandleUpdate(int elapsed)
{
	

	// If not all the cubes have been initialized, init one by one, and advance count
	if (DiceGame::GetInstance().mnCubesSpawned<NUM_CUBES)
	{
		DiceGame::GetInstance().mnCubeSpawnDelay++;
		if (DiceGame::GetInstance().mnCubeSpawnDelay==CUBE_SPAWN_DELAY)
		{
			DiceGame::GetInstance().mpArrCubes[DiceGame::GetInstance().mnCubesSpawned]->Spawn();
			DiceGame::GetInstance().mnCubesSpawned++;
			DiceGame::GetInstance().mnCubeSpawnDelay = 0;
		}

	}


	DiceGame::GetInstance().UpdateCubes();

	// Check for player movements and update
	if (DiceGame::GetInstance().mpPlayer1->Update(DiceGame::GetInstance().mpPlayer2)) glutPostRedisplay();
	if (DiceGame::GetInstance().mpPlayer2->Update(DiceGame::GetInstance().mpPlayer1)) glutPostRedisplay();

	if (!DiceGame::GetInstance().mbGameOver)
	{
		// Check for players scoring
		int p1Score = 0;
		int p2Score = 0;
		if (p1Score=DiceGame::GetInstance().mpPlayer1->GetScore())
		{
			DiceGame::GetInstance().mpPlayer2->UpdateLife(-p1Score);
		}
		if (p2Score=DiceGame::GetInstance().mpPlayer2->GetScore())
		{
			DiceGame::GetInstance().mpPlayer1->UpdateLife(-p2Score);
		}

		// Game Over processing
		if (DiceGame::GetInstance().mpPlayer1->GetLife()<=0 || DiceGame::GetInstance().mpPlayer2->GetLife()<=0)
		{
			DiceGame::GetInstance().SinkAllCubes();
			DiceGame::GetInstance().mfGameOverTimer = 12.5f;
			DiceGame::GetInstance().mbGameOver = true;
		}
	}
	else
	{
		// Update Timer
		if (DiceGame::GetInstance().mfGameOverTimer>0.0f)
			DiceGame::GetInstance().mfGameOverTimer -= 0.025f;

		else
		{
			DiceGame::GetInstance().ResetGame();
		}		
	}
	
	DiceGame::GetInstance().mpParticleEmitter->Update(0.0025f);

	glutTimerFunc(25, HandleUpdate, 0);
	glutPostRedisplay();
}

void DiceGame::UpdateCubes()
{
	bool bRedrawNeeded = false;
	for (int i=0; i<NUM_CUBES;i++)
	{
		if (mpArrCubes[i]->Update()) bRedrawNeeded = true;
	}
	if (bRedrawNeeded) glutPostRedisplay();
}

void DiceGame::DrawText(std::string _strText, int _screenX, int _screenY)
{
	glPushMatrix();
		glLoadIdentity();
		freetype::print(*DiceGame::GetInstance().mpFontScore, (float)_screenX, (float)_screenY, _strText.c_str());
	glPopMatrix();
}

void DiceGame::DrawResult(std::string _strText, int _screenX, int _screenY)
{
	glPushMatrix();
		glLoadIdentity();
		freetype::print(*DiceGame::GetInstance().mpFontWin, (float)_screenX, (float)_screenY, _strText.c_str());
	glPopMatrix();
}

void DiceGame::HandleDraw()
{
	// Clear window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset matrix 
	glLoadIdentity();

	////Ambient light
	//GLfloat ambientColor[] = { 0.3f, 0.3f , 0.3f, 1.0f }; 
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	///*lighting with shader is: color_out = max(dot(normal,light_direction),0.0)*diffuse_color;*/

	//// Add positioned light (Red player)
	//GLfloat lightColor0[] = { ( (200.0f - DiceGame::GetInstance().mpPlayer2->GetLife() ) / 200.0f ) , 0.0f, 0.0f, 1.0f}; // Reddish Color
	//GLfloat lightPos0[] = {DiceGame::GetInstance().mpPlayer1->GetPosition().x,
	//					   DiceGame::GetInstance().mpPlayer1->GetPosition().y,
	//					   DiceGame::GetInstance().mpPlayer1->GetPosition().z,
	//					   1.0f}; // Positioned at Red Player.
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	//

	//// Add positioned light (Blue player)
	//GLfloat lightColor1[] = {0.0f, 0.0f, ( (200.0f - DiceGame::GetInstance().mpPlayer1->GetLife() )  / 200.0f ), 1.0f}; // Bluish Color
	//GLfloat lightPos1[] = {DiceGame::GetInstance().mpPlayer2->GetPosition().x, 
	//					   DiceGame::GetInstance().mpPlayer2->GetPosition().y, 
	//					   DiceGame::GetInstance().mpPlayer2->GetPosition().z, 
	//					   1.0f}; // Positioned at Blue Player.
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	//glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	// Directed light
    GLfloat lightColor2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPos2[] = { 0.0f, 1.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
	
	//Add fog
	GLfloat fogColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 260.0f);
	glFogf(GL_FOG_END, 380.0f);
	
	// Camera
	DiceGame::GetInstance().mpCamera->Set();

	//Skybox
	DiceGame::GetInstance().mpSkybox->Draw();

	glEnable(GL_FOG);
	
	DiceGame::GetInstance().DrawCubes();

	DiceGame::GetInstance().DrawPlayers();

	DiceGame::GetInstance().mpTerrain->Draw(false);

	glDisable(GL_FOG);



	DiceGame::GetInstance().mpParticleEmitter->Draw();

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	std::string strP1Score = static_cast<std::ostringstream*>( &(std::ostringstream() << DiceGame::GetInstance().mpPlayer1->GetLife()) )->str();;
	DiceGame::GetInstance().DrawText(strP1Score, (int)(DiceGame::GetInstance().mnScreenWidth>>1) - 170, DiceGame::GetInstance().mnScreenHeight - 80);
	
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	std::string strP2Score = static_cast<std::ostringstream*>( &(std::ostringstream() << DiceGame::GetInstance().mpPlayer2->GetLife()) )->str();;
	DiceGame::GetInstance().DrawText(strP2Score, (int)(DiceGame::GetInstance().mnScreenWidth>>1), DiceGame::GetInstance().mnScreenHeight - 80);


	if (DiceGame::GetInstance().mbGameOver)
	{
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

		if (DiceGame::GetInstance().mpPlayer1->GetLife()>DiceGame::GetInstance().mpPlayer2->GetLife())
		{
			DiceGame::GetInstance().DrawResult("Player 1 Wins!", (int)(DiceGame::GetInstance().mnScreenWidth>>1) - 500, (DiceGame::GetInstance().mnScreenHeight>>1) - 100);
		}
		else if (DiceGame::GetInstance().mpPlayer1->GetLife()<DiceGame::GetInstance().mpPlayer2->GetLife())
		{
			DiceGame::GetInstance().DrawResult("Player 2 Wins!", (int)(DiceGame::GetInstance().mnScreenWidth>>1) - 500, (DiceGame::GetInstance().mnScreenHeight>>1) - 100);
		}
		else
		{
			DiceGame::GetInstance().DrawText("You both suck!!", (int)(DiceGame::GetInstance().mnScreenWidth>>1) - 500, (DiceGame::GetInstance().mnScreenHeight>>1) - 100);
		}
			
	}


	// flush GL buffers 
	glFlush(); 

	// Double buffering
	glutSwapBuffers();
}

void DiceGame::Shutdown()
{

}

void DiceGame::DrawCubes()
{
	for (int i=0; i<NUM_CUBES;i++)
	{
		mpArrCubes[i]->Draw();
	}
}

void DiceGame::DrawPlayers()
{
	mpPlayer1->Draw();
	mpPlayer2->Draw();
}

void DiceGame::ThrowAwayCubes()
{

}

void DiceGame::SinkAllCubes()
{
	for (int i=0; i<NUM_CUBES;i++)
	{
		mpArrCubes[i]->StartSinking();
	}
}

void DiceGame::ResetGame()
{
	mbGameOver = false;
	mfGameOverTimer = 0.0f;

	mpArrCubes[0]->WipeGrid();
	InitCubes(true);
	InitPlayers(true);
}

DiceGame& DiceGame::GetInstance()
{
	static DiceGame mInstance;
	return mInstance;
}

void DiceGame::HandleKeypress(unsigned char key, int x, int y)
{
	if (!DiceGame::GetInstance().mbGameOver)
	{
		switch (key)
		{
		case 'w': 
		case 'W':
			DiceGame::GetInstance().mpPlayer1->Move(glm::vec3(0.0f, 0.0f, -1.0f), DiceGame::GetInstance().mpPlayer2); break;
		case 'a': 
		case 'A':
			DiceGame::GetInstance().mpPlayer1->Move(glm::vec3(-1.0f, 0.0f, 0.0f), DiceGame::GetInstance().mpPlayer2); break;
		case 's': 
		case 'S':
			DiceGame::GetInstance().mpPlayer1->Move(glm::vec3(0.0f, 0.0f, 1.0f), DiceGame::GetInstance().mpPlayer2); break;
		case 'd': 
		case 'D':
			DiceGame::GetInstance().mpPlayer1->Move(glm::vec3(1.0f, 0.0f, 0.0f), DiceGame::GetInstance().mpPlayer2); break;
		}
	}
}

void DiceGame::HandleSpecialKeypress(int key, int x, int y)
{
	if (!DiceGame::GetInstance().mbGameOver)
	{
		switch (key)
		{
		case GLUT_KEY_UP: DiceGame::GetInstance().mpPlayer2->Move(glm::vec3(0.0f, 0.0f, -1.0f), DiceGame::GetInstance().mpPlayer1); break;
		case GLUT_KEY_LEFT: DiceGame::GetInstance().mpPlayer2->Move(glm::vec3(-1.0f, 0.0f, 0.0f), DiceGame::GetInstance().mpPlayer1); break;
		case GLUT_KEY_DOWN: DiceGame::GetInstance().mpPlayer2->Move(glm::vec3(0.0f, 0.0f, 1.0f), DiceGame::GetInstance().mpPlayer1); break;
		case GLUT_KEY_RIGHT: DiceGame::GetInstance().mpPlayer2->Move(glm::vec3(1.0f, 0.0f, 0.0f), DiceGame::GetInstance().mpPlayer1); break;
		}
	}
}

void DiceGame::HandleResize(int w, int h)
{
	// Store and set width and height
	glViewport(0, 0, DiceGame::GetInstance().mnScreenWidth = w, DiceGame::GetInstance().mnScreenHeight = h);                                                                                                             

	// Set up projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    DiceGame::GetInstance().mpCamera->Focus(60.0f, (float)w / h, 1.0f, 1000.0f);

	// Set up model view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
}

void DiceGame::HandleMouseClick(int button, int state, int x, int y)
{
	DiceGame::GetInstance().mvMousePos.x = x;
	DiceGame::GetInstance().mvMousePos.y = y;
}

void DiceGame::HandleMouseMotion(int x, int y)
{
	int deltaX = x - DiceGame::GetInstance().mvMousePos.x;
	int deltaY = y - DiceGame::GetInstance().mvMousePos.y;

	DiceGame::GetInstance().mvMousePos.x = x;
	DiceGame::GetInstance().mvMousePos.y = y;

	// Update the Camera's rotation
	DiceGame::GetInstance().mpCamera->Rotate((float)-deltaX, (float)deltaY);

	// Ask GLUT to redraw the screen now
	glutPostRedisplay();
}