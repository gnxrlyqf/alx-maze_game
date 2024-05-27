#include "main.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(void)
{
	SDL_instance map, display;
	int size = 16, i, thickness = 1;
	coords dimensions1 = {33 * size, 55 * size};
	coords pos1 = {100, 25};
	coords dimensions2 = {1260, 720};
	coords pos2 = {1000, 100};
	cell **grid = init_grid(dimensions1, size);
	ray *rays = malloc(sizeof(ray) * 1260);
	// SDL_Rect **walls = malloc()
	SDL_Rect floor = {0, 360, 1260, 360};
	SDL_Rect sky = {0, 0, 1260, 360};
	entity *entities = malloc(sizeof(entity) * 2);
	column *walls = malloc(sizeof(column) * 1260);
	sprite *sprites = malloc(sizeof(sprite) * 2);

	entities[0].pos.x = 15 * 16 + 8, entities[1].pos.x = 17 * 16 + 8;
	entities[0].pos.y = 5 * 16 + 8, entities[1].pos.y = 2 * 16 + 8;
	if (instantiate(&map, dimensions1, "map", pos1) != 0)
		return (1);
	if (instantiate(&display, dimensions2, "display", pos2) != 0)
		return (1);
	SDL_SetRenderDrawBlendMode(display.renderer, SDL_BLENDMODE_BLEND);
	while (1)
	{
		SDL_SetRenderDrawColor(map.renderer, 255, 255, 255, 255), SDL_SetRenderDrawColor(display.renderer, 216, 217, 218, 0);
		SDL_RenderClear(map.renderer), SDL_RenderClear(display.renderer);
		SDL_SetRenderDrawColor(display.renderer, 0, 153, 0, 255);
		SDL_RenderFillRect(display.renderer, &floor);
		draw_player(map, grid);
		rays = raycasting(map.renderer, size, dimensions1, grid, rays);
		walls = process_rays(display, rays, size, dimensions2, walls);
		sprites = process_sprites(display.renderer, rays, entities, sprites);
		render(display.renderer, walls, sprites, dimensions2);
        draw_grid(map, grid, dimensions1, size);
        if (events(grid, size) == 1)
            break;
		SDL_RenderPresent(map.renderer), SDL_RenderPresent(display.renderer);
	}
	free_grid(grid, dimensions1.x / size);
	free(rays);
    SDL_DestroyRenderer(map.renderer), SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(map.window), SDL_DestroyWindow(display.window);
    SDL_Quit();
	return (0);
}