#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <time.h>
#define PI 3.14159265358979
#define DEG 0.0174533
#define RAD 57.2957795

/**
 * game - main game loop
 * @grid: two dimensional array of cells representing grid
 * @m: map renderer
 * @d: display renderer
 * @d1: grid dimensions
 * @d2: world dimensions
*/
void game(SDL_Renderer *m, SDL_Renderer *d, coords d1, coords d2)
{
	int size = 16, count = 20, i, t1, t2;
	player p = {{24, 24}, PI / 2, {0, 0}, 0};
	SDL_Rect e;
	cell **grid = init_grid(d1, size);
	ray *rays = malloc(sizeof(ray) * 1260);
	column *walls = malloc(sizeof(column) * 1260);
	entity *entities = spawn_entities(grid, p.pos);
	sprite *sprites = malloc(sizeof(sprite) * count);
	rgba **texture = init_texture("src/assets/bricks.png");
	SDL_Surface *key = IMG_Load("src/assets/key.png");
	SDL_Texture **cards = init_cards(d), **counter = init_counter(d, 20), *c;

	for (i = 0; i < count; i++)
		sprites[i].t = SDL_CreateTextureFromSurface(d, key);
	while (1)
	{
		// c = NULL;
		controls(grid, &p), rays = raycast(size, d1, grid, rays, p);
		process_rays(rays, &walls, size, p.theta);
		if (check_entities(&p, &entities, count) == 1)
		{
			if (p.keys < 20)
				c = cards[FOUND];
			if (p.keys == 20)
				c = cards[ALL];
			t1 = time(NULL);
		}
		sprites = process_sprites(d, entities, sprites, count, p);
		render(d, walls, sprites, count, texture);
		draw_map(m, grid, d1, size, p.pos, rays, count, entities);
		SDL_RenderCopy(d, counter[p.keys], NULL, NULL);
		if (time(NULL) < t1 + 3)
			SDL_RenderCopy(d, c, NULL, NULL);
		if (cards_events(d, p, cards, &t2, &c) == 1)
			break;
		SDL_RenderPresent(d), SDL_RenderPresent(m);
	}
	free_all(rays, walls, entities, sprites, texture, grid, counter, cards);
	SDL_FreeSurface(key);
}

/**
 * controls - handles keyboard polling for player controls
 * @grid: two dimensional array of cells representing grid
 * @p: player struct
*/
void controls(cell **grid, player *p)
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	coordsf check;
	float strafe = p->theta - 90 * DEG;

	if (keystate[SDL_SCANCODE_LEFT])
		p->theta -= .03, p->d.x = cos(p->theta), p->d.y = sin(p->theta);
	if (keystate[SDL_SCANCODE_RIGHT])
		p->theta += .03, p->d.x = cos(p->theta), p->d.y = sin(p->theta);
	if (keystate[SDL_SCANCODE_W])
	{
		check.x = p->pos.x + p->d.x * 2, check.y = p->pos.y + p->d.y * 2;
		if (grid[(int)(check.x / 16)][(int)(p->pos.y / 16)].state != 1)
			p->pos.x += p->d.x * .8;
		if (grid[(int)(p->pos.x / 16)][(int)(check.y / 16)].state != 1)
			p->pos.y += p->d.y * .8;
	}
	if (keystate[SDL_SCANCODE_S])
	{
		check.x = p->pos.x - p->d.x * 2, check.y = p->pos.y - p->d.y * 2;
		if (grid[(int)(check.x / 16)][(int)(p->pos.y / 16)].state != 1)
			p->pos.x -= p->d.x * .8;
		if (grid[(int)(p->pos.x / 16)][(int)(check.y / 16)].state != 1)
			p->pos.y -= p->d.y * .8;
	}
	if (keystate[SDL_SCANCODE_A])
	{
		check.x = p->pos.x + cos(strafe), check.y = p->pos.y + sin(strafe);
		if (grid[(int)(check.x / 16)][(int)(p->pos.y / 16)].state != 1)
			p->pos.x += cos(strafe) * .8;
		if (grid[(int)(p->pos.x / 16)][(int)(check.y / 16)].state != 1)
			p->pos.y += sin(strafe) * .8;
	}
	if (keystate[SDL_SCANCODE_D])
	{
		check.x = p->pos.x - cos(strafe), check.y = p->pos.y - sin(strafe);
		if (grid[(int)(check.x / 16)][(int)(p->pos.y / 16)].state != 1)
			p->pos.x -= cos(strafe) * .8;
		if (grid[(int)(p->pos.x / 16)][(int)(check.y / 16)].state != 1)
			p->pos.y -= sin(strafe) * .8;
	}
}
