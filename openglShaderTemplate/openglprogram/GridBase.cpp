#include "GridBase.h"

GridBase::GridBase(int _nGridSize, float _fCubeSize)
{
	mnGridSize = _nGridSize;
	mfCubeSize = _fCubeSize;
}

GridBase::~GridBase()
{

}

void GridBase::Draw()
{
	float offset = mnGridSize * mfCubeSize / 2.0f; // Half the grid size, so as to centre the grid at the origin

	for (int row=0; row<mnGridSize; row++)
	{
		for (int col=0; col<mnGridSize; col++)
		{
			float anchorX = mfCubeSize * row - offset;
			float anchorY = mfCubeSize * col - offset;

			glBegin(GL_QUADS);
				glNormal3f(0.0f, 1.0f, 0.0f);
				glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
				glVertex3f(anchorX, 0, anchorY);
				glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
				glVertex3f(anchorX, 0, anchorY+mfCubeSize);
				glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
				glVertex3f(anchorX+mfCubeSize, 0, anchorY+mfCubeSize);
				glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
				glVertex3f(anchorX+mfCubeSize, 0, anchorY);
			glEnd();
		}
	}

	// Draw bottom front
	float gap = 0.005f;
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(offset+gap, 0.0f, offset+gap);
		glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
		glVertex3f(-offset-gap, 0, offset+gap);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(-offset-gap, -mfCubeSize*1.2f, offset+gap);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(offset+gap, -mfCubeSize*1.2f, offset+gap);
	glEnd();

	// Draw bottom right
	glBegin(GL_QUADS);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(offset+gap, 0.0f, offset+gap);
		glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
		glVertex3f(offset+gap, -mfCubeSize*1.2f, offset+gap);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(offset+gap, -mfCubeSize*1.2f, -offset-gap);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(offset+gap, 0.0f, -offset-gap);
	glEnd();

	// Draw bottom back
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(offset+gap, -mfCubeSize*1.2f, -offset-gap);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(-offset-gap, -mfCubeSize*1.2f, -offset-gap);
		glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
		glVertex3f(-offset-gap, 0, -offset-gap);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(offset+gap, 0.0f, -offset-gap);
	glEnd();

	// Draw bottom left
	glBegin(GL_QUADS);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(-offset-gap, 0.0f, -offset-gap);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(-offset-gap, -mfCubeSize*1.2f, -offset-gap);		
		glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
		glVertex3f(-offset-gap, -mfCubeSize*1.2f, offset+gap);
		glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
		glVertex3f(-offset-gap, 0.0f, offset+gap);
	glEnd();
}
