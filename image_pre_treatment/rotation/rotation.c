#include "rotation.h"
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#define NOT_IN_IMAGE -1

/* 
 * Absolute value of a double
 * @param double x
 * @return absolute value
 */
double absf(double x)
{
	if (x < 0)
		return -x;
	return x;
}


/*
 * 	Computes X coordinate from pixel number of 
 *	an image of given width
 *	@param x : pixel index in image's pixels array
 *  @param width : image's width
 *	@return	coordinate on X axis (from LEFT to RIGHT) 
 */
int getXFromPixelNb(int x, unsigned int width)
{
	return x % width - (width / 2);
}



/*	
 *	Computes Y coordinate from pixel number of 
 *	an image of given width and height
 *
 *	@param x : pixel number in image pixels' array
 *	@param width : image's width
 *	@param height : image's heigth
 *
 *	@return coordinate on Y axis (from TOP to BOTTOM) 
 */
int getYFromPixelNb(int x, unsigned int width, unsigned int height)
{
	return x / width - (height / 2);
}



/*
 *	Computes  coordinate from pixel number of 
 *	an image of given width and height.
 *	If coordinates are not in image, returns NOT_IN_IMAGE.
 *
 *	@param x : pixel's X axis cootdinate (from LEFT to RIGHT)
 *	@param y : pixel's Y axis coordinate (from TOP to BOTTOM)
 *	@param width : image's width
 *	@param height : image's heigth
 *
 *	@return pixel's index in image array, 
 *			or NOT_IN_IMAGE.
 */
long getPixelNbFromXY(int x, int y, unsigned int width, unsigned int height)
{	
	long retour_x = (x + width/2);
	if (retour_x < 0 || retour_x >= width)
		return NOT_IN_IMAGE;

	long retour_y = (y + height/2);
	if (retour_y < 0 || retour_y >= height)
		return NOT_IN_IMAGE;

	return (retour_y * width) + retour_x;
}


/*
 * Rotates a surface of <angle> degrees
 *
 *	@param surface : surface to rotate
 *	@param angle : angle (degrees) to rotate the surface with
 *
 *	@return void
 */
void rotate(SDL_Surface** surface_param, int angle) 
{   
	SDL_Surface* surface = *surface_param;
	if (LOG_LEVEL)
		printf("---------------\nStarting rotation...\n");
	
	double newAngle = (double)angle / 180 * M_PI;
	
	SDL_LockSurface(surface);

	if (LOG_LEVEL > 1)	{printf("Radian angle: %f\n", newAngle);}

	unsigned int initW = surface->w,	initH = surface->h;
	unsigned int newW = initH * absf(sin(newAngle)) +
		initW * absf(cos(newAngle));
	unsigned int newH = initH + absf(cos(newAngle)) +
		initH * absf(sin(newAngle));
	
	if (LOG_LEVEL > 1){
		printf("Initial width : %i, initial height : %i\n",
				initW, initH);
		printf("New width : %i, new height : %i\n",
				newW, newH);}
	
	Uint32* pixelDepart = surface->pixels;
	unsigned long pixelCount = newW * newH;

	SDL_Surface* new_surface = SDL_CreateRGBSurfaceWithFormat(0,
				newW, newH, 32,
				(surface->format)->format);

	Uint32* rotated_pixels = new_surface->pixels;

	for(unsigned long pixelNb=0; pixelNb < pixelCount; pixelNb++)
	{	
		int x_depart = 
			getXFromPixelNb(pixelNb, newW) *
			cos(newAngle) 
			+
			getYFromPixelNb(pixelNb, newW, newH) *
			sin(newAngle);
        int y_depart = 
			-getXFromPixelNb(pixelNb, newW)*
			sin(newAngle) 
			+
			getYFromPixelNb(pixelNb, newW, newH) *
			cos(newAngle);
	
		long pixelDepartNb = getPixelNbFromXY(x_depart, y_depart,
				initW, initH);
		
		if (pixelDepartNb == NOT_IN_IMAGE) 
		{
			rotated_pixels[pixelNb] = SDL_MapRGB(new_surface->format,
				0,0,0);
		}
		else
		{	
			rotated_pixels[pixelNb] = pixelDepart[pixelDepartNb];
		}
	}

	for (unsigned long line = 0; line<newH; line++)
	{
		Uint8* retour = RgbAverageLine(new_surface, line);
		Uint8 retour_int = *retour;
		free(retour);
		if (retour_int <= 3)
		{
			for(unsigned int i = 0; i<newW; i++)
			{
				rotated_pixels[i*line] = SDL_MapRGB(new_surface->format,
					0,0,0);
			}
		}
	}
	
	SDL_Surface* temp = *surface_param;
	*surface_param = new_surface;
	SDL_UnlockSurface(temp);
	SDL_FreeSurface(temp);
	
	if (LOG_LEVEL)
		printf("Image successfully rotated !\n---------------\n");
}
