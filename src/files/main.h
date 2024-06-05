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
 * @valid: validity of an entitiy being placed in the cell
*/
typedef struct cell
{
	coords pos;
	int size;
	int state;
	int valid;
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
 * struct sprite - struct representing entity sprite
 * @rect: rectangle for drawing sprite
 * @t: texture of sprite
 * @dist: distance from player
 * @scale: scaling factor
 * @visible: boolean for sprite visibility
*/
typedef struct sprite
{
	SDL_Rect rect;
	SDL_Texture *t;
	float dist, scale;
	int visible;
} sprite;

/**
 * struct entity - struct representing game entity
 * @behavior: entity behavior
 * 0: doesn't exist
 * 1: item
 * 2: enemy
 * 3: boss
 * @pos: entity position
 * @z: elevation
 * @exists: boolean for entity existence
*/
typedef struct entity
{
	int behavior, exists;
	coordsf pos;
	float z;
} entity;

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
 * @keys: amount of keys collected
*/
typedef struct player
{
	coordsf pos;
	float theta;
	coordsf d;
	int keys;
} player;

int init_window(SDL_instance *instance, coords dim, char *name, coords pos);
void draw_map(SDL_Renderer *m, cell **g, coords d, int s, coordsf p, ray *r,
int c, entity *entities);
cell **init_grid(coords dimensions, int cellSize);
int events(void);
void free_grid(cell **grid, int rows);
void controls(cell **grid, player *p);
void game(SDL_Renderer *m, SDL_Renderer *d, coords d1);
ray *raycast(int size, coords dim, cell **grid, ray *rays, player p);
float distance(coordsf a, coordsf b);
ray horizontal(float rtheta, int size, coords dim, cell **grid, coordsf pos);
ray vertical(float rtheta, int size, coords dim, cell **grid, coordsf pos);
int ftoi(float x);
void process_rays(ray *rays, column **walls, int size, float theta);
void render(SDL_Renderer *display, column *w, sprite *s, int c,
rgba **texture);
void maze(cell **grid, coords first, coords range);
frontier *add(frontier **head, coords new);
frontier *get(frontier *head, int index);
int delete(frontier **head, unsigned int index);
int listlen(const frontier *h);
rgba **init_texture(char *file);
void patch(cell **grid, coords size, coords pos);
sprite *process_sprites(entity *e, sprite *s, int c, player p);
entity *spawn_entities(cell **grid);
float distancei(coords a, coords b);
int check_entities(player *p, entity **keys, int size);
int title(SDL_Renderer *display);
void free_rgba(rgba **arr, int size);
void free_sprites(sprite *sprites, int size);
char *concatenate(char *a, char *b, char *c);
SDL_Texture **init_counter(SDL_Renderer *renderer, int count);
void free_texture(SDL_Texture **counter, int count);
SDL_Texture **init_cards(SDL_Renderer *r);
void free_all(ray *rays, column *walls, entity *entities, sprite *sprites,
rgba **texture, cell **grid, SDL_Texture **counter, SDL_Texture **cards);
int cards_events(SDL_Renderer *d, player p, SDL_Texture **cards,
int t1, int *t, SDL_Texture **card);

void quick_sort_sprite(sprite *arr, int low, int high);
int partition_sprite(sprite *arr, int low, int high);
void swap_sprite(sprite *a, sprite *b);

void quick_sort_wall(column *arr, int low, int high);
int partition_wall(column *arr, int low, int high);
void swap_wall(column *a, column *b);

/**
 * enum cards - iterator type for text card textures
 * @FOUND: index for "found" card
 * @ALL: index for "all" card
 * @NEED: index for "need" card
 * @COMPLETE: index for "complete" card
 * @OBJECTIVE: index for "objective" card
 * @QUIT: index for "quit" card
*/
enum cards
{
	FOUND,
	ALL,
	NEED,
	COMPLETE,
	OBJECTIVE,
	QUIT
};

#endif
