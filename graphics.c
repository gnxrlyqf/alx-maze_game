#include "main.h"
#include <SDL2/SDL.h>

void render(SDL_Renderer *map, SDL_Renderer *display, column *walls, sprite *sprites, coords res)
{
	int i, j, texturex, si = 0, col;
	SDL_Rect wall, floor = {0, 360, 1260, 360}, sky = {0, 0, 1260, 360};

	rgba fog = {216, 217, 218, 0}, **texture = init_texture("bricks.png", 0);
	column curr;
	sprite scurr;

	SDL_SetRenderDrawColor(display, 216, 217, 218, 0);
	SDL_RenderClear(display);
	SDL_SetRenderDrawColor(display, 0, 153, 0, 255);
	SDL_RenderFillRect(display, &floor);
	for (i = 0; i < 1260; i++)
	{
		wall.x = walls[i].pos.x;
		curr = walls[i];
		scurr = sprites[si];
		for (j = 0; j < 32; j++)
		{
			wall.y = ((res.y - curr.line * 32) / 2) + (curr.line * j);
			wall.w = 1, wall.h = curr.line + 1;
			SDL_SetRenderDrawColor(display,
				(texture[curr.x][j].r * curr.s) * (1 - curr.f) + (curr.f * fog.r),
				(texture[curr.x][j].g * curr.s) * (1 - curr.f) + (curr.f * fog.g),
				(texture[curr.x][j].b * curr.s) * (1 - curr.f) + (curr.f * fog.b),
				255
			);
			SDL_RenderFillRect(display, &wall);
		}
		if (scurr.dist < curr.dist)
		{
			SDL_SetRenderDrawColor(display, 255, 255, 255, 0);
			SDL_RenderCopy(display, sprites[si].texture, NULL, &sprites[si].rect);
			SDL_RenderDrawRect(display, &sprites[si].rect);
		}
	}
	SDL_RenderPresent(display);
}

void draw_player(SDL_Renderer *renderer, cell **grid, coordsf pos)
{
	SDL_Rect player = {pos.x - 4, pos.y - 4, 8, 8};

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &player);
}

void draw_map(SDL_Renderer *renderer, cell **grid, coords dimensions, int cellSize, coordsf pos, ray *rays)
{
	int i, j, size = grid[0][0].size;
	SDL_Rect player = {pos.x - 4, pos.y - 4, 8, 8};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	for (i = 0; i < dimensions.x / cellSize; i++)
		for (j = 0; j < dimensions.y / cellSize; j++)
		{
			SDL_Rect cell = {i * size, j * size, size, size};
			if (grid[i][j].state == 1)
				SDL_RenderFillRect(renderer, &cell);
		}
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &player);
	for (i = 0; i < 1260; i++)
		SDL_RenderDrawLine(renderer, pos.x, pos.y, rays[i].pos.x, rays[i].pos.y);
	SDL_RenderPresent(renderer);
}
