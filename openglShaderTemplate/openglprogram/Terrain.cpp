
#include "Terrain.h"

Terrain::Terrain(const char* filename, float height, const char* texturename)
{
	Image* image = loadBMP(filename);

    mnWidth = image->width;
	mnLength = image->height;
        
	// Create new 2D array of floats to store heights
    mArrfHeight = new float*[mnLength];
    for(int i = 0; i < mnLength; i++) {
        mArrfHeight[i] = new float[mnWidth];
    }
     
	// Create new 2D array of vec3 to store normals
    mArrVecNormals = new glm::vec3*[mnLength];
    for(int i = 0; i < mnLength; i++) {
        mArrVecNormals[i] = new glm::vec3[mnWidth];
    }
       
	// Create new 2D array of vec3 to store normals
    mArrfColor = new glm::vec3*[mnLength];
    for(int i = 0; i < mnLength; i++) {
        mArrfColor[i] = new glm::vec3[mnWidth];
    }

	// Store the heights from the image height map
	for(int y = 0; y < mnLength; y++) {
        for(int x = 0; x < mnWidth; x++) {
            unsigned char color = (unsigned char)image->pixels[3 * (y * mnWidth + x)];
            float h = height * ((color / 255.0f) - 0.5f);
            SetHeightAt(x, y, h);
        }
    }

	delete image;
    
	if (texturename!=0)
	{
		mImageTerrain = loadBMP(texturename);
		mnTextureIdTerrain = loadTexture(mImageTerrain);
		if (mImageTerrain) delete mImageTerrain;
		
	}
	else 
		mnTextureIdTerrain = 0;

	ComputeNormals();

	computeHeatMap();
}

Terrain::Terrain(const char* filename, float height)
{
	Image* image = loadBMP(filename);

    mnWidth = image->width;
	mnLength = image->height;
            
	// Create new 2D array of floats to store heights
    mArrfHeight = new float*[mnLength];
    for(int i = 0; i < mnLength; i++) {
        mArrfHeight[i] = new float[mnWidth];
    }
           
	// Create new 2D array of vec3 to store normals
    mArrVecNormals = new glm::vec3*[mnLength];
    for(int i = 0; i < mnLength; i++) {
        mArrVecNormals[i] = new glm::vec3[mnWidth];
    }
    
	// Create new 2D array of vec3 to store normals
    mArrfColor = new glm::vec3*[mnLength];
    for(int i = 0; i < mnLength; i++) {
        mArrfColor[i] = new glm::vec3[mnWidth];
    }

	// Store the heights from the image height map
	for(int y = 0; y < mnLength; y++) 
	{
        for(int x = 0; x < mnWidth; x++) 
		{
            unsigned char color = (unsigned char)image->pixels[3 * (y * mnWidth + x)];
            float h = height * ((color / 255.0f) - 0.5f);

            SetHeightAt(x, y, h);
        }
    }


	delete image;
    
	mnTextureIdTerrain = 0;

	ComputeNormals();

	computeHeatMap();
}

Terrain::~Terrain()
{
	for(int i = 0; i < mnLength; i++) 
	{
        delete[] mArrfHeight[i];
    }
    delete[] mArrfHeight;
            
    for(int i = 0; i < mnLength; i++) 
	{
        delete[] mArrVecNormals[i];
    }
    delete[] mArrVecNormals;
}

void Terrain::computeHeatMap()
{

			glm::vec3 Color(0.0f, 0.0f, 0.0f);

			for(int i = 0; i< mnLength - 1; i++)
			{

				for(int j = 0; j< mnWidth; j++)
				{
					float height = mArrfHeight[j][i] + 45;

				//0.0 to 0.125
				if(height/90 >= 0.0f && height/90 <= 0.125f)
				{
					float red = 0.0f;
					float green = 0.0f;
					float blue = ( ( ( height/90 - 0.0f ) / 0.125f ) * 0.5f )+ 0.5f;
					Color = glm::vec3 (red, green, blue);

					
				}

				//0.125 to 0.375
				else if(height/90 > 0.125f && height/90 <= 0.375f)
				{
					float red = 0.0f;
					float green =   ( height/90 - 0.125f ) / 0.25f ;
					float blue = 1.0f;
					Color = glm::vec3 (red, green, blue);

				}				

				//0.375 to 0.625
				else if(height/90 >= 0.375f && height/90 <= 0.625f)
				{
					float red = ( height/90 - 0.375f )  / 0.25f ;
					float green = 1.0f;
					float blue = 1.0f - ( ( height/90 - 0.375f )  / 0.25f ) ;
					Color = glm::vec3 (red, green, blue);

					
				}
				
				//0.625 to 0.875
				else if(height/90 >= 0.625f && height/90 <= 0.875f)
				{
					float red = 1.0f;
					float green =  1.0f - ( ( height/90 - 0.625f )  / 0.25f );
					float blue = 0.0f;

					Color = glm::vec3 (red, green, blue);

					

				}
				//0.875 to 1.0
				else
				{
					float red =( (1-((height/90)-0.875)/(1.0f-0.875f) ) * 1.0f) + 0.5f;
					float green = 0.0f;
					float blue = 0.0f;

					Color = glm::vec3 (red, green, blue);

					
				}
					mArrfColor[i][j] = Color;
				}
			}
}

void Terrain::Draw(bool _bDrawNormals)
{
	// Compute the scaling factor needed to draw the terrain at its requied size
    float fScale = TERRAIN_SIZE / std::max(mnWidth - 1, mnLength - 1);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPushMatrix();

		// Appply scale
		glScalef(fScale, fScale, fScale);

		// Move to centre
		glTranslatef(-float(mnWidth) / 2, 0.0f, -float(mnLength) / 2);

		// White color for all vertices!
		glColor3f(1.0f, 1.0f, 1.0f);

		// Enable Texturing
		glEnable(GL_TEXTURE_2D);

		// Select the crrate side texture
		glBindTexture(GL_TEXTURE_2D, mnTextureIdTerrain);

		// Set the far-away filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// Set the near filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		float red;
		float green;
		float blue;

		
		float red2;
		float green2;
		float blue2;

		// For each row (along length/height)
			for(int i = 0; i < mnLength - 1; i++)
		{

			// Draw each row as a triangle strip
			glBegin(GL_TRIANGLE_STRIP);

			// For each column (along width)
				for(int j = 0; j < mnWidth; j++)

			{
				
				

				glTexCoord2f((float)j/mnWidth , (float)i/mnLength);
				glNormal3f( GetNormalAt(j, i).x ,GetNormalAt(j,i).y , GetNormalAt(j,i).z );
				// Draw first vertex
				glColor3f(mArrfColor[j][i].x, mArrfColor[j][i].y, mArrfColor[j][i].z);

				glVertex3f(j, GetHeightAt(j, i), i);

				

				glTexCoord2f((float)j/mnWidth , (float)(i + 1)/mnLength);
				glNormal3f( GetNormalAt(j, i + 1).x ,GetNormalAt(j,i + 1).y , GetNormalAt(j,i + 1).z );
				// Draw second vertex
				glColor3f(mArrfColor[j][i+1].x, mArrfColor[j][i+1].y, mArrfColor[j][i+1].z);
				glVertex3f(j , GetHeightAt(j, i + 1), i + 1);
			}

			glEnd();
		
		}

	glPopMatrix();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (_bDrawNormals) DrawNormals();
}

void Terrain::DrawNormals()
{
	// Compute the scaling factor needed to draw the terrain at its size
    float fScale = TERRAIN_SIZE / std::max(mnWidth - 1, mnLength - 1);
	
	glPushMatrix();

		// Appply scale
		glScalef(fScale, fScale, fScale);

		// Move to centre
		glTranslatef(-float(mnWidth) / 2, 0.0f, -float(mnLength) / 2);

		// RED!
		glColor3f(1.0f, 0.0f, 0.0f);

		// Draw each row as a triangle strip
		glBegin(GL_LINES);

			// For each row
			for(int z = 0; z < mnLength; z++) {

				// Draw the 2 vertices
				for(int x = 0; x < mnWidth; x++) {

					// First one
					glVertex3f(x, GetHeightAt(x, z), z);

					// Second one
					glm::vec3 vNormal = GetNormalAt(x, z);
					vNormal *= 1.2f; // Scale it down!!!
					glVertex3f(x + vNormal.x, GetHeightAt(x, z) + vNormal.y, z + vNormal.z);
				}
			}

		glEnd();

	glPopMatrix();
}

void Terrain::ComputeNormals()
{
		 glm::vec3** tempNormals = new glm::vec3*[mnLength];
		 for(int i = 0; i < mnLength; i++)
		 {
			 tempNormals[i] = new glm::vec3[mnWidth];
		 }

	// Computes the normals, if they haven't been computed yet
    if (mbComputedNormals) {
        return;
    }

	// For each row
    for(int z = 0; z < mnLength; z++) {

		
		

	  // For each column
        for(int x = 0; x < mnWidth; x++) {
		// Create new vector sum of size 0
        glm::vec3 sum(0.0f, 0.0f, 0.0f);

		// Create backward vector
		glm::vec3 backward;
        if (z > 0) {
            backward = glm::vec3(0.0f, mArrfHeight[z - 1][x] - mArrfHeight[z][x], -1.0f);
        }

		// Create forward vector
		glm::vec3 forward;
		if(z < mnLength - 1) {
			forward = glm::vec3(0.0f, mArrfHeight[z + 1][x] - mArrfHeight[z][x], 1.0f);
		}

		// Create leftward vector
		glm::vec3 leftward;
		if(x > 0)
		{
			leftward = glm::vec3(-1.0f , mArrfHeight[z][x - 1] - mArrfHeight[z][x] , 0.0f);
		}
		// Create rightward vector
		glm::vec3 rightward;
		if(x < mnWidth - 1)
		{
			rightward = glm::vec3(1.0f, mArrfHeight[z][x + 1] - mArrfHeight[z][x] , 0.0f);
		}
                   
		// Find normal of top-left triangle and add to sum (cross)
		if(z > 0 &&  x > 0)
		{
		sum += glm::cross( backward, leftward);
		}
		// Find normal of bottom-left triangle and add to sum (cross)
		if(z < mnLength - 1 && x > 0)
		{
		sum += glm::cross(leftward, forward);
		}
		// Find normal of bottom-right triangle and add to sum (cross)            
		if( x > 0 && z < mnLength - 1 )
		{
		sum += glm::cross(forward, rightward);
		}
		// Find normal of top-right triangle and add to sum (cross)
		if( x < mnWidth - 1 && z > 0)
		{
		 sum += glm::cross(rightward, backward);
		}
		// Just in case it's a zero-sum(add everything and the result is 0)
		// If sum is 0 in magnitude, set normal to point in Y-axis dir
		
		if(glm::length(sum) == 0)
		 {
			 sum.x = 0.0f;
			 sum.y = 1.0f;
			 sum.z = 0.0f;
		 }
		else
		{
			sum = glm::normalize(sum);
		}
		// Store the vector in the array
		
		
		tempNormals[z][x] = sum;
		mArrVecNormals[z][x] = tempNormals[z][x];

        }
    }

    // SMOOTHING BEGINS BELOW THIS LINE       

	const float FALLOUT_RATIO = 0.9f;

	// For each row
    for(int z = 0; z < mnLength; z++) {

		// For each column
		for(int x = 0; x < mnWidth; x++) {
			// Add myself first

			glm::vec3 sum = tempNormals[z][x];
                    
			// Add ratio (half) of my left neighbour's normal
			if( x > 0 )
			{
				sum += tempNormals[z][x - 1] * FALLOUT_RATIO;
			}
			// Add ratio (half) of my right neighbour's normal
			if( x > mnWidth - 1 )
			{
				sum += tempNormals[z][x + 1] * FALLOUT_RATIO;
			}
			// Add ratio (half) of my back neighbour's normal
			if( z > 0 )
			{
				sum += tempNormals[z - 1][x] * FALLOUT_RATIO;
			}
			// Add ratio (half) of my front neighbour's normal
               if( z < mnWidth - 1 )
			{
				sum += tempNormals[z + 1][x] * FALLOUT_RATIO;
			} 
			// Just in case it's a zero-sum (u add everything and the result is 0)
			   if(glm::length(sum) == 0)
			{
				sum.x = 0.0f;
				sum.y = 1.0f;
				sum.z = 0.0f;
			}
			   else
			   {
				   sum = glm::normalize(sum);
			   }
			// Set actual normal inside mArrVecNormals
			mArrVecNormals[z][x] = sum;
        }
    }
}

glm::vec3 Terrain::GetNormalAt(int _x, int _z)
{
	return mArrVecNormals[_z][_x];
}

int Terrain::GetLength()
{
	return mnLength;
}

int Terrain::GetWidth()
{
	return mnWidth;
}

float Terrain::GetHeightAt(int _x, int _z)
{
	return mArrfHeight[_z][_x];
}

void Terrain::SetHeightAt(int _x, int _z, float _newHeight)
{
	mArrfHeight[_z][_x] = _newHeight;
    mbComputedNormals = false;
}