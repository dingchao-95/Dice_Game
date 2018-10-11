
#include "Skybox.h"

float Skybox::skyboxVertices[VERTEX_COUNT][3] = {
	{ TERRAIN_SIZE/2,  TERRAIN_SIZE/2, -TERRAIN_SIZE/2 }, // 0 - TopRightBack
	{-TERRAIN_SIZE/2,  TERRAIN_SIZE/2, -TERRAIN_SIZE/2 }, // 1 - TopLeftBack
	{-TERRAIN_SIZE/2,  TERRAIN_SIZE/2,  TERRAIN_SIZE/2 }, // 2 - TopLeftFront
	{ TERRAIN_SIZE/2,  TERRAIN_SIZE/2,  TERRAIN_SIZE/2 }, // 3 - TopRightFront
	{ TERRAIN_SIZE/2, -TERRAIN_SIZE/2, -TERRAIN_SIZE/2 },	// 4 - BottomRightBack
	{-TERRAIN_SIZE/2, -TERRAIN_SIZE/2, -TERRAIN_SIZE/2 }, // 5 - BottomLeftBack
	{-TERRAIN_SIZE/2, -TERRAIN_SIZE/2,  TERRAIN_SIZE/2 }, // 6 - BottomLeftFront
	{ TERRAIN_SIZE/2, -TERRAIN_SIZE/2,  TERRAIN_SIZE/2 }  // 7 - BottomRightFront 
};

int Skybox::skyboxFaceVertices[FACE_COUNT][VERTICES_PER_FACE] = {
	{ 0,3,2,1 },	// f1 - top
	{ 1,5,4,0 },	// f5 - back 
	{ 2,6,5,1 },	// f6 - left
	{ 0,4,7,3 },	// f4 - right	
	{ 3,7,6,2 },	// f3 - front
	{ 7,4,5,6 }		// f2 - bottom		
};

float Skybox::skyboxFaceVertexTexCoords[FACE_COUNT*VERTICES_PER_FACE][2] = {
	//f1 (top)
	{ 0.50f, 1.00f },	// v0
	{ 0.50f, 0.75f },	// v3
	{ 0.25f, 0.75f },	// v2	
	{ 0.25f, 1.00f },	// v1	
	
	// f5 (back)
	{ 0.50f, 0.75f },	// v1
	{ 0.50f, 0.50f },	// v5	
	{ 0.25f, 0.50f },	// v4	
	{ 0.25f, 0.75f },	// v0
	// f6 (left)
	
	{ 0.75f, 0.75f },	// v2
	{ 0.75f, 0.50f },	// v6	
	{ 0.50f, 0.50f },	// v5	
	{ 0.50f, 0.75f },	// v1
	// f4 (right)
	{ 0.25f, 0.75f },	// v0
	{ 0.25f, 0.50f },	// v4
	{ 0.00f, 0.50f },	// v7
	{ 0.00f, 0.75f },	// v3
	// f3 (front)
	{ 1.00f, 0.75f },	// v3
	{ 1.00f, 0.50f },	// v7
	{ 0.75f, 0.50f },	// v6
	{ 0.75f, 0.75f },	// v2
	// f2 (bottom)
	{ 0.25f, 0.25f },	// v7
	{ 0.25f, 0.50f },	// v4
	{ 0.50f, 0.50f },	// v5
	{ 0.50f, 0.25f }	// v6
};

Skybox::Skybox(const char* filename)
{
	mImageSkyBox = loadBMP(filename);
	mnTextureIdSkyBox = loadTexture(mImageSkyBox);
	if (mImageSkyBox) delete mImageSkyBox;


}

void Skybox::Draw()
{
	// Enable Texturing
	glEnable(GL_TEXTURE_2D);	
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, mnTextureIdSkyBox);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	int vertexIndex;
	for (int face=0; face<6; face++)
	{
		for (int vertex=0; vertex<VERTICES_PER_FACE; vertex++)
		{
				vertexIndex = skyboxFaceVertices[face][vertex];
								glTexCoord2f(skyboxFaceVertexTexCoords[face*VERTICES_PER_FACE+vertex][0], 
								 skyboxFaceVertexTexCoords[face*VERTICES_PER_FACE+vertex][1]);

								glVertex3f(skyboxVertices[vertexIndex][0],
								skyboxVertices[vertexIndex][1],    skyboxVertices[vertexIndex][2]);

		}

	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}