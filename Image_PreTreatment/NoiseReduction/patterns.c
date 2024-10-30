#include "patterns.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <strings.h>


/*	@Function : Patterns_ReplaceLosange
 *
 *	-----------------------------------
 *
 *	Function replacing white pixel pattern
 *	by black pixels, if we find a match >= to thresold 
 *	( thresold : double [0-1] ).
 *	Pattern is searched with index_to_check == 1st white pixel.
 *	Pattern searched here :
 *
 *	..... => .....
 *	..X.. => .....
 *	.X.X. => ..... 
 *	..X.. => .....
 *	..... => .....
 *	en partant du 1er X rencontre
 *
 *	@param surface			:	target surface
 *	@param index_to_check	:	index of starting pixel (1st X)
 *								in surface's pixel array.
 *	@param thrseold			:	if searched area 
 *								matches ([0-1]) >= thresold ([0-1]),
 *								white pixels become black
 *
 *	@return					: void
 *
 */ 
void Patterns_RepalceLosange(SDL_Surface* surface, long index_to_check, double thresold)
{
	long W = surface->w, H = surface->h;
	Uint32* pixels = surface->pixels;
	Uint8 color=0;
	double diff = 0;

	for (long line = -1; line < 4; line++)
	{
		for (long row = -2; row < 3; row++)
		{
			SDL_GetRGB(
					pixels[index_to_check + line * W + row],
					surface->format,
					&color, &color, &color);
			if (((row == 0 && (line == 0 || line == 2 )) || 
				(line == 1 && (row == -1 || row == 1))))
			{
				if (color == 0)
				{return;}
			}
			else if (color == 255)
				diff++;
		}
	}

	if ((25 - diff)/25 >= thresold)
	{
		if (LOG_LEVEL > 1)
			printf("Removed losange\n");
		pixels[index_to_check] = SDL_MapRGB(surface->format, 0, 0, 0);
		pixels[index_to_check + W - 1] = SDL_MapRGB(surface->format, 0, 0, 0);
		pixels[index_to_check + W + 1] = SDL_MapRGB(surface->format, 0, 0, 0);
		pixels[index_to_check + W * 2] = SDL_MapRGB(surface->format, 0, 0, 0);
	}
}

void Patterns_Line5(SDL_Surface* surface, long index_to_check, double thresold)
{
	long W = surface->w, H = surface->h;
	Uint32* pixels = surface->pixels;
	Uint8 color=0;
	double diff = 0;

	for (long line = 0; line < 1; line++)
	{
		for (long row = 0; row < 5; row++)
		{
			SDL_GetRGB(
					pixels[index_to_check + line * W + row],
					surface->format,
					&color, &color, &color);
			if (row == 0  || row == 4)
			{
				if (color == 0)
				{return;}
			}
			else if (color == 0)
				diff++;
		}
	}

	if ((5 - diff)/5 >= thresold)
	{
		pixels[index_to_check] = SDL_MapRGB(surface->format, 255, 255, 255);
		pixels[index_to_check+1] = SDL_MapRGB(surface->format, 255, 255, 255);
		pixels[index_to_check+2] = SDL_MapRGB(surface->format, 255, 255, 255);
		pixels[index_to_check+3] = SDL_MapRGB(surface->format, 255, 255, 255);
		pixels[index_to_check+4] = SDL_MapRGB(surface->format, 255, 255, 255);
	}
}

void Patterns_Row5(SDL_Surface* surface, long index_to_check, double thresold)
{
	long W = surface->w, H = surface->h;
	Uint32* pixels = surface->pixels;
	Uint8 color=0;
	double diff = 0;

	for (long line = 0; line < 5; line++)
	{
		for (long row = 0; row < 1; row++)
		{
			SDL_GetRGB(
					pixels[index_to_check + line * W + row],
					surface->format,
					&color, &color, &color);
			if (line == 0  || line == 4)
			{
				if (color == 0)
				{return;}
			}
			else if (color == 0)
				diff++;
		}
	}

	if ((5 - diff)/5 >= thresold)
	{
		pixels[index_to_check] = SDL_MapRGB(surface->format, 255, 255, 255);
		pixels[index_to_check+1*W] = SDL_MapRGB(surface->format, 255, 255, 255);
		pixels[index_to_check+2*W] = SDL_MapRGB(surface->format, 255, 255, 255);
		pixels[index_to_check+3*W] = SDL_MapRGB(surface->format, 255, 255, 255);
		pixels[index_to_check+4*W] = SDL_MapRGB(surface->format, 255, 255, 255);
	}
}

//si trop de trucs avec 1 seul voisin max => delete
