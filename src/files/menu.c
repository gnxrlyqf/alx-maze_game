#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int menu(SDL_Renderer *display)
{
	SDL_Surface *surface = IMG_Load("src/assets/title.png");
	SDL_Texture *texture;
	int out = 0, event = 0;

	texture = SDL_CreateTextureFromSurface(display, surface);
	while (1)	
	{
		SDL_RenderCopy(display, texture, NULL, NULL);
		SDL_RenderPresent(display);
		event = events();
		if (event == 1)
		{
			out = 0;
			break;
		}
		if (event == 2)
		{
			out = 1;
			break;
		}
	}
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	return (out);
}