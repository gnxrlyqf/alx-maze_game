#include "main.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(void)
{
	SDL_instance map, display;
	int size = 16, i, thickness = 1;
	coords dimensions1 = {33 * size, 55 * size};
	coords pos1 = {100, 50};
	coords dimensions2 = {1280, 720};
	coords pos2 = {1000, 100};
	cell **grid = init_grid(dimensions1, size);
	coordsf *rays = malloc(sizeof(coordsf) * 1280);
	SDL_Rect sky = {0, 0, 1280, 360};

	if (instantiate(&map, dimensions1, "map", pos1) != 0)
		return (1);
	if (instantiate(&display, dimensions2, "display", pos2) != 0)
		return (1);
	while (1)
	{
		SDL_SetRenderDrawColor(map.renderer, 0, 0, 0, 0), SDL_SetRenderDrawColor(display.renderer, 0, 153, 0, 0);
		SDL_RenderClear(map.renderer), SDL_RenderClear(display.renderer);
		SDL_SetRenderDrawColor(display.renderer, 216, 217, 218, 255);
		SDL_RenderFillRect(display.renderer, &sky);
		draw_player(map, grid);
		rays = raytracing(map, size, dimensions1, grid, rays);
		draw_walls(display, rays, size, thickness, dimensions2);
		draw_sprite("sprite", display);
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