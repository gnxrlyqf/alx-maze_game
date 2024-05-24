#include "main.h"
#include <math.h>
#include <SDL2/SDL.h>
#define PI 3.14159265358979
#define DEG 0.0174533
float theta = PI / 2, dx, dy, strafe;
float posX = 264, posY = 48;
float raytheta;

void draw_player(SDL_instance instance, cell **grid)
{
	poll_controls(grid);
	SDL_SetRenderDrawColor(instance.renderer, 255, 0, 0, 255);
	SDL_Rect player = {posX - 4, posY - 4, 8, 8};
	SDL_RenderFillRect(instance.renderer, &player);
}

coordsf *raytracing(SDL_instance instance, int size, coords dimensions, cell **grid, coordsf *rays)
{
	dimensions.x = dimensions.x / size;
	dimensions.y = dimensions.y / size;
	raytheta = theta - (DEG * 40);
	int i;
	coordsf ray, hor, ver;

	for (int i = 0; i < 1280; i++)
	{
		ver = vertical(raytheta, size, dimensions, grid);
		hor = horizontal(raytheta, size, dimensions, grid);
		if (hor.val < ver.val)
			rays[i].x = hor.x, rays[i].y = hor.y, rays[i].val = hor.val, rays[i].dir = 1;
		if (ver.val < hor.val)
			rays[i].x = ver.x, rays[i].y = ver.y, rays[i].val = ver.val, rays[i].dir = 0;
		SDL_RenderDrawLine(instance.renderer, posX, posY, rays[i].x, rays[i].y);
		raytheta += DEG / 16;
		if (raytheta < 0)
			raytheta += 2 * PI;
		if (raytheta > 2 * PI)
			raytheta -= 2 * PI;
	}
	return (rays);
}

float distance(float ax ,float ay, float bx, float by, float theta)
{
	return(sqrt(pow(bx - ax, 2) + pow(by - ay, 2)));
}

coordsf horizontal(float raytheta, int size, coords dimensions, cell **grid)
{
	coordsf offset, ray, horizontal = {posX, posY}, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
	if (raytheta > PI)
	{
		ray.y = (((int)posY >> 4) << 4) - .0001;
		ray.x = (posY - ray.y) / (-tan(raytheta)) + posX;
		offset.y = -size, offset.x = offset.y / tan(raytheta);
	}
	if (raytheta < PI)
	{
		ray.y = (((int)posY >> 4) << 4) + size;
		ray.x = (posY - ray.y) / (-tan(raytheta)) + posX;
		offset.y = size, offset.x = offset.y / tan(raytheta);
	}
	if (raytheta == 0 || raytheta == PI)
		ray.x = posX, ray.y = posY, dof = 0;
	while (dof < 32)
	{
		map.x = (int)ray.x >> 4, map.y = (int)ray.y >> 4;
		if (0 <= map.x && map.x < dimensions.x && 0 <= map.y && map.y < dimensions.y && grid[map.x][map.y].state == 1)
		{
			length = distance(posX, posY, ray.x, ray.y, raytheta);
			break;
		}	
		else
			ray.x += offset.x, ray.y += offset.y;
		dof += 1;
	}
	out.x = ray.x, out.y = ray.y, out.val = length;
	return (out);
}

coordsf vertical(float raytheta, int size, coords dimensions, cell **grid)
{
	coordsf offset, ray, vertical = {posX, posY}, out;
	coords map;
	int dof = 0;
	float length = 999999999;

	out.val = 0;
	if (raytheta > (PI / 2) && raytheta < ((3 * PI) / 2))
	{
		ray.x = (((int)posX >> 4) << 4) - .0001;
		ray.y = (posX - ray.x) * (-tan(raytheta)) + posY;
		offset.x = -size, offset.y = offset.x * tan(raytheta);
	}
	if (raytheta < (PI / 2) || raytheta > ((3 * PI) / 2))
	{
		ray.x = (((int)posX >> 4) << 4) + size;
		ray.y = (posX - ray.x) * (-tan(raytheta)) + posY;
		offset.x = size, offset.y = offset.x * tan(raytheta);
	}
	if (raytheta == 0 || raytheta == PI)
		ray.x = posX, ray.y = posY, dof = 0;
	while (dof < 32)
	{
		map.x = (int)ray.x >> 4, map.y = ((int)ray.y >> 4);
		if (0 <= map.x && map.x < dimensions.x && 0 <= map.y && map.y < dimensions.y && grid[map.x][map.y].state == 1)
		{
			length = distance(posX, posY, ray.x, ray.y, raytheta);
			vertical.x = ray.x, vertical.x = ray.y;
			break;
		}	
		else
			ray.x += offset.x, ray.y += offset.y;
		dof += 1;
	}
	out.x = ray.x, out.y = ray.y, out.val = length;
	return (out);
}

void draw_walls(SDL_instance instance, coordsf *rays, int size, int thickness, coords resolution)
{
	int i, j, x, y, flip = 0, linediv, wallx;
	float line, distance, fisheyefix, shade, brightness, fogval;
	rgba fog = {216, 217, 218, 0}, **texture = init_wall("wall.png");

	for (int i = 0; i < 1280; i++)
	{
		fisheyefix = theta - raytheta;
		if (fisheyefix < 0)
			fisheyefix += 2 * PI;
		if (fisheyefix > 2 * PI)
			fisheyefix -= 2 * PI;
		distance = (rays[i].val * 32) * cos(fisheyefix);
		line = (size * 720) / distance;
		if (line > 720) line = 720;
		x = i, y = (resolution.y - line * 32) / 2;
		wallx = rays[i].dir == 1 ? (int)(rays[i].x * 2) % 32 : 31 - (int)(rays[i].y * 2) % 32;
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

void draw_sprite(char *file, SDL_instance instance)
{
	// rgba **sprite = init_sprite("skull.png");
	coords pos = {13 * 32, 32};
	coordsf angles = {cos(theta), sin(theta)};
	float a, b;

	pos.x -=  posX, pos.y -= posY;
	a = (pos.y * angles.x) + (pos.x * angles.y);
	b = (pos.x * angles.x) + (pos.y + angles.y);
	pos.x = a, pos.y = b;

	pos.x = (pos.x * 108 / pos.y) + (120 / 2);
	pos.y = (108 / pos.y) + (80 / 2);

	SDL_RenderDrawPoint(instance.renderer, pos.x * 16, pos.y * 16);
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
		if (grid[(int)((posX -= cos(strafe) / 4) / 16)][(int)(posY / 16)].state != 1)
			posX -= cos(strafe) / 4;
		if (grid[(int)(posX / 16)][(int)((posY -= sin(strafe) / 4) / 16)].state != 1)
			posY -= sin(strafe) / 4;		
	}
	if (keystate[SDL_SCANCODE_D])
	{
		if (grid[(int)((posX += cos(strafe) / 4) / 16)][(int)(posY / 16)].state != 1)
			posX += cos(strafe) / 4;
		if (grid[(int)(posX / 16)][(int)((posY += sin(strafe) / 4) / 16)].state != 1)
			posY += sin(strafe) / 4;		
	}
}