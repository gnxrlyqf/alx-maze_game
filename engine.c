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
	poll_controls(grid);
	SDL_SetRenderDrawColor(instance.renderer, 255, 0, 0, 255);
	SDL_Rect player = {posX - 4, posY - 4, 8, 8};
	SDL_RenderFillRect(instance.renderer, &player);
}

ray *raytracing(SDL_instance instance, int size, coords dim, cell **grid, ray *rays)
{
	dim.x = dim.x / size;
	dim.y = dim.y / size;
	rtheta = theta - (DEG * 30);
	int i;
	ray ray, hor, ver;

	for (int i = 0; i < 1260; i++)
	{
		ver = vertical(rtheta, size, dim, grid);
		hor = horizontal(rtheta, size, dim, grid);
		if (hor.val < ver.val)
			rays[i].pos.x = hor.pos.x, rays[i].pos.y = hor.pos.y, rays[i].val = hor.val, rays[i].theta = rtheta, rays[i].dir = 1;
		if (ver.val < hor.val)
			rays[i].pos.x = ver.pos.x, rays[i].pos.y = ver.pos.y, rays[i].val = ver.val, rays[i].theta = rtheta, rays[i].dir = 0;
		SDL_RenderDrawLine(instance.renderer, posX, posY, rays[i].pos.x, rays[i].pos.y);
		rtheta += DEG / 21;
		if (rtheta < 0)
			rtheta += 2 * PI;
		if (rtheta > 2 * PI)
			rtheta -= 2 * PI;
	}
	return (rays);
}

float distance(float ax ,float ay, float bx, float by, float theta)
{
	return(sqrt(pow(bx - ax, 2) + pow(by - ay, 2)));
}

ray horizontal(float rtheta, int size, coords dim, cell **grid)
{
	coordsf offset, horizontal = {posX, posY};
	ray ray, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
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
	if (rtheta == 0 || rtheta == PI)
		ray.pos.x = posX, ray.pos.y = posY, dof = 0;
	while (dof < 55)
	{
		map.x = (int)ray.pos.x >> 4, map.y = (int)ray.pos.y >> 4;
		if (0 <= map.x && map.x < dim.x && 0 <= map.y && map.y < dim.y && grid[map.x][map.y].state == 1)
		{
			length = distance(posX, posY, ray.pos.x, ray.pos.y, rtheta);
			break;
		}	
		else
			ray.pos.x += offset.x, ray.pos.y += offset.y;
		dof += 1;
	}
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
	if (rtheta == 0 || rtheta == PI)
		ray.pos.x = posX, ray.pos.y = posY, dof = 0;
	while (dof < 33)
	{
		map.x = (int)ray.pos.x >> 4, map.y = ((int)ray.pos.y >> 4);
		if (0 <= map.x && map.x < dim.x && 0 <= map.y && map.y < dim.y && grid[map.x][map.y].state == 1)
		{
			length = distance(posX, posY, ray.pos.x, ray.pos.y, rtheta);
			vertical.x = ray.pos.x, vertical.x = ray.pos.y;
			break;
		}	
		else
			ray.pos.x += offset.x, ray.pos.y += offset.y;
		dof += 1;
	}
	out.pos.x = ray.pos.x, out.pos.y = ray.pos.y, out.val = length;
	return (out);
}

void draw_walls(SDL_instance instance, ray *rays, int size, int thickness, coords resolution)
{
	int i, j, x, y, flip = 0, linediv, wallx;
	float line, distance, lens, shade, brightness, fogval;
	rgba fog = {216, 217, 218, 0}, **texture = init_wall("bricks.png");

	for (int i = 0; i < 1260; i++)
	{
		lens = theta - rays[i].theta;
		if (lens < 0) lens += 2 * PI;
		if (lens > 2 * PI) lens -= 2 * PI;
		distance = (rays[i].val * 20) * cos(lens);
		line = (size * 720) / distance;
		if (line > 720) line = 720;
		x = i, y = (resolution.y - line * 32) / 2;
		wallx = rays[i].dir == 1 ? (int)(rays[i].pos.x * 2) % 32 : 31 - (int)(rays[i].pos.y * 2) % 32;
		brightness = 1 - rays[i].val / 720;
		if (brightness < 0) brightness = 0;
		shade = (rays[i].dir == 0 ? .8 : 1) * brightness;
		fogval = rays[i].val / 300;
		if (fogval > 1) fogval = 1;
		for (j = 0; j < 32; j++)
		{
			SDL_Rect wall = {x, y + (line * j), thickness, line + 1};
			SDL_SetRenderDrawColor(instance.renderer,
				(texture[wallx][j].r * shade) * (1 - fogval) + (fogval * fog.r),
				(texture[wallx][j].g * shade) * (1 - fogval) + (fogval * fog.g),
				(texture[wallx][j].b * shade) * (1 - fogval) + (fogval * fog.b),
				(texture[wallx][j].a * shade) * (1 - fogval) + (fogval * fog.a)
			);
			SDL_RenderFillRect(instance.renderer, &wall);
		}
	}
}

void draw_sprite(SDL_instance map , SDL_instance display, ray *rays)
{
	SDL_Rect point, mappoint = {13 * 16, 5 * 16, 16, 16};
	sprite test = {0, {13 * 16, 5 * 16}, 0};
	coordsf temp = {test.pos.x - posX, test.pos.y - posY};
	coordsf result = {(temp.y * cos(theta)) - (temp.x * sin(theta)), (temp.x * cos(theta)) + (temp.y * sin(theta))};

	test.pos.x = result.x, test.pos.y = result.y;
	point.x = (result.x * 1260 / result.y) + (1260 / 2);
	point.y = (test.z * 1260 / result.y) + (720 / 2);
	point.w = 16, point.h = 16;
	SDL_SetRenderDrawColor(display.renderer, 255, 240, 0, 0);
	SDL_SetRenderDrawColor(map.renderer, 255, 240, 0, 0);
	SDL_RenderFillRect(map.renderer, &mappoint);
	if (point.x > 0 && point.x < 1260 && result.y < rays[point.x].val)
		SDL_RenderFillRect(display.renderer, &point);
}

void poll_controls(cell **grid)
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	
	strafe = theta - 90 * DEG;
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
	if (keystate[SDL_SCANCODE_W])
	{
		if (grid[(int)((posX + dx * 2) / 16)][(int)(posY / 16)].state != 1)
			posX += dx;
		if (grid[(int)(posX / 16)][(int)((posY + dy * 2) / 16)].state != 1)
			posY += dy;
	}
	if (keystate[SDL_SCANCODE_S])
	{
		if (grid[(int)((posX - dx * 2) / 16)][(int)(posY / 16)].state != 1)
			posX -= dx;
		if (grid[(int)(posX / 16)][(int)((posY - dy * 2) / 16)].state != 1)
			posY -= dy;
	}
	if (keystate[SDL_SCANCODE_A])
	{
		if (grid[(int)((posX += cos(strafe) / 2) / 16)][(int)(posY / 16)].state != 1)
			posX += cos(strafe) / 2;
		if (grid[(int)(posX / 16)][(int)((posY += sin(strafe) / 2) / 16)].state != 1)
			posY += sin(strafe) / 2;		
	}
	if (keystate[SDL_SCANCODE_D])
	{
		if (grid[(int)((posX -= cos(strafe) / 2) / 16)][(int)(posY / 16)].state != 1)
			posX -= cos(strafe) / 2;
		if (grid[(int)(posX / 16)][(int)((posY -= sin(strafe) / 2) / 16)].state != 1)
			posY -= sin(strafe) / 2;		
	}
}