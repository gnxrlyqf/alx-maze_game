#include "main.h"
#include <SDL2/SDL.h>

/**
 * render - handles rendering of game elements
 * @display: renderer on which to draw elements
 * @walls: array of vertical lines forming walls
 * @sprites: array of sprites to be drawn
 * @res: screen resolution
*/
void render(SDL_Renderer *display, column *walls, sprite *sprites, coords res)
{
	int i, j, texturex, si = 0, col;
	SDL_Rect wall, floor = {0, 360, 1260, 360}, sky = {0, 0, 1260, 360};

	rgba fog = {216, 217, 218, 0}, **texture = init_texture("bricks.png");
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
			SDL_RenderCopy(display, sprites[si].t, NULL, &sprites[si].rect);
			SDL_RenderDrawRect(display, &sprites[si].rect);
		}
	}
	SDL_RenderPresent(display);
}

/**
 * draw_map - displays a top down view of the world map
 * @m: renderer to display on
 * @g: two dimensional array representing grid
 * @d: world dimensions
 * @s: cell size
 * @p: player position
 * @r: array of rays
*/
void draw_map(SDL_Renderer *m, cell **g, coords d, int s, coordsf p, ray *r)
{
	int i, j;
	SDL_Rect player = {p.x - 4, p.y - 4, 8, 8}, cell;

	SDL_SetRenderDrawColor(m, 255, 255, 255, 255);
	SDL_RenderClear(m);
	SDL_SetRenderDrawColor(m, 0, 0, 0, 0);
	for (i = 0; i < d.x / s; i++)
		for (j = 0; j < d.y / s; j++)
		{
			cell.x = i * s, cell.y = j * s;
			cell.w = cell.h = s;
			if (g[i][j].state == 1)
				SDL_RenderFillRect(m, &cell);
		}
	SDL_SetRenderDrawColor(m, 255, 0, 0, 255);
	SDL_RenderFillRect(m, &player);
	for (i = 0; i < 1260; i++)
		SDL_RenderDrawLine(m, p.x, p.y, r[i].pos.x, r[i].pos.y);
	SDL_RenderPresent(m);
}
