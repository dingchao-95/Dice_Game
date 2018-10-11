
#include "Player.h"

Player::Player()
{
	mState = ePlayerStateINVALID;
}

Player::~Player()
{

}

void Player::Init(int _nID, glm::vec3 _pos, glm::uvec4 _color, Cube* _referenceCube)
{
	mnID = _nID;
	mvColor = _color;
	mvPosition = mvLastPosition = mvTargetPosition = _pos;
	mpReferenceCube = _referenceCube;
	mfSquashedTimer = 0.0f;
	mnTargetLife = MAX_LIFE;
	mnLife = 0;
	mnScore = 0;

	// Find cube below us, we will always spawn on a cube
	mpCubeImRolling = mpReferenceCube->GetCubeAt((int)glm::round(mvPosition.x), (int)glm::round(mvPosition.z));	
	mState = ePlayerStateIdleOnCube;		
}

void Player::Move(glm::vec3 _movement, Player* _otherPlayer)
{
	if (mnID==1)
	{
		int i = 1;
	}

	// New position to move to
	glm::vec3 newPosition = mvPosition + _movement;

	// Check out of bounds of grid
	float bounds = GRID_SIZE / 2.0f;
	if (newPosition.x<-bounds || newPosition.x>=bounds || newPosition.z<-bounds || newPosition.z>=bounds)
		return;

	// Check for collisions against other player
	if (GetHorizontalDistance(newPosition, _otherPlayer->GetLastPosition())==0.0f || 
		GetHorizontalDistance(newPosition,_otherPlayer->GetTargetPosition())==0.0f) 
	{
		if (IsOnGround() && _otherPlayer->IsOnGround()) return;
		if (IsOnGround() && _otherPlayer->IsOnCube()) return;
		if (IsOnCube() && _otherPlayer->IsOnCube()) return;
		if (IsOnCube() && _otherPlayer->IsOnGround()) 
		{
			// Special case!
			if (mState==ePlayerStateIdleOnCube && mpCubeImRolling->CubeIsIdle())
				mState = ePlayerStateSquashing; 
			else return;
		}
	}

	// Check for a cube at destination
	Cube* cubeAtDestination = mpReferenceCube->GetCubeAt((int)glm::round(newPosition.x), (int)glm::round(newPosition.z));

	// Check destination cube inactivity
	if (cubeAtDestination && !cubeAtDestination->CubeIsActive()) cubeAtDestination = 0;

	// Check for cube below us
	mpCubeImRolling = mpReferenceCube->GetCubeAt((int)glm::round(mvPosition.x), (int)glm::round(mvPosition.z));			

	// Check below cube inactivity
	if (mpCubeImRolling && !mpCubeImRolling->CubeIsActive()) mpCubeImRolling = 0;

	// Note: Move only activates then Cube is currently IDLE (either on ground or on cubes)
	switch(mState)
	{

	case ePlayerStateIdleOnGround:

		// Cube exists at destination
		if (cubeAtDestination)
		{
			// Cannot move up a full cube if we're on the ground
			if (cubeAtDestination->CubeIsIdle())
				return;

			// But can move on a spawning/sinking cube
			else if (cubeAtDestination->CubeIsSpawning() || cubeAtDestination->CubeIsSinking())
			{
				// Assignment movement variables
				mvLastPosition = mvPosition;
				mvTargetPosition = newPosition;
				mpCubeImGoingTo = cubeAtDestination;
				mState = ePlayerStateMovingOnCubes;
			}
		}

		// Move on the ground!
		else
		{
			// Assignment movement variables
			mvLastPosition = mvPosition;
			mvTargetPosition = newPosition;

			mState = ePlayerStateMovingOnGround;
		}
		break;

	case ePlayerStateMovingOnGround:
		/**NOPE**/
		break;

	case ePlayerStateSquashing:
	case ePlayerStateIdleOnCube:

		// If there is a cube (idle/spawning/sinking), we will move over!
		if (cubeAtDestination && 
			(cubeAtDestination->CubeIsIdle() ||
			 cubeAtDestination->CubeIsSpawning() || 
			 cubeAtDestination->CubeIsSinking())
		   )
		{
			// Assignment movement variables
			mpCubeImGoingTo = cubeAtDestination;
			mvLastPosition = mvPosition;
			mvTargetPosition = newPosition;
			mState = ePlayerStateMovingOnCubes;
		}
		
		// If no cube at destination AND we have an IDLE cube, we'll be rolling ours!
		else if (!cubeAtDestination && mpCubeImRolling->CubeIsIdle())
		{
			// Assignment movement variables
			mvLastPosition = mvPosition;
			mvTargetPosition = newPosition;
			mpCubeImRolling->StartRolling(newPosition);
			mState = ePlayerStateRollingCube;
		}
		// If no cube at destination AND we are on a sinking/spawning cube, we can go onto the ground!
		else if (!cubeAtDestination && 
				(mpCubeImRolling->CubeIsSpawning() || 
				 mpCubeImRolling->CubeIsSinking())
				)
		{
			// Assignment movement variables
			mpCubeImGoingTo = mpCubeImRolling = 0;
			mvLastPosition = mvPosition;
			mvTargetPosition = newPosition;
			mState = ePlayerStateMovingOnGround;
		}
		break;

	case ePlayerStateRollingCube:
		/**NOPE**/
		break;

	case ePlayerStateMovingOnCubes:
		/**NOPE**/
		break;

	case ePlayerStateSquashed:
		/**NOPE**/
		break;

	}
}

bool Player::Update(Player* _otherPlayer)
{
	// Lower bound threshold to stop moving
	float bound = 0.01f;

	// Return value
	bool bReturn = false;

	// Update life
	if (mnLife!=mnTargetLife) 
	{
		mnLife += mnLife<mnTargetLife?+1:-1;
		bReturn = true;
	}

	switch(mState)
	{

	// Idle on ground == fixed floating height (you can't possible change height standing on ground)
	case ePlayerStateIdleOnGround:
		ComputeHeight();
		break;


	// Moving on ground, LERP, then fixed floating height
	case ePlayerStateMovingOnGround:
		{
			// Lerp, update position
			mvPosition = GetHorizontalLerp(mvPosition, mvTargetPosition);

			// We have reached
			if ( GetHorizontalDistance(mvPosition, mvTargetPosition) <= bound )
			{
				// Update our position
				mvLastPosition = mvPosition = mvTargetPosition;
				mState = ePlayerStateIdleOnGround;
			}

			ComputeHeight();
			bReturn = true;
		}
		break;


	// Idle on Cube == fixed floating height from Cube
	case ePlayerStateIdleOnCube:

		// Check for sinking cubes
		if (mpCubeImRolling->CubeIsSinking() && mpCubeImRolling->GetPosition().y<=-CUBE_SIZE)
		{
			// We're on the ground now!
			mpCubeImRolling = 0;
			mState = ePlayerStateIdleOnGround;
		}
		// Not sinking, are we squashing someone??
		else
		{
			// We are squashing the other guy!
			if (GetHorizontalDistance(_otherPlayer->GetPosition(), mvPosition)==0.0f && _otherPlayer->IsOnGround())
				_otherPlayer->OMGDioSquashed(); // Piak! XD
		}

		ComputeHeight();
		break;


	// Rolling on Cube == fixed floating height from Cube
	case ePlayerStateSquashing:
	case ePlayerStateRollingCube:
		{
			// Lerp, update position (before rolling cube)
			mvPosition = GetHorizontalLerp(mvPosition, mvTargetPosition);

			// Roll our cube
			if (mpCubeImRolling!=0)
			{
				mpCubeImRolling->Roll(mvPosition);
			}

			// We have reached
			if ( GetHorizontalDistance(mvPosition, mvTargetPosition) <= bound )
			{
				// Update our position
				mvLastPosition = mvPosition = mvTargetPosition;
				mState = ePlayerStateIdleOnCube;

				// We are squashing the other guy!
				if (GetHorizontalDistance(_otherPlayer->GetPosition(), mvPosition)==0.0f && _otherPlayer->IsOnGround())
					_otherPlayer->OMGDioSquashed(); // Piak! XD

				if (mpCubeImRolling!=0)
				{
					mnScore = mpCubeImRolling->UpdatePositionAndScore((int)glm::round(mvPosition.x), (int)glm::round(mvPosition.z));
				}
			}

			// Just stand on cubes normally and follow their height
			ComputeHeight();
			bReturn = true;
		}
		break;


	// Moving on Cubes, need to check which Cube we are on, then follow its height
	case ePlayerStateMovingOnCubes:
		{
			// Lerp
			mvPosition = GetHorizontalLerp(mvPosition, mvTargetPosition);

			// We have reached
			if ( GetHorizontalDistance(mvPosition,mvTargetPosition) <= bound )
			{
				// Update our position
				mvLastPosition = mvPosition = mvTargetPosition;
				mpCubeImRolling = mpCubeImGoingTo;
				mpCubeImGoingTo = 0;
				mState = ePlayerStateIdleOnCube;
			}

			ComputeHeight();
			bReturn = true;
		}
		break;

	// Squashed, we are flat, cannot move
	case ePlayerStateSquashed:
		mfSquashedTimer -= 0.025f;
		if (mfSquashedTimer<=0.0f)
		{
			// Find Cube below us
			Cube* cubeAtDestination = mpReferenceCube->GetCubeAt((int)glm::round(mvPosition.x), (int)glm::round(mvPosition.z));

			if (cubeAtDestination && !cubeAtDestination->CubeIsRolling())
			{
				mpCubeImRolling = cubeAtDestination;
				mState = ePlayerStateIdleOnCube;
			}
			else
				mState = ePlayerStateIdleOnGround;

			ComputeHeight();
			bReturn = true;
		}
		break;
	}
	
	return bReturn;
}

int Player::GetScore()
{
	if (mnScore)
	{
		int nReturnScore = mnScore;
		mnScore = 0;
		return nReturnScore;
	}
	return 0;
}

void Player::UpdateLife(int _change)
{
	mnTargetLife += _change;
	if (mnTargetLife<0) mnTargetLife = 0;
}

int Player::GetLife()
{
	return mnLife;
}

bool Player::IsAlive()
{
	return (mnLife>=0);
}

void Player::Draw()
{
	glPushMatrix();
	glColor4f(mvColor.r, mvColor.g, mvColor.b, mvColor.a);	

	if (IsSquashed())
	{
		glTranslatef(mvPosition.x * CUBE_SIZE, 0.01f, mvPosition.z * CUBE_SIZE);
		glScalef(1.0f, 0.01f, 1.0f);
		glutSolidSphere(2.0f, 10, 10);		
	}
	else
	{
		glTranslatef(mvPosition.x * CUBE_SIZE, mvPosition.y, mvPosition.z * CUBE_SIZE);
		glutSolidSphere(2.0f, 10, 10);
	}	

	glPopMatrix();
}

glm::vec3 Player::GetPosition()
{
	return mvPosition;
}

glm::vec3 Player::GetLastPosition()
{
	return mvLastPosition;
}

glm::vec3 Player::GetTargetPosition()
{
	return mvTargetPosition;
}

void Player::ComputeHeight()
{
	// Find Cube below us
	Cube* cubeAtDestination = mpReferenceCube->GetCubeAt((int)glm::round(mvPosition.x), (int)glm::round(mvPosition.z));

	if (cubeAtDestination)
	{
		if (IsOnGround() && !cubeAtDestination->CubeIsSpawning())
			mvPosition.y = FLOATING_HEIGHT;			
		else
			mvPosition.y = cubeAtDestination->GetPosition().y + CUBE_SIZE + FLOATING_HEIGHT;
	}
	else
		mvPosition.y = FLOATING_HEIGHT;

	
}

float Player::GetHorizontalDistance(glm::vec3 _v1, glm::vec3 _v2)
{
	_v1.y = 0.0f;
	_v2.y = 0.0f;
	float fLength = glm::length(_v1 - _v2);
	if (fLength<=0.05f) fLength = 0.0f;
	return fLength;
}

glm::vec3 Player::GetHorizontalLerp(glm::vec3 _v1, glm::vec3 _v2)
{
	// Interpolation factor
	float factor = 0.3f;

	_v1.y = 0.0f;
	_v2.y = 0.0f;
	return _v1 + factor * (_v2 - _v1);
}

void Player::OMGDioSquashed()
{
	// Xiao liao
	mfSquashedTimer = 3.0f;
	mState = ePlayerStateSquashed;	
}

bool Player::IsOnGround()
{
	switch (mState)
	{
	case ePlayerStateIdleOnGround:
	case ePlayerStateMovingOnGround:
	case ePlayerStateSquashed:
		return true;
	case ePlayerStateIdleOnCube:
	case ePlayerStateRollingCube:
	case ePlayerStateMovingOnCubes:		
	case ePlayerStateSquashing:
		return false;	
	}
	return false;
}

bool Player::IsOnCube()
{
	return !IsOnGround();
}

bool Player::IsSquashed()
{
	return (mState==ePlayerStateSquashed);
}