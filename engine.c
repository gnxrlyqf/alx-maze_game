#include "main.h"
#include <math.h>
#include <SDL2/SDL.h>
#define PI 3.14159265358979
#define DEG 0.0174533
#define RAD 57.2957795
float theta = PI / 2, dx, dy, strafe;
float posX = 264, posY = 48;
float rtheta;

void draw_player(SDL_instance instance, cell **grid)
{
	SDL_Rect player = {posX - 4, posY - 4, 8, 8};

	poll_controls(grid);
	SDL_SetRenderDrawColor(instance.renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(instance.renderer, &player);
}

ray *raycasting(SDL_Renderer *renderer, int size, coords dim, cell **grid,
ray *rays) {
	/* Convert world coordinates to grid coordinates */
	dim.x = dim.x / size;
	dim.y = dim.y / size;
	/* Initialize FOV */
	rtheta = theta - (DEG * 30);
	int i;
	ray ray, hor, ver;

	for (int i = 0; i < 1260; i++)
	{
		/* Get vertical/horizontal line checks */
		ver = vertical(rtheta, size, dim, grid);
		hor = horizontal(rtheta, size, dim, grid);
		/* Compare vertical and horizontal lengths, pick the shorter one*/
		if (hor.val < ver.val)
		{
			rays[i].pos.x = hor.pos.x, rays[i].pos.y = hor.pos.y;
			rays[i].val = hor.val, rays[i].theta = rtheta, rays[i].dir = 1;
		}
		if (ver.val < hor.val)
		{
			rays[i].pos.x = ver.pos.x, rays[i].pos.y = ver.pos.y;
			rays[i].val = ver.val, rays[i].theta = rtheta, rays[i].dir = 0;
		}
		SDL_RenderDrawLine(renderer, posX, posY, rays[i].pos.x, rays[i].pos.y);
		/* Increment ray angle */
		rtheta += DEG / 21;
		/* Normalize ray angle */
		if (rtheta < 0)
			rtheta += 2 * PI;
		if (rtheta > 2 * PI)
			rtheta -= 2 * PI;
	}
	return (rays);
}

float distance(float ax, float ay, float bx, float by, float theta)
{
	return (sqrt(pow(bx - ax, 2) + pow(by - ay, 2)));
}

ray horizontal(float rtheta, int size, coords dim, cell **grid)
{
	coordsf offset, horizontal = {posX, posY};
	ray ray, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
	/* Calculate offset */
	if (rtheta > PI)
	{
		ray.pos.y = (((int)posY >> 4) << 4) - .0001;
		ray.pos.x = (posY - ray.pos.y) / (-tan(rtheta)) + posX;
		offset.y = -size, offset.x = offset.y / tan(rtheta);
	}
	if (rtheta < PI)
	{
		ray.pos.y = (((int)posY >> 4) << 4) + size;
		ray.pos.x = (posY - ray.pos.y) / (-tan(rtheta)) + posX;
		offset.y = size, offset.x = offset.y / tan(rtheta);
	}
	/* Edge case for ray being angled straight right or left */
	if (rtheta == 0 || rtheta == PI)
		ray.pos.x = posX, ray.pos.y = posY, dof = 0;
	/* Check for walls */
	while (dof < 55)
	{
		/* Convert ray world position to grid position */
		map.x = (int)ray.pos.x >> 4, map.y = (int)ray.pos.y >> 4;
		/**
		 * Check cell state, if:
		 * Wall - record length and exit loop
		 * No wall - add offset and reiterate
		*/
		if (map.x >= 0 && map.x < dim.x &&
			map.y >= 0 && map.y < dim.y &&
			grid[map.x][map.y].state == 1) {
			length = distance(posX, posY, ray.pos.x, ray.pos.y, rtheta);
			break;
		}
		else
			ray.pos.x += offset.x, ray.pos.y += offset.y;
		dof += 1;
	}
	/* Assign final ray values and return */
	out.pos.x = ray.pos.x, out.pos.y = ray.pos.y, out.val = length;
	return (out);
}

ray vertical(float rtheta, int size, coords dim, cell **grid)
{
	coordsf offset, vertical = {posX, posY};
	ray ray, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
	/* Calculate offset */
	if (rtheta > (PI / 2) && rtheta < ((3 * PI) / 2))
	{
		ray.pos.x = (((int)posX >> 4) << 4) - .0001;
		ray.pos.y = (posX - ray.pos.x) * (-tan(rtheta)) + posY;
		offset.x = -size, offset.y = offset.x * tan(rtheta);
	}
	if (rtheta < (PI / 2) || rtheta > ((3 * PI) / 2))
	{
		ray.pos.x = (((int)posX >> 4) << 4) + size;
		ray.pos.y = (posX - ray.pos.x) * (-tan(rtheta)) + posY;
		offset.x = size, offset.y = offset.x * tan(rtheta);
	}
	/* Edge case for ray being angled straight up or down */
	if (rtheta == PI / 2 || rtheta == 2 * PI / 3)
		ray.pos.x = posX, ray.pos.y = posY, dof = 0;
	/* Check for walls */
	while (dof < 33)
	{
		/* Convert ray world position to grid position */
		map.x = (int)ray.pos.x >> 4, map.y = ((int)ray.pos.y >> 4);
		if (map.x >= 0 && map.x < dim.x &&
			map.y >= 0 && map.y < dim.y &&
			grid[map.x][map.y].state == 1) {
			/**
			 * Check cell state, if:
			 * Wall - record length and exit loop
			 * No wall - add offset and reiterate
			*/
			length = distance(posX, posY, ray.pos.x, ray.pos.y, rtheta);
			break;
		}
		else
			ray.pos.x += offset.x, ray.pos.y += offset.y;
		dof += 1;
	}
	/* Assign final ray values and return */
	out.pos.x = ray.pos.x, out.pos.y = ray.pos.y, out.val = length;
	return (out);
}

column *process_rays(SDL_instance instance, ray *rays, int size, coords res, column *walls)
{
	int i, j;
	float line, distance, lens, brightness;
	rgba fog = {216, 217, 218, 0}, **texture = init_texture("bricks.png", 0);

	for (int i = 0; i < 1260; i++)
	{
		/* Fix fisheye distortion */
		lens = theta - rays[i].theta;
		if (lens < 0)
			lens += 2 * PI;
		if (lens > 2 * PI)
			lens -= 2 * PI;
		distance = (rays[i].val * 20) * cos(lens);
		/* Calculate column height*/
		walls[i].line = (size * 720) / distance;
		if (walls[i].line > 720)
			walls[i].line = 720;
		/* Calculate wall texture x index */
		if (rays[i].dir == 1)
			walls[i].x = (int)(rays[i].pos.x * 2) % 32;
		if (rays[i].dir == 0)
			walls[i].x = 31 - (int)(rays[i].pos.y * 2) % 32;
		/* Calculate brightness and shade factors */
		brightness = 1 - rays[i].val / 720;
		if (brightness < 0)
			brightness = 0;
		walls[i].s = (rays[i].dir == 0 ? .8 : 1) * brightness;
		walls[i].f = rays[i].val / 300;
		if (walls[i].f > 1)
			walls[i].f = 1;
	}
	return (walls);
}

sprite *process_sprites(SDL_Renderer *renderer, ray *rays, entity *entities, sprite *sprites)
{
	int i, size = sizeof(entities) / sizeof(entity);
	float scale;
	entity curr;
	SDL_Rect draw;
	SDL_Texture *texture;
	coordsf temp, result;

	for (i = 0; i < 2; i++)
	{
		curr = entities[i];
		temp.x = curr.pos.x - posX;
		temp.y = curr.pos.y - posY;
		result.x = (temp.y * cos(theta)) - (temp.x * sin(theta));
		result.y = (temp.x * cos(theta)) + (temp.y * sin(theta));
		scale = 360 / result.y;
		curr.pos.x = result.x, curr.pos.y = result.y;
		draw.x = (result.x * 1260 / result.y) + (1260 / 2) - (16 * scale);
		draw.y = (curr.z * 1260 / result.y) + (720 / 2);
		draw.w = draw.h = scale * 32;
		sprites[i].rect = draw;
		sprites[i].texture =  SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skull007.bmp"));
	}
	return (sprites);
}

void poll_controls(cell **grid)
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	strafe = theta - 90 * DEG;
	/* rotate player and calculate offset */
	if (keystate[SDL_SCANCODE_LEFT])
	{
		theta -= .025;
		dx = cos(theta), dy = sin(theta);
	}
		if (keystate[SDL_SCANCODE_RIGHT])
	{
		theta += .025;
		dx = cos(theta), dy = sin(theta);
	}
	/* Check for collisions, move player if none*/
	if (keystate[SDL_SCANCODE_W])
	{
		if (grid[(int)((posX + dx * 2) / 16)][(int)(posY / 16)].state != 1)
			posX += dx * .8;
		if (grid[(int)(posX / 16)][(int)((posY + dy * 2) / 16)].state != 1)
			posY += dy * .8;
	}
	if (keystate[SDL_SCANCODE_S])
	{
		if (grid[(int)((posX - dx * 2) / 16)][(int)(posY / 16)].state != 1)
			posX -= dx * .8;
		if (grid[(int)(posX / 16)][(int)((posY - dy * 2) / 16)].state != 1)
			posY -= dy * .8;
	}
	if (keystate[SDL_SCANCODE_A])
	{
		if (grid[(int)((posX + cos(strafe)) / 16)][(int)(posY / 16)].state != 1)
			posX += cos(strafe) * .8;
		if (grid[(int)(posX / 16)][(int)((posY + sin(strafe)) / 16)].state != 1)
			posY += sin(strafe) * .8;
	}
	if (keystate[SDL_SCANCODE_D])
	{
		if (grid[(int)((posX - cos(strafe)) / 16)][(int)(posY / 16)].state != 1)
			posX -= cos(strafe) * .8;
		if (grid[(int)(posX / 16)][(int)((posY - sin(strafe)) / 16)].state != 1)
			posY -= sin(strafe) * .8;
	}
}
