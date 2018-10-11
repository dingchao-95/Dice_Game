
#include "Cube.h"

Cube*  Cube::mCubeGrid[GRID_SIZE][GRID_SIZE] = { 0 };
bool   Cube::mScoreGrid[GRID_SIZE][GRID_SIZE] = { 0 };
bool   Cube::mLinkedGrid[GRID_SIZE][GRID_SIZE] = { 0 };
Image* Cube::mImage = 0;
GLuint Cube::mnTextureId = -1;

float  Cube::defaultVertices[VERTEX_COUNT][3] = {					
	{ CUBE_SIZE/2,  CUBE_SIZE/2, -CUBE_SIZE/2 }, // 0 - TopRightBack
	{-CUBE_SIZE/2,  CUBE_SIZE/2, -CUBE_SIZE/2 }, // 1 - TopLeftBack
	{-CUBE_SIZE/2,  CUBE_SIZE/2,  CUBE_SIZE/2 }, // 2 - TopLeftFront
	{ CUBE_SIZE/2,  CUBE_SIZE/2,  CUBE_SIZE/2 }, // 3 - TopRightFront
	{ CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2 },
	{-CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2 },
	{-CUBE_SIZE/2, -CUBE_SIZE/2,  CUBE_SIZE/2 },
	{ CUBE_SIZE/2, -CUBE_SIZE/2,  CUBE_SIZE/2 }
};
float Cube::defaultFaceVertexTexCoords[FACE_COUNT*VERTICES_PER_FACE][2] = {
	// TASK#2: Define the UV Coordinates of the faces
	// f1 (top)
	{ 0.5f, 1.0f },	// v0
	{ 0.25f, 1.0f },	// v1	
	{ 0.25f, 0.75f },	// v2	
	{ 0.5f, 0.75f },	// v3

	// f2 (left)
	{ 0.0f, 0.5f },	// v5
	{ 0.25f , 0.5f },	// v6	
	{ 0.25f, 0.75f },	// v2	
	{ 0.0f, 0.75f },	// v1

	// f3 (bottom)
	{ 0.5f, 0.5f },	// v5
	{ 0.25f ,0.5f },	// v4	
	{ 0.25f, 0.25f },	// v7	
	{ 0.5f, 0.25f },	// v6

	// f4 (right)
	{ 0.75f, 0.5f },	// v7
	{ 0.75f, 0.75f },	// v4	
	{ 0.5f, 0.75f },	// v0	
	{ 0.5f, 0.5f },	// v3

	// f5 (front)
	{1.0f, 0.75f },	// v6
	{ 0.75f, 0.75f },	// v7	
	{ 0.75f, 0.5f },	// v3	
	{ 1.0f, 0.5f },	// v2

	// f6 (back)
	{ 0.5f, 0.5f },	// v4
	{ 0.5f, 0.75f },	// v5	
	{ 0.25f, 0.75f },	// v1	
	{ 0.25f, 0.5f },	// v0

};
int Cube::defaultFaceVertices[FACE_COUNT][VERTICES_PER_FACE] = {
	{ 2, 3, 0, 1 },	// f1 (top) ONE
	{ 5, 6, 2, 1 },	// f2 (left) THREE
	{ 5, 4, 7, 6 },	// f3 (bottom) SIX
	{ 7, 4, 0, 3 },	// f4 (right) FOUR
	{ 6, 7, 3, 2 },	// f5 (front) FIVE
	{ 4, 5, 1, 0 }	// f6 (back) TWO
};

float Cube::defaultFaceColours[FACE_COUNT][4] = {
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // f1 (top) ONE (White) 
	{ 0.6f, 1.0f, 0.6f, 1.0f }, // f5 (front) FIVE
	{ 1.0f, 0.4f, 0.4f, 1.0f }, // f3 (bottom) SIX 
	{ 1.0f, 1.0f, 0.0f, 1.0f },	// f6 (back) TWO
	{ 0.6f, 1.0f, 1.0f, 1.0f }, // f4 (right) FOUR
	{ 0.8f, 0.5f, 1.0f, 1.0f } // f2 (left) THREE
	
};
float Cube::defaultFaceNormals[FACE_COUNT][3] = { 0 };
float Cube::defaultVertexNormals[VERTEX_COUNT][3] = { 0 };


Cube::Cube()
{
	ComputeFaceNormals();
	ComputeVertexNormals();	
}

void Cube::ComputeFaceNormals()
{
	// TASK#3: Compute face normals - 10marks
	// For each face, identify 3 of the vertices, and using them, construct 2 vectors.
	// Calculate the cross product for each face and store them.
	//defaultVertices[VERTEX_COUNT][3]
	//defaultFaceVertices[FACE_COUNT][VERTICES_PER_FACE]
	//defaultFaceNormals[FACE_COUNT][3]
	glm::vec3 v1;
	glm::vec3 v2;
	for (int i=0; i<FACE_COUNT; i++)
	{
		v1.x = defaultVertices [defaultFaceVertices[i][0]] [0] - defaultVertices [defaultFaceVertices[i][1]] [0];
		v1.y = defaultVertices [defaultFaceVertices[i][0]] [1] - defaultVertices [defaultFaceVertices[i][1]] [1];
		v1.z = defaultVertices [defaultFaceVertices[i][0]] [2] - defaultVertices [defaultFaceVertices[i][1]] [2];

		v2.x = defaultVertices [defaultFaceVertices[i][2]] [0] - defaultVertices [defaultFaceVertices[i][1]] [0];
		v2.y = defaultVertices [defaultFaceVertices[i][2]] [1] - defaultVertices [defaultFaceVertices[i][1]] [1];
		v2.z = defaultVertices [defaultFaceVertices[i][2]] [2] - defaultVertices [defaultFaceVertices[i][1]] [2];
		

		defaultFaceNormals[i][0] = glm::cross(v2, v1).x;
		defaultFaceNormals[i][1] = glm::cross(v2, v1).y;
		defaultFaceNormals[i][2] = glm::cross(v2, v1).z;

		 
		float magnitude = sqrt( defaultFaceNormals[i][0] * defaultFaceNormals[i][0] + 
							   defaultFaceNormals[i][1] * defaultFaceNormals[i][1] + 
							   defaultFaceNormals[i][2] * defaultFaceNormals[i][2] );

		defaultFaceNormals[i][0] = defaultFaceNormals[i][0]/magnitude;
		defaultFaceNormals[i][1] = defaultFaceNormals[i][1]/magnitude;
		defaultFaceNormals[i][2] = defaultFaceNormals[i][2]/magnitude;



	}
}

void Cube::ComputeVertexNormals()
{
	// TASK#3: Compute vertex normals - 10marks
	// For each vertex, loop through all faces, and see the vertex is part of that face
	// If so, add up the normal of that face
	// After all of them have been added, average it or normalize it and store the normal.
	for (int vertexIdx=0; vertexIdx<VERTEX_COUNT; vertexIdx++)
	{
		int firstIdx = 10;
		int secondIdx = 10;
		int thirdIdx = 10;
		for(int i = 0; i<FACE_COUNT; i++)
		{
			for(int j = 0; j<VERTICES_PER_FACE; j++)
			{

				if( defaultFaceVertices[i][j] == vertexIdx )
				{
					if( firstIdx == 10 )
					{
						firstIdx = i;
					}

					else if( secondIdx == 10 )
					{
						secondIdx = i;
					}
					
					else if( thirdIdx == 10 )
					{
						thirdIdx = i;
					}
				}
			}

		}
		glm::vec3 firstVector = glm::vec3(defaultFaceNormals[firstIdx][0], defaultFaceNormals[firstIdx][1], defaultFaceNormals[firstIdx][2]);
		glm::vec3 secondVector = glm::vec3(defaultFaceNormals[secondIdx][0], defaultFaceNormals[secondIdx][1], defaultFaceNormals[secondIdx][2]);
		glm::vec3 thirdVector = glm::vec3(defaultFaceNormals[thirdIdx][0], defaultFaceNormals[thirdIdx][1], defaultFaceNormals[thirdIdx][2]);
		
		glm::vec3 sum = firstVector + secondVector + thirdVector;
		sum = glm::normalize(sum);

		defaultVertexNormals[vertexIdx][0] = sum.x;
		defaultVertexNormals[vertexIdx][1] = sum.y;
		defaultVertexNormals[vertexIdx][2] = sum.z;


	}
}

Cube::~Cube()
{

}

bool Cube::Init(glm::vec3 _position)
{

	if (mImage==0) 
	{
		// TASK#2: Load the textures here
		mImage = loadBMP("dice.bmp");
		mnTextureId = loadTexture(mImage);
		if (mImage) delete mImage;
	}

	// Cube Sides
	mvArrSides[eCubeSideTop] = 1;
	mvArrSides[eCubeSideBack] = 2;
	mvArrSides[eCubeSideLeft] = 3;
	mvArrSides[eCubeSideRight] = 4;
	mvArrSides[eCubeSideFront] = 5;
	mvArrSides[eCubeSideBottom] = 6;

	if (GetCubeAt((int)_position.x, (int)_position.z)==0)
	{
		mvPosition = mvLastPosition = _position;
		mvPosition.y = -0.5f-CUBE_SIZE;
		SetRandomOrientation();
		SetCubeAt((int)_position.x, (int)_position.z, this);
		mfBrightnessMultiplier = 1.0f;
		mfRespawnTime = 10.0f;
		mState = eCubeStateInactive;
		return true;
	}
	return false;
}

void Cube::Spawn()
{
	mState = eCubeStateSpawning;
}

void Cube::Respawn()
{
	// Find a random empty spot on the grid
	int x, z;
	do
	{
		x = rand()%GRID_SIZE;
		z = rand()%GRID_SIZE;
	} while (mCubeGrid[x][z]!=0);

	// Init
	ClearOrientation();
	mvPosition.x = (float)(x - (GRID_SIZE>>1));
	mvPosition.z = (float)(z - (GRID_SIZE>>1));
	Init(mvPosition);
	Spawn();
}

void Cube::SetRandomOrientation()
{
	int max_rolls = 5;
	int randDirection = 0;
	mpTransformationList = 0;
	for (int rolls=0; rolls<max_rolls; rolls++)
	{
		randDirection = rand()%4;
		TransformListNode* pNewNode = new TransformListNode();
		switch(randDirection)
		{
		case 0: // Left
			pNewNode->mvTransform.w = 90.0f;
			pNewNode->mvTransform.x = 0.0f;
			pNewNode->mvTransform.y = 0.0f; 
			pNewNode->mvTransform.z = 1.0f;
			RollLeft();
			break;
		case 1: // Right
			pNewNode->mvTransform.w = -90.0f;
			pNewNode->mvTransform.x = 0.0f;
			pNewNode->mvTransform.y = 0.0f; 
			pNewNode->mvTransform.z = 1.0f;
			RollRight();
			break;
		case 2: // Back
			pNewNode->mvTransform.w = -90.0f;
			pNewNode->mvTransform.x = 1.0f;
			pNewNode->mvTransform.y = 0.0f; 
			pNewNode->mvTransform.z = 0.0f;
			RollBackward();
			break;
		case 3: // Front
			pNewNode->mvTransform.w = 90.0f;
			pNewNode->mvTransform.x = 1.0f;
			pNewNode->mvTransform.y = 0.0f; 
			pNewNode->mvTransform.z = 0.0f;
			RollForward();
			break;
		}	
		// Add to front of list
		pNewNode->mpNext = mpTransformationList;
		mpTransformationList = pNewNode;
	}
}

void Cube::ClearOrientation()
{
	while (mpTransformationList)
	{
		TransformListNode* pDelete = mpTransformationList;
		mpTransformationList = mpTransformationList->mpNext;
		delete pDelete;
	}
}

void Cube::StartRolling(glm::vec3 _vTowards)
{
	switch (mState)
	{
	case eCubeStateSpawning:
		/***/
		break;
	case eCubeStateIdle:
		// Hello! This space is taken!! Flag target grid as occupied (we now occupy 2 spaces)
		SetCubeAt((int)glm::round(_vTowards.x), (int)glm::round(_vTowards.z), this);

		// Store current position;
		mvLastPosition = mvPosition;

		// Store target position
		mvRollingToPosition = _vTowards;
		break;
	case eCubeStateRolling:
		/***/
		break;
	case eCubeStateSinking:
		/***/
		break;
	case eCubeStateInactive:
		/***/
		break;
	}


}

void Cube::Roll(glm::vec3 _vNewPosition)
{
	// We do not want the player's y to affect ours
	mvPosition.x = _vNewPosition.x;
	mvPosition.z = _vNewPosition.z;
}

void Cube::RollLeft()
{
	// Rotate Top, right, bottom, left
	int temp = mvArrSides[eCubeSideTop];
	mvArrSides[eCubeSideTop] = mvArrSides[eCubeSideRight];
	mvArrSides[eCubeSideRight] = mvArrSides[eCubeSideBottom];
	mvArrSides[eCubeSideBottom] = mvArrSides[eCubeSideLeft];
	mvArrSides[eCubeSideLeft] = temp;
}

void Cube::RollRight()
{
	// Rotate Top, left, bottom, right
	int temp = mvArrSides[eCubeSideTop];
	mvArrSides[eCubeSideTop] = mvArrSides[eCubeSideLeft];
	mvArrSides[eCubeSideLeft] = mvArrSides[eCubeSideBottom];
	mvArrSides[eCubeSideBottom] = mvArrSides[eCubeSideRight];
	mvArrSides[eCubeSideRight] = temp;
}

void Cube::RollForward()
{
	// Rotate Top, back, bottom, front
	int temp = mvArrSides[eCubeSideTop];
	mvArrSides[eCubeSideTop] = mvArrSides[eCubeSideBack];
	mvArrSides[eCubeSideBack] = mvArrSides[eCubeSideBottom];
	mvArrSides[eCubeSideBottom] = mvArrSides[eCubeSideFront];
	mvArrSides[eCubeSideFront] = temp;
}

void Cube::RollBackward()
{
	// Rotate Top, front, bottom, back
	int temp = mvArrSides[eCubeSideTop];
	mvArrSides[eCubeSideTop] = mvArrSides[eCubeSideFront];
	mvArrSides[eCubeSideFront] = mvArrSides[eCubeSideBottom];
	mvArrSides[eCubeSideBottom] = mvArrSides[eCubeSideBack];
	mvArrSides[eCubeSideBack] = temp;
}

void Cube::ComputeOrientation()
{
	// TASK#5: Compute the die's ONE TRUE CURRENT ORIENTATION!
	//check if the x axis is neg or pos , if its positive, move left, if negative, move right
	//current position - lastposition * 90 = angle of rotation
	//store in glrotatef
	//do the same for the z axis
	//Origin

	
	TransformListNode* newNode;

	//Check if it is rotating in the correct direction

	float diffZ = mvPosition.z - mvLastPosition.z; // Difference in z position when Current position - LastKnownPosition
	float diffX = mvPosition.x - mvLastPosition.x; // Difference in x position when Current position - LastKnownPosition
	if( mvPosition.z - mvLastPosition.z == 0.0f && mvPosition.x - mvLastPosition.x == 0.0f)
	{
		glTranslatef( mvPosition.x * CUBE_SIZE, mvPosition.y + (CUBE_SIZE / 2 ) , mvPosition.z * CUBE_SIZE);
	}
	else
	{
		if(diffZ > 0 || diffZ < 0) // check if there are any difference in the Z axis 
		{
			glTranslatef( mvLastPosition.x * CUBE_SIZE, (mvLastPosition.y + CUBE_SIZE) / 2  , mvLastPosition.z * CUBE_SIZE); // Shift to last position if its moving front or back

			if( diffZ > 0 ) // check if the die is moving frontwards
			{
				glTranslatef(0.0f, -5.0f, 5.0f); 
				glRotatef( diffZ * 90.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, 5.0f, -5.0f);
			}
			if(  diffZ < 0 ) // check if die is moving backwards
			{
				glTranslatef(0.0f, -5.0f, -5.0f);
				glRotatef( diffZ * 90.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f,  5.0f,  5.0f);
			}
		}
		if(diffX > 0 || diffX < 0) //Check if there are any difference in the X acis
		{
			glTranslatef( mvLastPosition.x * CUBE_SIZE, (mvLastPosition.y + CUBE_SIZE) / 2  , mvLastPosition.z * CUBE_SIZE); //move to last position

			if ( diffX > 0 )// check if it is moving right
			{
				glTranslatef(5.0f, -5.0f, 0.0f);
				glRotatef( diffX * -90.0f, 0.0f, 0.0f, 1.0f);
				glTranslatef(-5.0f, 5.0f, 0.0f);
			}
			else if ( diffX < 0 ) // check if it is moving left
			{
				glTranslatef(-5.0f, -5.0f, 0.0f);
				glRotatef( diffX * -90.0f, 0.0f, 0.0f, 1.0f);
				glTranslatef(5.0f, 5.0f, 0.0f);
			}
		}
	}


	//Traverse the linkedlist and get the 'history' of the orientation of the dice
	for( newNode = mpTransformationList; newNode != NULL; newNode = newNode->mpNext)
	{
		glRotatef(newNode->mvTransform.w , newNode->mvTransform.x, newNode->mvTransform.y , newNode->mvTransform.z);
	}	


}

void Cube::UpdateOrientation()
{
	// Update orientation
	float deltaX = mvPosition.x - mvLastPosition.x;
	float deltaZ = mvPosition.z - mvLastPosition.z;

	if (deltaX || deltaZ)
	{
		TransformListNode* pNewNode = new TransformListNode();
		if (deltaX<0.0f) // Left
		{
			pNewNode->mvTransform.w = 90.0f;
			pNewNode->mvTransform.x = 0.0f;
			pNewNode->mvTransform.y = 0.0f; 
			pNewNode->mvTransform.z = 1.0f;
			RollLeft();
		}
		else if (deltaX>0.0f) // Right
		{
			pNewNode->mvTransform.w = -90.0f;
			pNewNode->mvTransform.x = 0.0f;
			pNewNode->mvTransform.y = 0.0f; 
			pNewNode->mvTransform.z = 1.0f;
			RollRight();
		}
		else if (deltaZ<0.0f) // Back
		{
			pNewNode->mvTransform.w = -90.0f;
			pNewNode->mvTransform.x = 1.0f;
			pNewNode->mvTransform.y = 0.0f; 
			pNewNode->mvTransform.z = 0.0f;
			RollBackward();
		}
		else if (deltaZ>0.0f) // Front
		{	
			pNewNode->mvTransform.w = 90.0f;
			pNewNode->mvTransform.x = 1.0f;
			pNewNode->mvTransform.y = 0.0f; 
			pNewNode->mvTransform.z = 0.0f;
			RollForward();
		}
		// Add to front of list
		pNewNode->mpNext = mpTransformationList;
		mpTransformationList = pNewNode;
	}
}

int Cube::UpdatePositionAndScore(int _x, int _z)
{
	UpdateOrientation();
	SetCubeAt((int)glm::round(mvLastPosition.x), (int)glm::round(mvLastPosition.z), 0);
	mvPosition.x = mvLastPosition.x = (float)_x;
	mvPosition.z = mvLastPosition.z = (float)_z;

	// Check number of linked cubes and score
	ResetScoreGrid();
	ResetLinkedGrid();
	if (GetTopFaceNumber()==1)
	{
		int nScore = ComputeScoreFor1(_x, _z);
		if (nScore)
		{
			SinkCubes();
			return nScore;
		}
	}
	else
	{
		int linkedCubes = ComputeScore(_x, _z);
		if (linkedCubes>=GetTopFaceNumber())
		{
			SinkCubes();
			return linkedCubes*GetTopFaceNumber();
		}
	}

	return 0;
}

void Cube::SinkCubes()
{
	for (int x=0; x<GRID_SIZE; x++)
		for (int z=0; z<GRID_SIZE; z++)
			if (mLinkedGrid[x][z])
				mCubeGrid[x][z]->StartSinking();
}

void Cube::StartSinking()
{
	mfBrightnessMultiplier = 3.0f;
	// Raise a little, but cannot be higher than max height
	mvPosition.y += 0.5f;
	if (mvPosition.y > 0.0f) mvPosition.y = 0.0f;
	mState = eCubeStateSinking;
}

void Cube::ResetScoreGrid()
{
	for (int x=0; x<GRID_SIZE; x++)
		for (int z=0; z<GRID_SIZE; z++)
			mScoreGrid[x][z] = false;
}

void Cube::ResetLinkedGrid()
{
	for (int x=0; x<GRID_SIZE; x++)
		for (int z=0; z<GRID_SIZE; z++)
			mLinkedGrid[x][z] = false;
}

void Cube::WipeGrid()
{
	for (int x=0; x<GRID_SIZE; x++)
		for (int z=0; z<GRID_SIZE; z++)
			SetCubeAt(x, z, 0);
}

int Cube::ComputeScore(int _x, int _z)
{
	int sum = 1;
	int offset = (GRID_SIZE>>1);
	int x = _x + offset;
	int z = _z + offset;
	mScoreGrid[x][z] = true;
	mLinkedGrid[x][z] = true;

	// Check Left
	if (x>0 && mCubeGrid[x-1][z] && !mScoreGrid[x-1][z])
	{
		if (mCubeGrid[x-1][z]->GetTopFaceNumber()==GetTopFaceNumber())
		{			
			sum += ComputeScore(_x-1, _z);
		}
		mScoreGrid[x-1][z] = true;
	}
	// Check Right
	if (x<(GRID_SIZE-1) && mCubeGrid[x+1][z] && !mScoreGrid[x+1][z])
	{
		if (mCubeGrid[x+1][z]->GetTopFaceNumber()==GetTopFaceNumber())
		{		
			sum += ComputeScore(_x+1, _z);
		}
		mScoreGrid[x+1][z] = true;
	}
	// Check Back
	if (z>0 && mCubeGrid[x][z-1] && !mScoreGrid[x][z-1])
	{
		if (mCubeGrid[x][z-1]->GetTopFaceNumber()==GetTopFaceNumber())
		{			
			sum += ComputeScore(_x, _z-1);
		}
		mScoreGrid[x][z-1] = true;
	}	
	// Check Front
	if (z<(GRID_SIZE-1) && mCubeGrid[x][z+1] && !mScoreGrid[x][z+1])
	{
		if (mCubeGrid[x][z+1]->GetTopFaceNumber()==GetTopFaceNumber())
		{			
			sum += ComputeScore(_x, _z+1);
		}
		mScoreGrid[x][z+1] = true;
	}
	return sum;
}

int  Cube::ComputeScoreFor1(int _x, int _z)
{
	int offset = (GRID_SIZE>>1);
	int x = _x + offset;
	int z = _z + offset;
	bool bScore = false;

	// Check Left
	if (x>0 && mCubeGrid[x-1][z] && !mScoreGrid[x-1][z])
		if (mCubeGrid[x-1][z]->CubeIsSinking())
			bScore = true;

	// Check Right
	if (x<(GRID_SIZE-1) && mCubeGrid[x+1][z] && !mScoreGrid[x+1][z])
		if (mCubeGrid[x+1][z]->CubeIsSinking())
			bScore = true;

	// Check Back
	if (z>0 && mCubeGrid[x][z-1] && !mScoreGrid[x][z-1])
		if (mCubeGrid[x][z-1]->CubeIsSinking())
			bScore = true;

	// Check Front
	if (z<(GRID_SIZE-1) && mCubeGrid[x][z+1] && !mScoreGrid[x][z+1])
		if (mCubeGrid[x][z+1]->CubeIsSinking())
			bScore = true;

	if (bScore)
	{
		int sum = 0;
		for (int x=0; x<GRID_SIZE; x++)
		{
			for (int z=0; z<GRID_SIZE; z++)
			{
				if (mCubeGrid[x][z] && mCubeGrid[x][z]->GetTopFaceNumber()==1)
				{
					sum++;
					mLinkedGrid[x][z] = true;
				}
			}
		}
		return sum;
	}

	return 0;
}

bool Cube::Update()
{
	switch (mState)
	{

	case eCubeStateSpawning:
		mvPosition.y += 0.05f;
		if (mvPosition.y>=0.0f)
		{
			mvPosition.y = 0.0f;
			mState = eCubeStateIdle;
		}
		return true;

	case eCubeStateIdle:
		/***/
		break;

	case eCubeStateRolling:
		/***/
		return true;

	case eCubeStateSinking:
		mvPosition.y -= 0.025f;
		if (mvPosition.y<-0.05f-CUBE_SIZE)
		{
			SetCubeAt((int)glm::round(mvPosition.x), (int)glm::round(mvPosition.z), 0);
			mfRespawnTime = 3.0f + (rand()%5);
			mState = eCubeStateInactive;
		}
		return true;

	case eCubeStateInactive:
		mfRespawnTime -= 0.025f;
		if (mfRespawnTime<=0.0f) Respawn();
		break;
	}
	return false;
}

void Cube::Draw()
{
	switch (mState)
	{
	case eCubeStateSpawning:
	case eCubeStateIdle:
	case eCubeStateRolling:
	case eCubeStateSinking:
	glPushMatrix();

		 
		//glTranslatef(mvPosition.x * CUBE_SIZE, mvPosition.y + CUBE_SIZE/2, mvPosition.z * CUBE_SIZE);

		glPushMatrix();
		ComputeOrientation();
		
		// TASK#2: Enable Textures, and apply them to the cube - 10marks
		glEnable(GL_TEXTURE_2D);	
		glBindTexture(GL_TEXTURE_2D, mnTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		
		// TASK#1: Using GL_QUADS, write code to draw out this cube - 10marks
		

		glBegin( GL_QUADS );

		int vertexIndex;
		for (int face=0; face < FACE_COUNT; face++)
		{
			
			for (int vertex=0; vertex<VERTICES_PER_FACE; vertex++)
			{
				
				vertexIndex = defaultFaceVertices[face][vertex];

				glColor4f(defaultFaceColours[face][0] ,defaultFaceColours[face][1] ,defaultFaceColours[face][2] ,defaultFaceColours[face][3]);

				glTexCoord2f(defaultFaceVertexTexCoords[face * VERTICES_PER_FACE + vertex][0], 
							defaultFaceVertexTexCoords[face * VERTICES_PER_FACE + vertex][1]);

				//glNormal3f( defaultFaceNormals[face][0], defaultFaceNormals[face][1], defaultFaceNormals[face][2] );

				glNormal3f( defaultVertexNormals[defaultFaceVertices[face][vertex]][0] ,defaultVertexNormals[defaultFaceVertices[face][vertex]][1] , defaultVertexNormals[defaultFaceVertices[face][vertex]][2] );

				glVertex3f(defaultVertices[vertexIndex][0],    defaultVertices[vertexIndex][1],    defaultVertices[vertexIndex][2]);
				
			}
			
		}

		

		glEnd();
    

		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
		glPopMatrix();
	
		break;
	case eCubeStateInactive:
		break;
	}
}

void Cube::SetCubeAt(int _x, int _z, Cube* _cube)
{
	mCubeGrid[_x + (GRID_SIZE>>1)][_z + (GRID_SIZE>>1)] = _cube;
}

Cube* Cube::GetCubeAt(int _x, int _z)
{
	return mCubeGrid[_x + (GRID_SIZE>>1)][_z + (GRID_SIZE>>1)];
}

glm::vec3 Cube::GetPosition()
{
	return mvPosition;
}

int Cube::GetTopFaceNumber()
{
	return mvArrSides[eCubeSideTop];
}

bool Cube::CubeIsIdle()
{
	return (mState==eCubeStateIdle);
}

bool Cube::CubeIsActive()
{
	return (mState!=eCubeStateInactive);
}

bool Cube::CubeIsSpawning()
{
	return (mState==eCubeStateSpawning);
}

bool Cube::CubeIsSinking()
{
	return (mState==eCubeStateSinking);
}

bool Cube::CubeIsRolling()
{
	return (mState==eCubeStateRolling);
}