#include "main.h"
#include <math.h>
#include <SDL2/SDL.h>
#define PI 3.14159265358979
#define DEG 0.0174533
#define RAD 57.2957795

/**
 * raycast - casts rays to detect walls in front of player
 * @size: size of grid cell
 * @dim: world dimensions
 * @grid: two dimensional array of cells representing grid
 * @rays: array of rays
 * @p: player struct
 *
 * Return: array of rays hit
*/
ray *raycast(int size, coords dim, cell **grid, ray *rays, player p)
{
	dim.x = dim.x / size;
	dim.y = dim.y / size;
	float rtheta = p.theta - (DEG * 30);
	int i;
	ray ray, hor, ver;

	for (int i = 0; i < 1260; i++)
	{
		ver = vertical(rtheta, size, dim, grid, p.pos);
		hor = horizontal(rtheta, size, dim, grid, p.pos);
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
		rtheta += DEG / 21;
		if (rtheta < 0)
			rtheta += 2 * PI;
		if (rtheta > 2 * PI)
			rtheta -= 2 * PI;
	}
	return (rays);
}

/**
 * horizontal - checks for horizontal grid lines
 * @rtheta: current ray angle
 * @size: grid cell size
 * @dim: grid dimensions
 * @grid: two dimensional array of cells representing grid
 * @pos: player position
 *
 * Return: resulting ray
*/
ray horizontal(float rtheta, int size, coords dim, cell **grid, coordsf pos)
{
	coordsf offset;
	ray ray, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
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
	if (rtheta == 0 || rtheta == PI)
		ray.pos.x = pos.x, ray.pos.y = pos.y, dof = 0;
	while (dof < 55)
	{
		map.x = (int)ray.pos.x >> 4, map.y = (int)ray.pos.y >> 4;
		if (map.x >= 0 && map.x < dim.x &&
			map.y >= 0 && map.y < dim.y &&
			grid[map.x][map.y].state == 1) {
			length = distance(pos, ray.pos);
			break;
		}
		else
			ray.pos.x += offset.x, ray.pos.y += offset.y;
		dof += 1;
	}
	out.pos.x = ray.pos.x, out.pos.y = ray.pos.y, out.val = length;
	return (out);
}

/**
 * vertical - checks for vertical grid lines
 * @rtheta: current ray angle
 * @size: grid cell size
 * @dim: grid dimensions
 * @grid: two dimensional array of cells representing grid
 * @pos: player position
 *
 * Return: resulting ray
*/
ray vertical(float rtheta, int size, coords dim, cell **grid, coordsf pos)
{
	coordsf offset;
	ray ray, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
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
	if (rtheta == PI / 2 || rtheta == 2 * PI / 3)
		ray.pos.x = pos.x, ray.pos.y = pos.y, dof = 0;
	while (dof < 33)
	{
		map.x = (int)ray.pos.x >> 4, map.y = ((int)ray.pos.y >> 4);
		if (map.x >= 0 && map.x < dim.x &&
			map.y >= 0 && map.y < dim.y &&
			grid[map.x][map.y].state == 1) {
			length = distance(pos, ray.pos);
			break;
		}
		else
			ray.pos.x += offset.x, ray.pos.y += offset.y;
		dof += 1;
	}
	out.pos.x = ray.pos.x, out.pos.y = ray.pos.y, out.val = length;
	return (out);
}

/**
 * process_rays - processes rays returned by raycaster
 * @rays: ray array to process
 * @size: cell size
 * @theta: player angle
 *
 * Return: array of vertical lines to be draw on the screen
*/
void process_rays(ray *rays, column **walls, int size, float theta)
{
	int i, j;
	float line, distance, lens, brightness;
	rgba fog = {216, 217, 218, 0};

	for (int i = 0; i < 1260; i++)
	{
		(*walls)[i].pos.x = i;
		lens = theta - rays[i].theta;
		if (lens < 0)
			lens += 2 * PI;
		if (lens > 2 * PI)
			lens -= 2 * PI;
		distance = (rays[i].val * 20) * cos(lens);
		(*walls)[i].dist = rays[i].val;
		(*walls)[i].line = (size * 720) / distance;
		if ((*walls)[i].line > 720)
			(*walls)[i].line = 720;
		if (rays[i].dir == 1)
			(*walls)[i].x = (int)(rays[i].pos.x * 2) % 32;
		if (rays[i].dir == 0)
			(*walls)[i].x = 31 - (int)(rays[i].pos.y * 2) % 32;
		brightness = 1 - rays[i].val / 720;
		if (brightness < 0)
			brightness = 0;
		(*walls)[i].s = (rays[i].dir == 0 ? .8 : 1) * brightness;
		(*walls)[i].f = rays[i].val / 300;
		if ((*walls)[i].f > 1)
			(*walls)[i].f = 1;
	}
	quick_sort_wall(*walls, 0, 1259);
}

/**
 * process_sprites - processes entities into sprites to be drawn
 * @r: pointer to renderer
 * @e: array of entities
 * @size: size of array
 * @p: player struct
 *
 * Return: array of sprites to be drawn on the screen
*/
sprite *process_sprites(SDL_Renderer *r, entity *e, sprite* s, int c, player p)
{
	int i;
	float scale;
	entity curr;
	SDL_Rect draw;
	coordsf temp, result;

	for (i = 0; i < c; i++)
	{
		s[i].visible = 1;
		curr = e[i];
		temp.x = curr.pos.x - p.pos.x;
		temp.y = curr.pos.y - p.pos.y;
		result.x = (temp.y * cos(p.theta)) - (temp.x * sin(p.theta));
		result.y = (temp.x * cos(p.theta)) + (temp.y * sin(p.theta));
		scale = 360 / result.y;
		s[i].dist = distance(curr.pos, p.pos);
		s[i].scale = scale;
		curr.pos.x = result.x, curr.pos.y = result.y;
		draw.x = (result.x * 1260 / result.y) + (1260 / 2) - (8 * scale);
		draw.y = (3 * 1260 / result.y) + (720 / 2);
		draw.w = draw.h = scale * 16;
		if (e[i].exists != 1)
			s[i].visible = 0;
		s[i].rect = draw;
	}
	quick_sort_sprite(s, 0, c - 1);
	return(s);
}
