#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

cell **init_grid(vector2 res, int cellSize)
{
	vector2 dim = {res.x / cellSize, res.y / cellSize};
	vector2 start = {15, 15};
	cell **grid = malloc(dim.x * sizeof(cell *));

	srand(time(NULL));
	if (!grid)
		exit(98);
	for (int i = 0; i < dim.x; i++)
	{
		grid[i] = malloc(dim.y * sizeof(cell));
		if (!grid[i])
			exit(99);
		for (int j = 0; j < dim.y; j++)
		{
			grid[i][j].pos.x = i;
			grid[i][j].pos.y = j;
			grid[i][j].size = cellSize;
			grid[i][j].state = 1;
		}
	}
	grid[start.x][start.y].state = 0;
	maze(grid, start, dim);
	return (grid);
}

/**
 * init_window - creates window
 * @instance: SDL instance to create
 * @dim: window dimensions
 * @name: window name
 * @pos: window position on screen
 *
 * Return: 0 (Success)
*/
int init_window(SDL_instance *instance, vector2 dim, char *name, vector2 pos)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Initialization failed: %s\n", SDL_GetError());
		exit(1);
	}
	instance->window = SDL_CreateWindow(name, pos.x, pos.y, dim.x, dim.y, 0);
	if (!instance->window)
	{
		printf("Window instantiation failed: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	instance->renderer = SDL_CreateRenderer(
		instance->window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!instance->renderer)
	{
		printf("Render failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(instance->window);
		SDL_Quit();
		exit(3);
	}
	return (0);
	}

/**
 * init_texture - creates texture from local file
 * @file: file name
 *
 * Return: two dimensional array of pixels
*/
rgba **init_texture(char *file)
{
	int i, j = 0;
	rgba **texture = malloc(32 * sizeof(rgba *));
	SDL_Surface *surface = IMG_Load(file);
	Uint32 pixel;

	for (i = 0; i < 32; i++)
	{
		texture[i] = malloc(32 * sizeof(rgba));
		for (j = 0; j < 32; j++)
		{
			pixel = getpixel(surface, i, j);
			texture[i][j].r = pixel & 0xFF;
			texture[i][j].g = (pixel >> 8) & 0xFF;
			texture[i][j].b = (pixel >> 16) & 0xFF;
			texture[i][j].a = (pixel >> 24) & 0xFF;
		}
	}
	SDL_FreeSurface(surface);
	return (texture);
}

/**
 * init_counter - initializes key counter text cards
 * @renderer: pointer to renderer
 * @count: key count
 *
 * Return: array of textures
*/
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
		free(string), free(countstr);
		SDL_FreeSurface(surface);
	}
	return (counter);
}

/**
 * init_cards - initializes text cards
 * @r: pointer to renderer
 *
 * Return: array of textures
*/
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

	return (cards);
}
