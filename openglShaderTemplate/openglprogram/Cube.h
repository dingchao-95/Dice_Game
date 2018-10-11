#ifndef CUBE_H
#define CUBE_H

#include "Globals.h"
#include "GL\glut.h"
#include "glm.hpp"
#include "ImageLoader.h"

struct TransformListNode
{
	glm::vec4 mvTransform;
	TransformListNode* mpNext;
};

class Cube
{
private:
	enum CubeState {
		eCubeStateINVALID,
		eCubeStateSpawning,
		eCubeStateIdle,
		eCubeStateRolling,
		eCubeStateFlashing,
		eCubeStateSinking,
		eCubeStateInactive,
		eCubeStateMAX
	};

	enum CubeSide {
		eCubeSideINVALID = -1,
		eCubeSideTop = 0,
		eCubeSideLeft,
		eCubeSideBottom,
		eCubeSideRight,
		eCubeSideFront,		
		eCubeSideBack,		
		eCubeSideMAX
	};

	// Class Constants
	static const int VERTEX_COUNT		= 8;		// # vertices
	static const int FACE_COUNT			= 6;		// # faces
	static const int VERTICES_PER_FACE	= 4;		// # vertices per face
	static const int FACES_PER_VERTEX	= 3;		// # faces adjoining any vertex

	// Class Variables
	static Cube* mCubeGrid[GRID_SIZE][GRID_SIZE];
	static bool  mScoreGrid[GRID_SIZE][GRID_SIZE];
	static bool  mLinkedGrid[GRID_SIZE][GRID_SIZE];
	static float defaultVertices[VERTEX_COUNT][3];
	static float defaultFaceVertexTexCoords[FACE_COUNT*VERTICES_PER_FACE][2];
	static float defaultFaceColours[FACE_COUNT][4];	
	static float defaultFaceNormals[FACE_COUNT][3];
	static float defaultVertexNormals[VERTEX_COUNT][3];
	static Image* mImage;
	static GLuint mnTextureId;

	// For each face, what are the vertices.
	// The values stored in this array are indices for the vertex array.
	// The vertices are specified in anticlockwise order looking at the 
	// face from outside the 3D object (Right-hand Corkscrew Rule)	
	static int   defaultFaceVertices[FACE_COUNT][VERTICES_PER_FACE];

	// State machine
	CubeState mState;

	// Orientation
	glm::vec3 mvRotation;
	TransformListNode* mpTransformationList;
	int mvArrSides[6];

	// Position
	glm::vec3 mvPosition;
	glm::vec3 mvLastPosition;
	glm::vec3 mvRollingToPosition;

	// Flare
	float mfBrightnessMultiplier;

	// Respawn time
	float mfRespawnTime;

public:
	Cube();
	~Cube();

	bool Init(glm::vec3 _position);
	void ComputeFaceNormals();
	void ComputeVertexNormals();
	void Spawn();
	void Respawn();
	void SetRandomOrientation();
	void ClearOrientation();
	void StartRolling(glm::vec3 _vTowards);
	void Roll(glm::vec3 _vNewPosition);
	void RollLeft();
	void RollRight();
	void RollForward();
	void RollBackward();
	void ComputeOrientation();
	void UpdateOrientation();
	int  UpdatePositionAndScore(int _x, int _z);
	void ResetScoreGrid();
	void ResetLinkedGrid();
	void SinkCubes();
	void StartSinking();
	void WipeGrid();
	int  ComputeScore(int _x, int _z);
	int  ComputeScoreFor1(int _x, int _z);
	bool Update();
	void Draw();	

	void  SetCubeAt(int _x, int _z, Cube* _cube);
	Cube* GetCubeAt(int _x, int _z);
	glm::vec3 GetPosition();
	int  GetTopFaceNumber();
	bool CubeIsIdle();
	bool CubeIsActive();
	bool CubeIsSpawning();
	bool CubeIsSinking();
	bool CubeIsRolling();
};

#endif