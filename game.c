#include "main.h"
#define PI 3.14159265358979
#define DEG 0.0174533
#define RAD 57.2957795

void game(cell **grid, SDL_Renderer *map, SDL_Renderer *display, coords dim1, coords dim2)
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
		poll_controls(grid, &p);
		rays = raycast(map, size, dim1, grid, rays, p);
		walls = process_rays(rays, size, p.theta);
		sprites = process_sprites(display, entities, s_count, p);
		render(map, display, walls, sprites, dim2);
		draw_map(map, grid, dim1, size, p.pos, rays);
		if (events(grid, size) == 1)
			return;
	}

}

void poll_controls(cell **grid, player *p)
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	float strafe = p->theta - 90 * DEG;
	/* rotate player and calculate offset */
	if (keystate[SDL_SCANCODE_LEFT])
	{
		p->theta -= .025;
		p->d.x = cos(p->theta), p->d.y = sin(p->theta);
	}
		if (keystate[SDL_SCANCODE_RIGHT])
	{
		p->theta += .025;
		p->d.x = cos(p->theta), p->d.y = sin(p->theta);
	}
	/* Check for collisions, move player if none*/
	if (keystate[SDL_SCANCODE_W])
	{
		if (grid[(int)((p->pos.x + p->d.x * 2) / 16)][(int)(p->pos.y / 16)].state != 1)
			p->pos.x += p->d.x * .8;
		if (grid[(int)(p->pos.x / 16)][(int)((p->pos.y + p->d.y * 2) / 16)].state != 1)
			p->pos.y += p->d.y * .8;
	}
	if (keystate[SDL_SCANCODE_S])
	{
		if (grid[(int)((p->pos.x - p->d.x * 2) / 16)][(int)(p->pos.y / 16)].state != 1)
			p->pos.x -= p->d.x * .8;
		if (grid[(int)(p->pos.x / 16)][(int)((p->pos.y - p->d.y * 2) / 16)].state != 1)
			p->pos.y -= p->d.y * .8;
	}
	if (keystate[SDL_SCANCODE_A])
	{
		if (grid[(int)((p->pos.x + cos(strafe)) / 16)][(int)(p->pos.y / 16)].state != 1)
			p->pos.x += cos(strafe) * .8;
		if (grid[(int)(p->pos.x / 16)][(int)((p->pos.y + sin(strafe)) / 16)].state != 1)
			p->pos.y += sin(strafe) * .8;
	}
	if (keystate[SDL_SCANCODE_D])
	{
		if (grid[(int)((p->pos.x - cos(strafe)) / 16)][(int)(p->pos.y / 16)].state != 1)
			p->pos.x -= cos(strafe) * .8;
		if (grid[(int)(p->pos.x / 16)][(int)((p->pos.y - sin(strafe)) / 16)].state != 1)
			p->pos.y -= sin(strafe) * .8;
	}
}
