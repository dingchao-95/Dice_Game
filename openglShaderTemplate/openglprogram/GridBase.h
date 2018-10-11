#ifndef GRIDBASE_H
#define GRIDBASE_H

#include "GL\glut.h"

class GridBase
{
private:
	int mnGridSize;
	float mfCubeSize;

public:
	GridBase(int _nGridSize, float _fCubeSize);
	~GridBase();

	void Draw();

};

#endif