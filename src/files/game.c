#include "main.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define PI 3.14159265358979
#define DEG 0.0174533
#define RAD 57.2957795

/**
 * game - main game loop
 * @m: map renderer
 * @d: display renderer
 * @dimensions: grid dimensions
*/
void game(SDL_Renderer *m, SDL_Renderer *d, vector2 dimensions)
{
	int size = 16, count = 20, i, t1, t2;
	player p = {{24, 24}, PI / 2, {0, 0}, 0};
	cell **grid = init_grid(dimensions, size);
	ray *rays = malloc(sizeof(ray) * 1260);
	column *walls = malloc(sizeof(column) * 1260);
	entity *entities = spawn_entities(grid);
	sprite *sprites = malloc(sizeof(sprite) * count);
	rgba **texture = init_texture("src/assets/bricks.png");
	SDL_Surface *key = IMG_Load("src/assets/key.png");
	SDL_Texture **cards = init_cards(d), **counter = init_counter(d, 20), *c;

	for (i = 0; i < count; i++)
		sprites[i].t = SDL_CreateTextureFromSurface(d, key);
	while (1)
	{
		controls(grid, &p), rays = raycast(size, dimensions, grid, rays, p);
		if (p.theta < 0)
			p.theta += 2 * PI;
		if (p.theta > 2 * PI)
			p.theta -= 2 * PI;
		process_rays(rays, &walls, size, p.theta);
		if (check_entities(&p, &entities, count) == 1)
		{
			if (p.keys < 20)
				c = cards[FOUND];
			if (p.keys == 20)
				c = cards[ALL];
			t1 = time(NULL);
		}
		sprites = process_sprites(entities, sprites, count, p);
		render(d, walls, sprites, count, texture);
		if (m)
			draw_map(m, grid, dimensions, size, p.pos, rays, count, entities);
		if (cards_events(d, p, cards, t1, &t2, &c) == 1)
			break;
		SDL_RenderCopy(d, counter[p.keys], NULL, NULL);
		SDL_RenderPresent(d);
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
	fvector2 check;
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

/**
 * title - displays title screen
 * @display: sdl renderer
 *
 * Return: gamestate (int)
*/
int title(SDL_Renderer *display)
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

/* I didn't know where to put this function */

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16 *)p;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

			case 4:
				return *(Uint32 *)p;
			break;

			default:
				return 0;
		}
}