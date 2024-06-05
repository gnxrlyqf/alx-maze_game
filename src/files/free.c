#include "main.h"

/**
 * free_grid - frees a grid
 * @grid: grid to free
 * @rows: row count
*/
void free_grid(cell **grid, int rows)
{
	int i;

	for (i = 0; i < rows; i++)
		free(grid[i]);
	free(grid);
}

/**
 * free_rgba - frees a two dimensional array of pixels
 * @arr: array to free
 * @size: array row size
*/
void free_rgba(rgba **arr, int size)
{
	int i;

	for (i = 0; i < size; i++)
		free(arr[i]);
	free(arr);
}

/**
 * free_sprites - frees an array of sprites
 * @sprites: array to free
 * @size: size of array
*/
void free_sprites(sprite *sprites, int size)
{
	int i;

	for (i = 0; i < size; i++)
		SDL_DestroyTexture(sprites[i].t);
	free(sprites);
}

/**
 * free_texture - frees array of sdl textures
 * @texture: array to pointers of textures
 * @count:size of array
*/
void free_texture(SDL_Texture **texture, int count)
{
	int i;

	for (i = 0; i < count; i++)
		SDL_DestroyTexture(texture[i]);
	free(texture);
}

/**
 * free_all - frees all allocated memory
 * @rays: array of rays
 * @walls: array of columns
 * @entities: array of entities
 * @sprites: array of sprites
 * @texture: two dimensional array of pixels
 * @grid: two dimensional array of cells representing grid
 * @counter: array of textures representing key counter
 * @cards: array of textures representing text cards
*/
void free_all(ray *rays, column *walls, entity *entities, sprite *sprites,
rgba **texture, cell **grid, SDL_Texture **counter, SDL_Texture **cards) {
	free(rays);
	free(walls);
	free(entities);
	free_sprites(sprites, 20);
	free_rgba(texture, 32);
	free_grid(grid, 33);
	free_texture(counter, 21);
	free_texture(cards, 4);
}
