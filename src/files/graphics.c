#include "main.h"
#include <time.h>
#include <SDL2/SDL.h>
#define PI 3.14159265358979
#define DEG 0.0174533
#define RAD 57.2957795

/**
 * render - handles rendering of game elements
 * @display: renderer on which to draw elements
 * @w: array of vertical lines forming walls
 * @s: array of sprites to be drawn
 * @c: sprite count
 * @texture: two dimensional array representing texture
*/
void render(SDL_Renderer *display, column *w, sprite *s, int c, rgba **texture)
{
	int i, j, si = 0;
	SDL_Rect wall, floor = {0, 360, 1260, 360};
	rgba fog = {216, 217, 218, 0};
	column curr;
	sprite scurr;

	SDL_SetRenderDrawColor(display, 216, 217, 218, 0);
	SDL_RenderClear(display);
	SDL_SetRenderDrawColor(display, 0, 153, 0, 255);
	SDL_RenderFillRect(display, &floor);
	for (i = 0; i < 1260; i++)
	{
		curr = w[i];
		wall.x = curr.pos.x;
		scurr = s[si];
		for (j = 0; j < 32; j++)
		{
			wall.y = ((720 - curr.line * 32) / 2) + (curr.line * j);
			wall.w = 1, wall.h = curr.line + 1;
			SDL_SetRenderDrawColor(display,
				(texture[curr.x][j].r * curr.s) * (1 - curr.f) + (curr.f * fog.r),
				(texture[curr.x][j].g * curr.s) * (1 - curr.f) + (curr.f * fog.g),
				(texture[curr.x][j].b * curr.s) * (1 - curr.f) + (curr.f * fog.b),
				255
			);
			SDL_RenderFillRect(display, &wall);
		}
		if (scurr.dist < curr.dist && scurr.visible == 1)
		{
			SDL_SetRenderDrawColor(display, 255, 255, 255, 0);
			SDL_RenderCopy(display, s[si].t, NULL, &s[si].rect);
			SDL_RenderDrawRect(display, &s[si].rect);
		}
		else if (si < c - 1)
			si++;
	}
}

/**
 * draw_map - displays a top down view of the world map
 * @m: renderer to display on
 * @g: two dimensional array representing grid
 * @d: world dimensions
 * @s: cell size
 * @p: player position
 * @r: array of rays
 * @c: entity count
 * @entities: array of entities
*/
void draw_map(SDL_Renderer *m, cell **g, coords d, int s, coordsf p, ray *r,
int c, entity *entities) {
	int i, j;
	SDL_Rect player = {p.x - 4, p.y - 4, 8, 8}, cell, e;

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
	for (int i = 0; i < c; i++)
	{
		e.x = entities[i].pos.x - 4;
		e.y = entities[i].pos.y - 4;
		e.w = e.h = 8;
		SDL_SetRenderDrawColor(m, 0, 0, 255, 0);
		if (entities[i].exists == 1)
			SDL_RenderFillRect(m, &e);
	}
	SDL_RenderPresent(m);
}

int cards_events(SDL_Renderer *d, player p, SDL_Texture **cards,
int t1, int *t, SDL_Texture **card) {
	if (time(NULL) < t1 + 3)
		SDL_RenderCopy(d, *card, NULL, NULL);
	if (p.pos.x > 16 && p.pos.x < 64 && p.pos.y > 16 && p.pos.y < 64)
	{
		(*card) = cards[OBJECTIVE];
		SDL_RenderCopy(d, *card, NULL, NULL);
	}
	if (p.pos.x > 456 && p.pos.x < 520 && p.pos.y > 520 && p.pos.y < 584)
	{
		if (p.keys < 20)
		{
			(*card) = cards[NEED];
			SDL_RenderCopy(d, *card, NULL, NULL);
		}
		else if (p.keys == 20)
		{
			(*card) = cards[COMPLETE];
			SDL_RenderCopy(d, *card, NULL, NULL);
			SDL_RenderPresent(d);
			SDL_Delay(4000);
			return (1);
		}
	}
	if (time(NULL) < (*t) + 3)
	{
		(*card) = cards[QUIT];
		SDL_RenderCopy(d, *card, NULL, NULL);
		if (events() == 1)
			return (1);
	}
	if (events() == 1)
		*t = time(NULL);
	return (0);
}
