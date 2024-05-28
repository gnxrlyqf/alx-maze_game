#ifndef MAIN_H
#define MAIN_H
#include <SDL2/SDL.h>

typedef struct SDL_instance
{
    SDL_Window *window;
    SDL_Renderer *renderer;
} SDL_instance;

typedef struct coords
{
	int x, y;
} coords;

typedef struct coordsf
{
	float x, y;
} coordsf;

typedef struct cell
{
    coords pos;
    int size;
    int state;
} cell;

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

typedef struct entity
{
	int behavior;
	coordsf pos;
	float z, theta;
} entity;

typedef struct sprite
{
	SDL_Rect rect;
	SDL_Texture *texture;
	float dist, scale;
} sprite;

typedef struct column
{
	coords pos;
	float line, s, f, dist;
	int x;
	char *texture;
} column;

typedef enum {
	SPRITE,
	COLUMN
} TYPE;

int instantiate(SDL_instance *instance, coords dimensions, char *name, coords pos);
void draw_grid(SDL_instance instance, cell **grid, coords dimensions, int cellSize);
cell **init_grid(coords dimensions, int cellSize);
int events(cell **grid, int size);
void free_grid(cell **grid, int rows);
void draw_player(SDL_instance instance, cell **grid);
void poll_controls(cell **grid);
ray *raycasting(SDL_Renderer *renderer, int size, coords dimensions, cell **grid, ray *rays);
float distance(float ax ,float ay, float bx, float by);
ray horizontal(float raytau, int size, coords dim, cell **grid);
ray vertical(float raytau, int size, coords dim, cell **grid);
int ftoi(float x);
column *process_rays(SDL_instance instance, ray *rays, int size, coords res, column *walls);
void render(SDL_Renderer *renderer, column *walls, sprite *sprites, coords res);
void maze(cell **grid, coords first, coords range);
frontier *add(frontier **head, coords new);
frontier *get(frontier *head, int index);
int delete(frontier **head, unsigned int index);
int listlen(const frontier *h);
rgba **init_texture(char *file, int type);
void patch(cell **grid, coords size, coords pos);
sprite *process_sprites(SDL_Renderer *renderer, ray *rays, entity *entities, sprite *sprites);

void quick_sort_sprite(sprite *arr, int low, int high);
int partition_sprite(sprite *arr, int low, int high);
void swap_sprite(sprite* a, sprite* b);

void quick_sort_wall(column *arr, int low, int high);
int partition_wall(column *arr, int low, int high);
void swap_wall(column* a, column* b);


#endif