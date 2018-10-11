#ifndef CAMERA_H
#define CAMERA_H

#include "GL\glut.h"
#include "glm.hpp"

class Camera
{
private:
	float mfAngle;
	float mfElevation;
	float mfDistanceFromOrigin;
	float mfSensitivity;
	glm::vec3 mvPos;

public:
	Camera();
	~Camera();

	void Update();
	void Set();
	void Focus(float _fFOV, float _fAspectRatio, float _fNear, float _fFar);
	void Rotate(float _fAngle, float _fElevation);
	glm::vec3 GetPosition();
};

#endif