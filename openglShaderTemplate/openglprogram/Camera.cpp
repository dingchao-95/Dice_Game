
#include "Camera.h"

Camera::Camera()
{
	mfAngle = 35.0f;
	mfElevation = 40.0f;
	mfDistanceFromOrigin = 180.0f;
	mvPos.x = mvPos.y = mvPos.z = 0.0f;
	mfSensitivity = 0.35f;

	Update();
}

Camera::~Camera()
{

}

// Sets the lens to focus with
void Camera::Focus(float _fFOV, float _fAspectRatio, float _fNear, float _fFar)
{
	gluPerspective(_fFOV, _fAspectRatio, _fNear, _fFar);
}

// Updates the camera position based on rotation angles
void Camera::Update()
{
	mvPos.y = sinf((float)mfElevation*3.142f/180.0f) * mfDistanceFromOrigin;
	float fBaseLength = cosf((float)mfElevation*3.142f/180.0f) * mfDistanceFromOrigin;
	mvPos.x = sinf((float)mfAngle*3.142f/180.0f) * fBaseLength;
	mvPos.z = cosf((float)mfAngle*3.142f/180.0f) * fBaseLength;
}

// Updates the camera settings into OpenGL
void Camera::Set()
{
	gluLookAt(mvPos.x, mvPos.y, mvPos.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

// Rotates the camera
void Camera::Rotate(float _fAngle, float _fElevation)
{
	mfAngle += _fAngle * mfSensitivity;
	if (mfAngle > 360.0f) mfAngle -= 360.0f;
	else if (mfAngle < -360.0f) mfAngle += 360.0f;

	mfElevation += _fElevation * mfSensitivity;
	if (mfElevation > 89.9f) mfElevation = 89.9f;
	else if (mfElevation < -89.9f) mfElevation = -89.9f;

	Update();
}

glm::vec3 Camera::GetPosition()
{
	return mvPos;
}