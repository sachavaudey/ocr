#include "rotation.h"

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

	unsigned int line_top = 0;
	while (line_top < newH)
	{
		Uint8* retour = RgbAverageLine(new_surface, line_top);
		Uint8 retour_int = *retour;
		free(retour);
		if (retour_int > 3)
		{
			break;
		}
		line_top++;
	}

	unsigned int line_bottom = 0;
	while (line_bottom < newH)
	{
		Uint8* retour = RgbAverageLine(new_surface, newH-line_bottom-1);
		Uint8 retour_int = *retour;
		free(retour);
		if (retour_int > 3)
		{
			break;
		}
		line_bottom++;
	}
	
	unsigned int col_left = 0;
	while (col_left < newW)
	{
		Uint8* retour = RgbAverageCol(new_surface, col_left);
		Uint8 retour_int = *retour;
		free(retour);
		if (retour_int > 3)
		{
			break;
		}
		col_left++;
	}

	unsigned int col_right = 0;
	while (col_right < newW)
	{
		Uint8* retour = RgbAverageCol(new_surface, newW -col_right-1);
		Uint8 retour_int = *retour;
		free(retour);
		if (retour_int > 3)
		{
			break;
		}
		col_right++;
	}

	long final_w = newW - col_left - col_right,
		 final_h = newH - line_top - line_bottom;

	SDL_Surface* trimmed_surface = SDL_CreateRGBSurfaceWithFormat(0,
				final_w,
				final_h, 32,
				(surface->format)->format);

	Uint32* trimmed_pixels = trimmed_surface->pixels;
	unsigned long trimmed_count = 
		(final_w) 
		* (final_h);

	for (unsigned long pixel_index = 0;
			pixel_index < trimmed_count;
			pixel_index++) 
	{
		trimmed_pixels[pixel_index] =
			rotated_pixels[
			col_left + (line_top * newW) +
			(newW * (pixel_index / final_w)) +
			(pixel_index % final_w)
			];
	}

	
	SDL_Surface* temp = *surface_param;
	*surface_param = trimmed_surface;
	SDL_UnlockSurface(temp);
	SDL_FreeSurface(temp);
	SDL_FreeSurface(new_surface);
	if (LOG_LEVEL)
		printf("Image successfully rotated !\n---------------\n");
}
