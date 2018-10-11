#ifndef PLAYER_H
#define PLAYER_H

#include "Globals.h"
#include "GL\freeglut.h"
#include "glm.hpp"
#include "math.h"
#include "Cube.h"

class Player
{
private:
	enum PlayerState {
		ePlayerStateINVALID,
		ePlayerStateIdleOnGround,
		ePlayerStateMovingOnGround,
		ePlayerStateIdleOnCube,
		ePlayerStateRollingCube,
		ePlayerStateMovingOnCubes,
		ePlayerStateSquashing,
		ePlayerStateSquashed,
		ePlayerStateMAX
	};

	glm::vec4 mvColor;
	glm::vec3 mvPosition;
	glm::vec3 mvLastPosition;
	glm::vec3 mvTargetPosition;
	Cube* mpReferenceCube;
	Cube* mpCubeImGoingTo;
	Cube* mpCubeImRolling;
	PlayerState mState;
	float mfSquashedTimer;
	int mnID;
	int mnLife;
	int mnTargetLife;
	int mnScore;

public:
	Player();
	~Player();

	void Init(int _nID, glm::vec3 _pos, glm::uvec4 _color, Cube* _referenceCube);
	void Move(glm::vec3 _movement, Player* _otherPlayer);
	bool Update(Player* _otherPlayer);
	void UpdateLife(int _change);
	int GetScore();
	int GetLife();
	bool IsAlive();
	void Draw();
	glm::vec3 GetPosition();
	glm::vec3 GetLastPosition();
	glm::vec3 GetTargetPosition();
	void ComputeHeight();
	float GetHorizontalDistance(glm::vec3 _v1, glm::vec3 _v2);
	glm::vec3 GetHorizontalLerp(glm::vec3 _v1, glm::vec3 _v2);
	void OMGDioSquashed();
	bool IsOnGround();
	bool IsOnCube();
	bool IsSquashed();


};

#endif