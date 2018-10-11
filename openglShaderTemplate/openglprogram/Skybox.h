#ifndef SKYBOX_H
#define SKYBOX_H

#include "Globals.h"
#include "GL\glut.h"
#include "glm.hpp"
#include "ImageLoader.h"

class Skybox
{
private:
	// Class Constants
	static const int VERTEX_COUNT		= 8;		// # vertices
	static const int FACE_COUNT			= 6;		// # faces
	static const int VERTICES_PER_FACE	= 4;		// # vertices per face
	static const int FACES_PER_VERTEX	= 3;		// # faces adjoining any vertex

	// Clas Variables
	static float skyboxVertices[VERTEX_COUNT][3];
	static int   skyboxFaceVertices[FACE_COUNT][VERTICES_PER_FACE];
	static float skyboxFaceVertexTexCoords[FACE_COUNT*VERTICES_PER_FACE][2];

	// Textures
	Image* mImageSkyBox;
	GLuint mnTextureIdSkyBox;


public:
	Skybox();
	Skybox(const char* filename);
	void Draw();
};

#endif