#include "main.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

/**
 * main - entry point of program
 *
 * Return: Always 0
*/
int main(void)
{
	SDL_instance map, display;
	int size = 16, i, gamestate = 0;
	coords dimensions1 = {33 * size, 37 * size};
	coords pos1 = {100, 150};
	coords dimensions2 = {1260, 720};
	coords pos2 = {1000, 100};

	if (instantiate(&map, dimensions1, "map", pos1) != 0)
		return (1);
	if (instantiate(&display, dimensions2, "display", pos2) != 0)
		return (1);
	SDL_SetRenderDrawBlendMode(display.renderer, SDL_BLENDMODE_BLEND);
	while (1)
	{
		gamestate = menu(display.renderer);
		if (gamestate == 1)
			game(map.renderer, display.renderer, dimensions1, dimensions2);
		if (gamestate == 0)
			break;
	}
	SDL_DestroyRenderer(map.renderer), SDL_DestroyRenderer(display.renderer);
	SDL_DestroyWindow(map.window), SDL_DestroyWindow(display.window);
	SDL_Quit();
	return (0);

}
