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

typedef struct player
{
	coordsf pos;
	float theta;
	coordsf d;
} player;

int instantiate(SDL_instance *instance, coords dimensions, char *name, coords pos);
void draw_map(SDL_Renderer *renderer, cell **grid, coords dimensions, int cellSize, coordsf pos, ray *rays);
cell **init_grid(coords dimensions, int cellSize);
int events(cell **grid, int size);
void free_grid(cell **grid, int rows);
void draw_player(SDL_Renderer *renderer, cell **grid, coordsf pos);
void poll_controls(cell **grid, player *p);
void game(cell **grid, SDL_Renderer *map, SDL_Renderer *display, coords dim1, coords dim2);
ray *raycast(SDL_Renderer *r, int size, coords dimensions, cell **grid, ray *rays, player p);
float distance(float ax ,float ay, float bx, float by);
ray horizontal(float rtheta, int size, coords dim, cell **grid, coordsf pos);
ray vertical(float rtheta, int size, coords dim, cell **grid, coordsf pos);
int ftoi(float x);
column *process_rays(ray *rays, int size, float theta);
void render(SDL_Renderer *map, SDL_Renderer *display, column *walls, sprite *sprites, coords res);
void maze(cell **grid, coords first, coords range);
frontier *add(frontier **head, coords new);
frontier *get(frontier *head, int index);
int delete(frontier **head, unsigned int index);
int listlen(const frontier *h);
rgba **init_texture(char *file, int type);
void patch(cell **grid, coords size, coords pos);
sprite *process_sprites(SDL_Renderer *renderer, entity *entities, int size, player p);

void quick_sort_sprite(sprite *arr, int low, int high);
int partition_sprite(sprite *arr, int low, int high);
void swap_sprite(sprite* a, sprite* b);

void quick_sort_wall(column *arr, int low, int high);
int partition_wall(column *arr, int low, int high);
void swap_wall(column* a, column* b);


#endif