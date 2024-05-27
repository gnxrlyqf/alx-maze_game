#include "main.h"
#include <SDL2/SDL.h>

void render(SDL_Renderer *renderer, column *walls, sprite *sprites, coords res)
{
	int i, j, texturex;
	SDL_Rect wall;
	rgba fog = {216, 217, 218, 0}, **texture = init_texture("bricks.png", 0);
	column curr;

	for (i = 0; i < 1260; i++)
	{
		wall.x = i;
		curr = walls[i];
		for (j = 0; j < 32; j++)
		{
			wall.y = ((res.y - curr.line * 32) / 2) + (curr.line * j);
			wall.w = 1, wall.h = curr.line + 1;
			SDL_SetRenderDrawColor(renderer,
				(texture[curr.x][j].r * curr.s) * (1 - curr.f) + (curr.f * fog.r),
				(texture[curr.x][j].g * curr.s) * (1 - curr.f) + (curr.f * fog.g),
				(texture[curr.x][j].b * curr.s) * (1 - curr.f) + (curr.f * fog.b),
				255
			);
			SDL_RenderFillRect(renderer, &wall);
		}
	}
	for (i = 0; i < 2; i++)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderCopy(renderer, sprites[i].texture, NULL, &sprites[i].rect);
		SDL_RenderDrawRect(renderer, &sprites[i].rect);
	}
}