#include "main.h"
#include <png.h>
#include <zlib.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

cell **init_grid(coords res, int cellSize)
{
	coords dim = {res.x / cellSize, res.y / cellSize};
	srand(time(NULL));
	coords start = {15, 15};
	cell **grid = malloc(dim.x * sizeof(cell *));
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

void draw_grid(SDL_instance instance, cell **grid, coords dimensions, int cellSize)
{
	SDL_SetRenderDrawColor(instance.renderer, 0, 0, 0, 0);
	int size = grid[0][0].size;
	for (int i = 0; i < dimensions.x / cellSize; i++)
		for (int j = 0; j < dimensions.y / cellSize; j++)
		{
			SDL_Rect cell = {i * size, j * size, size, size};
			if (grid[i][j].state == 1)
				SDL_RenderFillRect(instance.renderer, &cell);
		}
}

int events(cell **grid, int size)
{
	SDL_Event event;
	SDL_Keycode key;

	while(SDL_PollEvent(&event))
	{
		int mousePosX;
		int mousePosY;
		switch (event.type)
		{
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
					return (1);
			case SDL_MOUSEMOTION:
				mousePosX = event.button.x / size;
				mousePosY = event.button.y / size;
				if (event.button.button == SDL_BUTTON_LEFT)
					grid[mousePosX][mousePosY].state = 1;
				if (event.button.button == SDL_BUTTON_MIDDLE)
					grid[mousePosX][mousePosY].state = 0;
				break;
			case SDL_KEYDOWN:
				key = event.key.keysym.sym;
				if (key == SDLK_ESCAPE)
					return (1);
				break;
		}
	}
	return (0);
}

int instantiate(SDL_instance *instance, coords dimensions, char *name, coords pos)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Initialization failed: %s\n", SDL_GetError());
        exit(1);
    }
    instance->window = SDL_CreateWindow(name, pos.x, pos.y, dimensions.x, dimensions.y, 0);
    if(!instance->window)
    {
        printf("Window instantiation failed: %s\n", SDL_GetError());
        SDL_Quit();
        exit(2);
    }
    instance->renderer = SDL_CreateRenderer(instance->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!instance->renderer)
    {
        printf("Render failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(instance->window);
        SDL_Quit();
        exit(3);
    }
    return (0);
}

rgba **init_texture(char *file, int type)
{
	int i, j = 0;
	png_image image;
	png_bytep buffer;

	rgba **texture = malloc(32 * sizeof(rgba *));
	memset(&image, 0, sizeof image);
	image.version = PNG_IMAGE_VERSION;
	if (png_image_begin_read_from_file(&image, file) != 0)
	{
		image.format = PNG_FORMAT_RGBA;
		buffer = malloc(PNG_IMAGE_SIZE(image));
		if (buffer && png_image_finish_read(&image, NULL, buffer, 0, NULL) != 0)
		{
			for (i = 0; i < 32; i++)
			{
				texture[i] = malloc(32 * sizeof(rgba));
				for (j = 0; j < 32; j++)
				{
					texture[i][j].r = buffer[(i + (j * 32)) * 4];
					texture[i][j].g = buffer[(i + (j * 32)) * 4 + 1];
					texture[i][j].b = buffer[(i + (j * 32)) * 4 + 2];
					texture[i][j].a = buffer[(i + (j * 32)) * 4 + 3];
				}
			}
		}
	}
	return (texture);
}

void free_grid(cell **grid, int rows)
{
	for (int i = 0; i < rows; i++)
		free(grid[i]);
	free(grid);
}