#ifndef TERRAIN_H
#define TERRAIN_H

#include "Globals.h"
#include "GL\glut.h"
#include "glm.hpp"
#include <algorithm>
#include "ImageLoader.h"

class Terrain
{
private:
	// Texture
	Image* mImageTerrain;
	GLuint mnTextureIdTerrain;

	int mnWidth;
	int mnLength;
	float** mArrfHeight; 
	glm::vec3** mArrVecNormals;
	glm::vec3** tempNormals;
	glm::vec3** mArrfColor;
	bool mbComputedNormals; 

public:
	Terrain(const char* filename, float height, const char* texturename);
	Terrain(const char* filename, float height);
	~Terrain();

	void Init();
	void Draw(bool _bDrawNormals = false);
	void DrawNormals();
	void ComputeNormals();
	glm::vec3 GetNormalAt(int _x, int _z);
	int GetLength();
	int GetWidth();
	float GetHeightAt(int _x, int _z);
	void SetHeightAt(int _x, int _z,float _newHeight);
	void computeHeatMap();

};

#endif