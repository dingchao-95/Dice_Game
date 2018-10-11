#include <iostream>
#include <cstdlib>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Globals.h"
#include "DiceGame.h"
using namespace std;

int main(int argc, char** argv)
{
	// Initialize random
	srand( (unsigned int)time(0) );

	// Create a new Devil Dice Game
	DiceGame::GetInstance().Init();	
	
	// Run the game
	DiceGame::GetInstance().Run();

	// Stop and remove the game
	DiceGame::GetInstance().Shutdown();

	return 0;
}
