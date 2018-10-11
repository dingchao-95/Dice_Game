#ifndef ParticleEmitter_H
#define ParticleEmitter_H

#include "Globals.h"
#include "GL\glut.h"
#include "glm.hpp"
#include "ImageLoader.h"
#include "Camera.h"
#include <vector>
#include <algorithm>


// Represents a single particle
struct Particle {
    glm::vec3 mvPosition;
    glm::vec3 mvVelocity;
    glm::vec3 mvColorStart;
	glm::vec3 mvColorEnd;
	float mfDistanceFromCamera;
    float mfTimeAlive; // The amount of time that this particle has been alive
    float mfLifespan;  // The total amount of time that this particle is to live
};

// Constants
const float GRAVITY = 20.0f;
const int NUM_PARTICLES = 2500 ;
const float PARTICLE_SIZE = 13.0f; // The length of the sides of the quadrilateral drawn for each particle

class ParticleEmitter
{
private:
	// Textures
	GLuint mnTextureParticleId;

	// Array of Particles in this Emitter
    Particle* mArrParticles[NUM_PARTICLES];

	// Position of emitter
	glm::vec3 mvPosition;

	// Direction of emitter
	glm::vec3 mvDirection;

	// Color of the emitter
	glm::vec3 mvColorStart;
	glm::vec3 mvColorEnd;

	// Particle Speed
	float mfSpeed;

	// Particle Spread
	float mfSpread;

	// Affected by gravity?
	bool mbGravity;

	// Pointer to camera
	Camera* mpCamera;

public:
	ParticleEmitter(glm::vec3 _vPosition, glm::vec3 _vDirection, glm::vec3 _vColorStart, glm::vec3 _vColorEnd, float _fSpeed, float _fSpread, bool _bGravity, const char* filename, const char* filenameAlphaChannel, Camera* _pCamera);
	~ParticleEmitter();

	Particle* CreateParticle();
	void InitParticle(Particle* _particle);
	void Update(float dt);
	void Draw();
	// Insert your sort function here
	
	int Partition(int left, int right);

	void quickSort(int left, int right);

	float GetRandFloat();

};

#endif