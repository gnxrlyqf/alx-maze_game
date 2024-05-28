#include "main.h"
#include <math.h>
#include <SDL2/SDL.h>
#define PI 3.14159265358979
#define DEG 0.0174533
#define RAD 57.2957795
float rtheta;

ray *raycast(SDL_Renderer *r, int size, coords dim, cell **grid, ray *rays, player p)
{
	/* Convert world coordinates to grid coordinates */
	dim.x = dim.x / size;
	dim.y = dim.y / size;
	/* Initialize FOV */
	rtheta = p.theta - (DEG * 30);
	int i;
	ray ray, hor, ver;

	for (int i = 0; i < 1260; i++)
	{
		/* Get vertical/horizontal line checks */
		ver = vertical(rtheta, size, dim, grid, p.pos);
		hor = horizontal(rtheta, size, dim, grid, p.pos);
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

ray horizontal(float rtheta, int size, coords dim, cell **grid, coordsf pos)
{
	coordsf offset;
	ray ray, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
	/* Calculate offset */
	if (rtheta > PI)
	{
		ray.pos.y = (((int)pos.y >> 4) << 4) - .0001;
		ray.pos.x = (pos.y - ray.pos.y) / (-tan(rtheta)) + pos.x;
		offset.y = -size, offset.x = offset.y / tan(rtheta);
	}
	if (rtheta < PI)
	{
		ray.pos.y = (((int)pos.y >> 4) << 4) + size;
		ray.pos.x = (pos.y - ray.pos.y) / (-tan(rtheta)) + pos.x;
		offset.y = size, offset.x = offset.y / tan(rtheta);
	}
	/* Edge case for ray being angled straight right or left */
	if (rtheta == 0 || rtheta == PI)
		ray.pos.x = pos.x, ray.pos.y = pos.y, dof = 0;
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
			length = distance(pos.x, pos.y, ray.pos.x, ray.pos.y);
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

ray vertical(float rtheta, int size, coords dim, cell **grid, coordsf pos)
{
	coordsf offset;
	ray ray, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
	/* Calculate offset */
	if (rtheta > (PI / 2) && rtheta < ((3 * PI) / 2))
	{
		ray.pos.x = (((int)pos.x >> 4) << 4) - .0001;
		ray.pos.y = (pos.x - ray.pos.x) * (-tan(rtheta)) + pos.y;
		offset.x = -size, offset.y = offset.x * tan(rtheta);
	}
	if (rtheta < (PI / 2) || rtheta > ((3 * PI) / 2))
	{
		ray.pos.x = (((int)pos.x >> 4) << 4) + size;
		ray.pos.y = (pos.x - ray.pos.x) * (-tan(rtheta)) + pos.y;
		offset.x = size, offset.y = offset.x * tan(rtheta);
	}
	/* Edge case for ray being angled straight up or down */
	if (rtheta == PI / 2 || rtheta == 2 * PI / 3)
		ray.pos.x = pos.x, ray.pos.y = pos.y, dof = 0;
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
			length = distance(pos.x, pos.y, ray.pos.x, ray.pos.y);
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

column *process_rays(ray *rays, int size, float theta)
{
	int i, j;
	float line, distance, lens, brightness;
	rgba fog = {216, 217, 218, 0}, **texture = init_texture("bricks.png", 0);
	column *walls = malloc(sizeof(column) * 1260);

	for (int i = 0; i < 1260; i++)
	{
		walls[i].pos.x = i;
		/* Fix fisheye distortion */
		lens = theta - rays[i].theta;
		if (lens < 0)
			lens += 2 * PI;
		if (lens > 2 * PI)
			lens -= 2 * PI;
		distance = (rays[i].val * 20) * cos(lens);
		walls[i].dist = rays[i].val;
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
	quick_sort_wall(walls, 0, 1259);
	return (walls);
}

sprite *process_sprites(SDL_Renderer *renderer, entity *entities, int size, player p)
{
	int i;
	float scale;
	entity curr;
	SDL_Rect draw;
	SDL_Texture *texture;
	coordsf temp, result;
	sprite *sprites = malloc(sizeof(sprite) * size);

	for (i = 0; i < 2; i++)
	{
		curr = entities[i];
		temp.x = curr.pos.x - p.pos.x;
		temp.y = curr.pos.y - p.pos.y;
		result.x = (temp.y * cos(p.theta)) - (temp.x * sin(p.theta));
		result.y = (temp.x * cos(p.theta)) + (temp.y * sin(p.theta));
		scale = 360 / result.y;
		sprites[i].dist = distance(curr.pos.x, curr.pos.y, p.pos.x, p.pos.y);
		sprites[i].scale = scale;
		curr.pos.x = result.x, curr.pos.y = result.y;
		draw.x = (result.x * 1260 / result.y) + (1260 / 2) - (16 * scale);
		draw.y = (curr.z * 1260 / result.y) + (720 / 2);
		draw.w = draw.h = scale * 32;
		sprites[i].rect = draw;
		sprites[i].texture =  SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skull007.bmp"));
	}
	// quick_sort_sprite(sprites, 0, 1);
	return (sprites);
}