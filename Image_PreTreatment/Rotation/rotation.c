#include "rotation.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


double absf(double x)
{
	if (x < 0)
		return -x;
	return x;
}

int pointInArr(int x, int y, int* rectangle)
{	
	return (x >= rectangle[0] && x <= rectangle[1] &&
			y >= rectangle[2] && y <= rectangle[3]);
}

int getXFromPixelNb(int x, int width)
{
	return x / width - (width / 2);
}

int getYFromPixelNb(int x, int width, int height)
{
	return x % width - (height / 2);
}

// Function converting a given surface to grey scale
void rotate(SDL_Surface *surface, int angle) {
    printf("---------------\n"
           "Starting rotation...\n");
	double newAngle = (double)angle / 180 * M_PI;
	
	SDL_LockSurface(surface);
	printf("Radian angle: %f\n", newAngle);

	unsigned int initW = surface->w,	initH = surface->h;
	unsigned int newW = initH * absf(sin(newAngle)) + initW * absf(cos(newAngle));
	unsigned int newH = initH + absf(cos(newAngle)) + initW * absf(sin(newAngle));
	
	printf("init width : %i, init height : %i\n", initW, initH);
	printf("new width : %i, new height : %i\n", newW, newH);

	SDL_Surface* newOne = SDL_CreateRGBSurface(0,newW,newH,32,0,0,0,0);

	Uint32* pixelDepart = surface->pixels;
	unsigned long count = initH * initW;

	Uint32* pixelPtr = newOne->pixels;
	unsigned long pixelCount = newW * newH;
	
	newAngle = -newAngle;

	for(unsigned long pixelNb=0; pixelNb < pixelCount; pixelNb++)
	{	
		int x_depart = getXFromPixelNb(pixelNb, newW)*cos(newAngle) + getYFromPixelNb(pixelNb, newW, newH) * sin(newAngle);
        int y_depart = -getXFromPixelNb(pixelNb, newW)*sin(newAngle) + getYFromPixelNb(pixelNb, newW, newH) * cos(newAngle);
	
		unsigned long pixelDepartNb = abs(((int)y_depart) + ((int)initH)/2) * initW + abs(((int)x_depart)+((int)initW/2)); 
		
		if (abs(x_depart) <= (int)(initW)/2 &&
			abs(y_depart) <= (int)initH/2 &&
			pixelDepartNb < count)
		{	
			pixelPtr[pixelNb] = pixelDepart[pixelDepartNb];
		}
		else 
		{
			//printf("X( %i ) > initW ( %i ) or Y( %i ) > initH( %i )\n", abs(x_depart), initW, abs(y_depart), initH);
			pixelPtr[pixelNb] = SDL_MapRGBA(newOne->format, 0, 0, 0, 1);
		}
	}

	//printf("x dep : %i, y dep: %i ", initW, initH);

	*surface = *newOne;
	SDL_UnlockSurface(surface);

	printf("image rotatedv2\n---------------\n");
}


