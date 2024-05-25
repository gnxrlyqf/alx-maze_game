#ifndef MAIN_H
#define MAIN_H
#include <SDL2/SDL.h>

typedef struct SDL_instance
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} SDL_instance;

typedef struct cell
{
    int x, y;
    int size;
    int state;
} cell;

typedef struct coords
{
	int x, y;
} coords;

typedef struct coordsf
{
	float x, y;
} coordsf;

typedef struct ray
{
	coordsf pos;
	float val, theta, dir;
} ray;

typedef struct frontier
{
	coords cell;
	struct frontier *prev, *next;
} frontier;

typedef struct rgba
{
	int r, g, b, a;
} rgba;

typedef struct sprite
{
	int hostility;
	coordsf pos;
	float z, theta;
	rgba **texture;
} sprite;

int instantiate(SDL_instance *instance, coords dimensions, char *name, coords pos);
void draw_grid(SDL_instance instance, cell **grid, coords dimensions, int cellSize);
cell **init_grid(coords dimensions, int cellSize);
int events(cell **grid, int size);
void free_grid(cell **grid, int rows);
void draw_player(SDL_instance instance, cell **grid);
void poll_controls(cell **grid);
ray *raytracing(SDL_instance instance, int size, coords dimensions, cell **grid, ray *rays);
float distance(float ax ,float ay, float bx, float by, float tau);
ray horizontal(float raytau, int size, coords dim, cell **grid);
ray vertical(float raytau, int size, coords dim, cell **grid);
int ftoi(float x);
void draw_walls(SDL_instance instance, ray *rays, int size, int thickness, coords resolution);
void maze(cell **grid, coords first, coords range);
frontier *add(frontier **head, coords new);
frontier *get(frontier *head, int index);
int delete(frontier **head, unsigned int index);
int listlen(const frontier *h);
rgba **init_wall(char *file);
void patch(cell **grid, coords size, coords pos);
rgba **init_sprite(char *file);
void draw_sprite(SDL_instance map , SDL_instance display, ray *rays);

#endif