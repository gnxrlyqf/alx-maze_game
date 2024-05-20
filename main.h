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
	int neighbors;
} cell;

typedef struct coords
{
	int x, y;
} coords;

typedef struct coordsf
{
	float x, y;
	float val;
	int dir;
} coordsf;

typedef struct frontier
{
	coords cell;
	struct frontier *prev, *next;
} frontier;

typedef struct rgba
{
	int r, g, b;
	int a;
} rgba;

int instantiate(SDL_instance *instance, coords dimensions, char *name, coords pos);
void draw_grid(SDL_instance instance, cell **grid, coords dimensions, int cellSize);
cell **init_grid(coords dimensions, int cellSize);
int events(cell **grid, int size);
void free_grid(cell **grid, int rows);
void draw_player(SDL_instance instance, cell **grid);
void poll_controls(cell **grid);
coordsf *raytracing(SDL_instance instance, int size, coords dimensions, cell **grid, coordsf *rays);
float distance(float ax ,float ay, float bx, float by, float tau);
coordsf horizontal(float raytau, int size, coords dimensions, cell **grid);
coordsf vertical(float raytau, int size, coords dimensions, cell **grid);
int ftoi(float x);
void draw_walls(SDL_instance instance, coordsf *rays, int size, int thickness, coords resolution);
void maze(cell **grid, coords first, coords range);
frontier *add(frontier **head, coords new);
frontier *get(frontier *head, int index);
int delete(frontier **head, unsigned int index);
int listlen(const frontier *h);
rgba **init_wall(char *file);
void patch(cell **grid, coords size, coords pos);
rgba **init_sprite(char *file);
void draw_sprite(char *file, SDL_instance instance);

#endif