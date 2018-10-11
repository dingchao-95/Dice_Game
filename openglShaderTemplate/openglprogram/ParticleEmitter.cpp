
#include "ParticleEmitter.h"


ParticleEmitter::ParticleEmitter(glm::vec3 _vPosition, 
								 glm::vec3 _vDirection, 
								 glm::vec3 _vColorStart, 
								 glm::vec3 _vColorEnd, 
								 float _fSpeed,
								 float _fSpread, 
								 bool _bGravity,
								 const char* filename, 
								 const char* filenameAlphaChannel,
								 Camera* _pCamera) 
{
		mvPosition = _vPosition;
		mvDirection = _vDirection;
		mvColorStart = _vColorStart;
		mvColorEnd = _vColorEnd;
		mfSpeed = _fSpeed;
		mfSpread = _fSpread;
		mbGravity = _bGravity;
		mpCamera = _pCamera;

		// Load the images
		Image* image = loadBMP(filename);
		Image* imageAlphaChannel = loadBMP(filenameAlphaChannel);
		mnTextureParticleId = loadAlphaTexture(image, imageAlphaChannel);

		// Create the particles
    		for(int i = 0; i < NUM_PARTICLES; i++) {
			mArrParticles[i] = CreateParticle();
    		}

}

ParticleEmitter::~ParticleEmitter()
{
	for (int i=0; i<NUM_PARTICLES; i++)
		delete mArrParticles[i];
}

// Create a new particle
Particle* ParticleEmitter::CreateParticle() 
{
	Particle* newParticle = new Particle();	

	InitParticle(newParticle);
	newParticle->mfTimeAlive = GetRandFloat()+1;

	return newParticle;
}

// Reset a particle
void ParticleEmitter::InitParticle(Particle* _particle) 
{
	    _particle->mvPosition = mvPosition; 

		//mfSpread = mfSpread * _particle->mfTimeAlive;

		_particle->mvVelocity = mvDirection * mfSpeed;
		_particle->mvVelocity += glm::vec3(mfSpread * GetRandFloat() - mfSpread/2.0f,
										   mfSpread * GetRandFloat() - mfSpread/2.0f,
										   mfSpread * GetRandFloat() - mfSpread/2.0f);

		_particle->mvColorStart = mvColorStart;
		_particle->mvColorEnd = mvColorEnd;

		_particle->mfLifespan = GetRandFloat() + 1 ;
		_particle->mfTimeAlive = 0.0f;

}

void ParticleEmitter::Update(float dt) 
{
	glm::vec3 camPos = mpCamera->GetPosition();

	for(int i = 0; i < NUM_PARTICLES; i++) {

        Particle* pCurrParticle = mArrParticles[i];
                
	  // Remember our beloved GMAPS? XD
        pCurrParticle->mvPosition += pCurrParticle->mvVelocity * dt;
        if (mbGravity) pCurrParticle->mvVelocity += glm::vec3(0.0f, -GRAVITY * dt, 0.0f);

	  // Compute distance from camera
		pCurrParticle->mfDistanceFromCamera = glm::length(camPos - pCurrParticle->mvPosition);

	  // Update the particle's alive time based on elapsed time (dt)
		pCurrParticle->mfTimeAlive += dt;

		// If particle is dead, reset it
		if (pCurrParticle->mfTimeAlive >= pCurrParticle->mfLifespan) {

			InitParticle(pCurrParticle);

        }


    }

}

//Draws the particle fountain.
void ParticleEmitter::Draw() 
{
		// Enable alpha blending
		glEnable(GL_BLEND); 
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	
		// Enable textures
		glEnable(GL_TEXTURE_2D);
    	glBindTexture(GL_TEXTURE_2D, mnTextureParticleId);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		//Sort the particles
		quickSort(NUM_PARTICLES - NUM_PARTICLES , NUM_PARTICLES - 1);

		// Draw all the particles
		glBegin(GL_QUADS);
    	for(int i = 0; i < NUM_PARTICLES; i++) {
	
		Particle* pParticle = mArrParticles[i];

		float aliveRatio = pParticle->mfTimeAlive / pParticle->mfLifespan;

		glm::vec3 currColor = (1.0f - aliveRatio) * pParticle->mvColorStart + (aliveRatio) * pParticle->mvColorEnd;

        glColor4f(currColor.r, currColor.g, currColor.b, // Color
				  (1.0f - aliveRatio)); // Life (Alpha)        
                
	  glm::vec3 posToDraw = pParticle->mvPosition;
	  float size = PARTICLE_SIZE / 2;
		   


	  //Towards camera vector
	  glm::vec3 vTowardsCamera = mpCamera->GetPosition() - pParticle->mvPosition;
	  vTowardsCamera = glm::normalize(vTowardsCamera);

	  glm::vec3 vRight;
	  glm::vec3 vUp;
	  glm::vec3 vParticleY = glm::vec3(0.0f, pParticle->mvPosition.y , 0.0f);

	  //Right vector
	  vRight = glm::cross(vParticleY , vTowardsCamera);
	  vRight = glm::normalize(vRight);


	  //Left vector
	  vUp = glm::cross(vTowardsCamera , vRight);
	  vUp = glm::normalize(vUp);

	  //Bottom Left vertex
	  glm::vec3 BottomLeft = pParticle->mvPosition - (vRight * size) - (vUp * size);

	  //Bottom Right Vertex
	  glm::vec3 BottomRight = pParticle->mvPosition + (vRight * size) - (vUp * size);

	  //Top Right Vertex
	  glm::vec3 TopRight = pParticle->mvPosition + (vRight * size) + (vUp * size);

	  //Top Left Vertex
	  glm::vec3 TopLeft = pParticle->mvPosition - (vRight * size) + (vUp * size);


		// Bottom Left
		glTexCoord2f(0, 0);
		//glNormal3f(BottomLeft.x, BottomLeft.y , BottomLeft.z);
        glVertex3f(BottomLeft.x, BottomLeft.y , BottomLeft.z);

		// Bottom Right
        glTexCoord2f(0, 1);
		//glNormal3f(BottomRight.x, BottomRight.y , BottomRight.z);
        glVertex3f(BottomRight.x, BottomRight.y , BottomRight.z);

		// Top Right
        glTexCoord2f(1, 1);
		//glNormal3f(TopRight.x, TopRight.y , TopRight.z);
        glVertex3f(TopRight.x, TopRight.y , TopRight.z);

		// Top Left
        glTexCoord2f(1, 0);
		//glNormal3f(TopLeft.x, TopLeft.y , TopLeft.z);
        glVertex3f(TopLeft.x, TopLeft.y , TopLeft.z);
	
      }
      glEnd();

	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_BLEND); 

}

// Insert your sort function here
int ParticleEmitter::Partition(int left , int right)
{
      Particle* tmp;
      Particle* pivot = mArrParticles[(right+left)/2];
 

      /* partition */
	  while(left < right)
	  {
		  while ( mArrParticles[left]->mfDistanceFromCamera > pivot->mfDistanceFromCamera )

				left++;

		  while ( mArrParticles[right]->mfDistanceFromCamera < pivot->mfDistanceFromCamera )

				right--;	

			if ( mArrParticles[left]->mfDistanceFromCamera == mArrParticles[right]->mfDistanceFromCamera )

				left++;

			else if ( left < right )
			{
				tmp = mArrParticles[left];
				mArrParticles[left] = mArrParticles[right];
				mArrParticles[right] = tmp;
			}
	  }
 
return right;

}

//Recursive function
void ParticleEmitter::quickSort(int left, int right)
{
    if ( left < right )
    {
        int j = Partition(left, right); 
		
        quickSort(left, j-1);
		
        quickSort(j+1, right);
		 
	}
}

float ParticleEmitter::GetRandFloat()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}








