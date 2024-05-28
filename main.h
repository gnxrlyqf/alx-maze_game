#ifndef MAIN_H
#define MAIN_H
#include <SDL2/SDL.h>

/**
 * struct SDL_instance - SDL instance struct
 * @window: window
 * @renderer: renderer
*/
typedef struct SDL_instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_instance;

/**
 * struct coords - x/y pair of ints
 * @x: x
 * @y: y
*/
typedef struct coords
{
	int x, y;
} coords;

/**
 * struct coordsf - x/y pair of floats
 * @x: x
 * @y: y
*/
typedef struct coordsf
{
	float x, y;
} coordsf;

/**
 * struct cell - grid cell
 * @pos: cell position
 * @size: cell size
 * @state: cell state (0: passage, 1: wall)
*/
typedef struct cell
{
	coords pos;
	int size;
	int state;
} cell;

/**
 * struct ray - ray struct for raycasting
 * @pos: ray position
 * @val: ray length
 * @theta: ray angle
 * @dir: ray direction (0: horizontal, 1: vertical)
*/
typedef struct ray
{
	coordsf pos;
	float val, theta, dir;
} ray;

/**
 * struct frontier - double linked list for maze generation algorithm
 * @cell: call insode the node
 * @prev: pointer to the previous node
 * @next: pointer to the next node
*/
typedef struct frontier
{
	coords cell;
	struct frontier *prev, *next;
} frontier;

/**
 * struct rgba - pixel struct
 * @r: red value
 * @g: green value
 * @b: blue value
 * @a: alpha value
*/
typedef struct rgba
{
	int r, g, b, a;
} rgba;

/**
 * struct entity - struct representing game entity
 * @behavior: entity behavior
 * 		0: prop
 * 		1: item
 * 		2: patrol
 * 		3: enemy
 * 		4: boss
 * @pos: entity position
 * @z: elevation
*/
typedef struct entity
{
	int behavior;
	coordsf pos;
	float z;
} entity;

/**
 * struct sprite - struct representing entity sprite
 * @rect: rectangle for drawing sprite
 * @t: texture of sprite
 * @dist: distance from player
 * @scale: scaling factor
*/
typedef struct sprite
{
	SDL_Rect rect;
	SDL_Texture *t;
	float dist, scale;
} sprite;

/**
 * struct column - vertical line for drawing walls
 * @pos: line position
 * @line: line height
 * @s: line shade factor
 * @f: line fog factor
 * @dist: distance from player
 * @x: texture x index
 * @texture: texture file name
*/
typedef struct column
{
	coords pos;
	float line, s, f, dist;
	int x;
	char *texture;
} column;

/**
 * struct player - player struct
 * @pos: player position
 * @theta: player angle
 * @d: position offset
*/
typedef struct player
{
	coordsf pos;
	float theta;
	coordsf d;
} player;

int instantiate(SDL_instance *instance, coords dim, char *name, coords pos);
void draw_map(SDL_Renderer *m, cell **g, coords d, int s, coordsf p, ray *r);
cell **init_grid(coords dimensions, int cellSize);
int events(cell **grid, int size);
void free_grid(cell **grid, int rows);
void controls(cell **grid, player *p);
void game(cell **grid, SDL_Renderer *m, SDL_Renderer *d, coords d1, coords d2);
ray *raycast(int size, coords dim, cell **grid, ray *rays, player p);
float distance(coordsf a, coordsf b);
ray horizontal(float rtheta, int size, coords dim, cell **grid, coordsf pos);
ray vertical(float rtheta, int size, coords dim, cell **grid, coordsf pos);
int ftoi(float x);
column *process_rays(ray *rays, int size, float theta);
void render(SDL_Renderer *display, column *walls, sprite *sprites, coords res);
void maze(cell **grid, coords first, coords range);
frontier *add(frontier **head, coords new);
frontier *get(frontier *head, int index);
int delete(frontier **head, unsigned int index);
int listlen(const frontier *h);
rgba **init_texture(char *file);
void patch(cell **grid, coords size, coords pos);
sprite *process_sprites(SDL_Renderer *r, entity *e, int size, player p);

void quick_sort_sprite(sprite *arr, int low, int high);
int partition_sprite(sprite *arr, int low, int high);
void swap_sprite(sprite *a, sprite *b);

void quick_sort_wall(column *arr, int low, int high);
int partition_wall(column *arr, int low, int high);
void swap_wall(column *a, column *b);

#endif
