#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#ifndef __linux__
#include <Windows.h>
#endif

#include <GL/gl.h>

class Image
{
public:
	Image(char *ps, int w, int h);
	~Image();

	// array of form R, G, B, R, G, B .... starting from the lower left pixel
	char *pixels;

	int width;
	int height;
};

char* addAlphaChannel(Image* image, Image* alphaChannel);
GLuint loadAlphaTexture(Image* image, Image* alphaChannel);

Image *loadBMP(const char *fileName);
GLuint loadTexture(Image *image);

#endif