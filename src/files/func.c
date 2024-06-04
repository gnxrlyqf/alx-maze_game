#include "main.h"
#include <png.h>
#include <zlib.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

cell **init_grid(coords res, int cellSize)
{
	coords dim = {res.x / cellSize, res.y / cellSize};
	coords start = {15, 15};
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
 * events - polls mouse and keyboard events
 * Return: 1 (close window) 0 (no events)
*/
int events(void)
{
	SDL_Event event;
	SDL_Keycode key;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
					return (1);
				break;
			case SDL_KEYDOWN:
				key = event.key.keysym.sym;
				if (key == SDLK_ESCAPE)
					return (1);
				if (key == SDLK_SPACE)
					return (2);
				break;
		}
	}
	return (0);
}

/**
 * instantiate - creates window
 * @instance: SDL instance to create
 * @dim: window dimensions
 * @name: window name
 * @pos: window position on screen
 *
 * Return: 0 (Success)
*/
int instantiate(SDL_instance *instance, coords dim, char *name, coords pos)
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
	png_image image;
	png_bytep buff;
	rgba **texture = malloc(32 * sizeof(rgba *));

	memset(&image, 0, sizeof(image));
	image.version = PNG_IMAGE_VERSION;
	if (png_image_begin_read_from_file(&image, file) != 0)
	{
		image.format = PNG_FORMAT_RGBA;
		buff = malloc(PNG_IMAGE_SIZE(image));
		if (buff && png_image_finish_read(&image, NULL, buff, 0, NULL) != 0)
		{
			for (i = 0; i < 32; i++)
			{
				texture[i] = malloc(32 * sizeof(rgba));
				for (j = 0; j < 32; j++)
				{
					texture[i][j].r = buff[(i + (j * 32)) * 4];
					texture[i][j].g = buff[(i + (j * 32)) * 4 + 1];
					texture[i][j].b = buff[(i + (j * 32)) * 4 + 2];
					texture[i][j].a = buff[(i + (j * 32)) * 4 + 3];
				}
			}
		}
	}
	free(buff);
	return (texture);
}

/**
 * free_all - frees all allocated memory
 * @rays: array of rays
 * @walls: array of columns
 * @entities: array of entities
 * @sprites: array of sprites
 * @texture: two dimensional array of pixels
 * @grid: two dimensional array of cells representing grid
 * @counter: array of textures representing key counter
 * @cards: array of textures representing text cards
*/
void free_all(ray *rays, column *walls, entity *entities, sprite *sprites,
rgba **texture, cell **grid, SDL_Texture **counter, SDL_Texture **cards) {
	free(rays);
	free(walls);
	free(entities);
	free_sprites(sprites, 20);
	free_rgba(texture, 32);
	free_grid(grid, 33);
	free_texture(counter, 21);
	free_texture(cards, 4);
}
