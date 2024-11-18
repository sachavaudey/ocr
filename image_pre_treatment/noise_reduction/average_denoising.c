#include "average_denoising.h"
#include <stdio.h>


/*	Function : AverageDenoising
 *
 *	----------------------------------------------------
 *
 *	Denoise a surface (grayscaled or color)
 *	by mapping pixel's to average value of itslef
 *	and his neighbour, when above a certain thresold.
 *
 *	@param surface		:	target surface
 *	@param thresold		:	thresold above which pixel
 *							color is averagged, otherwise
 *							becomes black.
 *
 *	@return				: void 
 */
void PRT_AverageDenoising(SDL_Surface *surface)
{
	if (LOG_LEVEL >= 1)
		printf("%s\nStarting average denoising (gray) ...\n",
				LOG_SEPARATOR);
	int width = surface->w, height = surface->h;

	Uint8* threshold = RgbAverageSurface(surface);
	
	float coeff = 2;
	Uint8 threshold_r = (threshold[0] * coeff),
		  threshold_g = (threshold[1] * coeff),
		  threshold_b = (threshold[2] * coeff);


	if (threshold_r > 200 && threshold_g > 200 && threshold_b > 200)
	{
		free(threshold);
		return;
	}
	
	printf("thresold %i %i %i", threshold_r, threshold_g, threshold_b);
	free(threshold);

    Uint32 *pixels = (Uint32 *)(surface->pixels);
    Uint32 *new_pixels = malloc(width * height * sizeof(Uint32));
    
	if (new_pixels == 0) 
	{
        errx(EXIT_FAILURE, "Memory allocation failed\n");
        return;
    }

    for (int line = 1; line+1 < height; line++) 
	{
        for (int row = 1; row+1 < width; row++) 
		{
			Uint8* retour = RgbAveragePixelsAround(surface,
												line * width + row);                        

            Uint8 avg_r = retour[0];
            Uint8 avg_g = retour[1];
            Uint8 avg_b = retour[2];

			free(retour);
			
			Uint8 r,g,b;
			r = g = b = 0;
			SDL_GetRGB(pixels[line * width + row], surface->format,  &r, &g, &b);
            
			if ((avg_r >= threshold_r ||
				avg_g >= threshold_g ||
				avg_b >= threshold_b)
				&&
				(r>=threshold_r &&
				 g>=threshold_g &&
				 b>=threshold_b) )
			{
				new_pixels[line * width + row] = SDL_MapRGBA(surface->format,
						avg_r, avg_g, avg_b, 255);
			}
			else
				new_pixels[line * width + row] = SDL_MapRGBA(surface->format,
						0, 0, 0, 255);
        }
    }

    memcpy(pixels, new_pixels, width * height * sizeof(Uint32));
    free(new_pixels);

	printf("Average denoising successfully finished !\n%s\n",
			LOG_SEPARATOR);
}

/*	Function : AverageDenoisingBlackWhite
 *
 *	----------------------------------------------------
 *
 *	Denoise a surface (grayscaled or color)
 *	by mapping pixel's to average value of itslef
 *	and his neighbour, when above a certain thresold.
 *
 *	@param surface		:	target surface
 *	@param thresold		:	thresold above which pixel
 *							color is averagged, otherwise
 *							becomes black.
 *
 *	@return				: void 
 */
void PRT_AverageDenoisingBlackWhite(SDL_Surface *surface)
{
	pid_t parentId = getpid();
	pid_t pid = fork();

	if (pid == 0)
	{
		int status = 0;
		if (LOG_LEVEL >= 1)
		{
			printf("%s\nStarting average denoising...\n",
				LOG_SEPARATOR);
			waitpid(parentId, &status, 0);
 			printf("Average denoising successfully finished !\n%s\n",
					LOG_SEPARATOR);
		}
		_exit(EXIT_SUCCESS);
	}

	int width = surface->w, height = surface->h;
	
	Uint8* thresholdArr = RgbAverageSurface(surface);
	float coeff = 1.5;
	Uint8 threshold = (thresholdArr[0] * coeff);

	free(thresholdArr);

    Uint32 *pixels = (Uint32 *)(surface->pixels);
    Uint32 *new_pixels = malloc(width * height * sizeof(Uint32));
    if (new_pixels == 0) 
	{
        errx(EXIT_FAILURE, "Memory allocation failed\n");
        return;
    }

    for (int line = 1; line+1 < height; line++) 
	{
        for (int row = 1; row+1 < width; row++) 
		{
			Uint8* retour = RgbAveragePixelsAround(surface,
					line * width + row);
            Uint8 avg = retour[0];
			free(retour);

			Uint8 color=0;
			SDL_GetRGB(pixels[line * width + row], surface->format,
					&color, &color, &color);
            

            if (avg >= threshold && color >= threshold)
				new_pixels[line * width + row] = SDL_MapRGBA(surface->format,
						255, 255, 255, 255);
            else 
				new_pixels[line * width + row] = SDL_MapRGBA(surface->format,
						0, 0, 0, 255);
        }
    }

    memcpy(pixels, new_pixels, width * height * sizeof(Uint32));
    free(new_pixels);

}
