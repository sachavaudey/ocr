#include "rotation.h"

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

/*	Function : getXFromPixelNb
 *
 *	--------------------------
 *
 *	Computes X coordinate from pixel number of 
 *	an image of given width
 *
 *	x		: pixel number in image pixels' array
 *
 *	returns	: coordinate on X axis (from LEFT to RIGHT) 
 */
int getXFromPixelNb(int x, unsigned int width)
{
	return x % width - (width / 2);
}

 /*	Function : getYFromPixelNb
 *
 *	--------------------------
 *
 *	Computes Y coordinate from pixel number of 
 *	an image of given width and height
 *
 *	x		: pixel number in image pixels' array
 *
 *	returns	: coordinate on Y axis (from TOP to BOTTOM) 
 */

int getYFromPixelNb(int x, unsigned int width, unsigned int height)
{
	return x / width - (height / 2);
}

/* X axis from left to right
 * Y axis from top to bottom 
 *  */
int getPixelNbFromXY(int x, int y, unsigned int width, unsigned int height)
{
	return ((y + height/2) * width) + (x + width/2);
}
// Function converting a given surface to grey scale
void rotate(SDL_Surface *surface, int angle) {
    printf("---------------\nStarting rotation...\n");
	double newAngle = (double)angle / 180 * M_PI;
	
	SDL_LockSurface(surface);
	printf("Radian angle: %f\n", newAngle);
	
	unsigned int initW = surface->w,	initH = surface->h;
	unsigned int newW = initH * absf(sin(newAngle)) + initW * absf(cos(newAngle));
	unsigned int newH = initH + absf(cos(newAngle)) + initW * absf(sin(newAngle));
	
	printf("Initial width : %i, initial height : %i\n", initW, initH);
	printf("New width : %i, new height : %i\n", newW, newH);

	SDL_Surface* newOne = SDL_CreateRGBSurface(0,newW,newH,32,0,0,0,0);

	Uint32* pixelDepart = surface->pixels;
	unsigned long initCount = initH * initW;

	Uint32* pixelPtr = newOne->pixels;
	unsigned long pixelCount = newW * newH;
	
	newAngle = newAngle;

	for(unsigned long pixelNb=0; pixelNb < pixelCount; pixelNb++)
	{	
		int x_depart = getXFromPixelNb(pixelNb, newW)*cos(newAngle) + getYFromPixelNb(pixelNb, newW, newH) * sin(newAngle);
        int y_depart = -getXFromPixelNb(pixelNb, newW)*sin(newAngle) + getYFromPixelNb(pixelNb, newW, newH) * cos(newAngle);
	
		unsigned long pixelDepartNb = getPixelNbFromXY(x_depart, y_depart, initW, initH);  
		
		if (abs(x_depart) <= (int)(initW)/2 &&
			abs(y_depart) <= (int)initH/2 &&
			pixelDepartNb < initCount)
		{	
			pixelPtr[pixelNb] = pixelDepart[pixelDepartNb];
		}
		else 
		{
			//printf("X( %i ) > initW ( %i ) or Y( %i ) > initH( %i )\n", abs(x_depart), initW, abs(y_depart), initH);
			pixelPtr[pixelNb] = SDL_MapRGBA(newOne->format, 0, 0, 0, 0);
		}
	}
	*surface = *newOne;
	SDL_UnlockSurface(surface);

	printf("Image successfully rotated !\n---------------\n");
}


