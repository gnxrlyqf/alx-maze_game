#include "main.h"
#include <stdlib.h>
#include <SDL2/SDL_image.h>

SDL_Texture **init_counter(SDL_Renderer *renderer, int count)
{
	int i;
	SDL_Texture **counter = malloc(sizeof(SDL_Texture *) * count);

	for (i = 0; i <= count; i++)
	{
		char *string, *countstr = malloc(sizeof(char) * 2);
		SDL_Surface *surface;

		SDL_itoa(i, countstr, 10);
		string = concatenate("src/assets/", countstr, ".png");
		surface = IMG_Load(string);
		counter[i] = SDL_CreateTextureFromSurface(renderer, surface);
		printf("%s\n", string);
		free(string), free(countstr);
		SDL_FreeSurface(surface);
	}
	return (counter);
}

SDL_Texture **init_cards(SDL_Renderer *r)
{
	SDL_Texture **cards = malloc(sizeof(SDL_Texture *) * 6);

	SDL_Surface *found = IMG_Load("src/assets/found.png");
	SDL_Surface *all = IMG_Load("src/assets/allkeys.png");
	SDL_Surface *need = IMG_Load("src/assets/needkeys.png");
	SDL_Surface *complete = IMG_Load("src/assets/complete.png");
	SDL_Surface *objective = IMG_Load("src/assets/objective.png");
	SDL_Surface *quit = IMG_Load("src/assets/quit.png");

	cards[FOUND] = SDL_CreateTextureFromSurface(r, found);
	cards[ALL] = SDL_CreateTextureFromSurface(r, all);
	cards[NEED] = SDL_CreateTextureFromSurface(r, need);
	cards[COMPLETE] = SDL_CreateTextureFromSurface(r, complete);
	cards[OBJECTIVE] = SDL_CreateTextureFromSurface(r, objective);
	cards[QUIT] = SDL_CreateTextureFromSurface(r, quit);

	SDL_FreeSurface(found);
	SDL_FreeSurface(all);
	SDL_FreeSurface(need);
	SDL_FreeSurface(complete);
	SDL_FreeSurface(objective);
	SDL_FreeSurface(quit);

	return(cards);
}