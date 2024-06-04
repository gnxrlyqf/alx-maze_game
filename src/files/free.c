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
