#include "main.h"
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
void game(cell **grid, SDL_Renderer *m, SDL_Renderer *d, coords d1, coords d2)
{
	int size = 16, s_count;
	ray *rays = malloc(sizeof(ray) * 1260);
	player p = {{264, 48}, PI / 2, {0, 0}};
	column *walls;
	sprite *sprites;
	entity *entities = malloc(sizeof(entity) * 2);

	entities[0].pos.x = 15 * 16 + 8, entities[1].pos.x = 17 * 16 + 8;
	entities[0].pos.y = 5 * 16 + 8, entities[1].pos.y = 2 * 16 + 8;

	while (1)
	{
		controls(grid, &p);
		rays = raycast(size, d1, grid, rays, p);
		walls = process_rays(rays, size, p.theta);
		sprites = process_sprites(d, entities, s_count, p);
		render(d, walls, sprites, d2);
		draw_map(m, grid, d1, size, p.pos, rays);
		if (events(grid, size) == 1)
			return;
	}
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
		p->theta -= .025, p->d.x = cos(p->theta), p->d.y = sin(p->theta);
	if (keystate[SDL_SCANCODE_RIGHT])
		p->theta += .025, p->d.x = cos(p->theta), p->d.y = sin(p->theta);
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
