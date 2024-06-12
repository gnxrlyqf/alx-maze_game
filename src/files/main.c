#include "main.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

/**
 * main - entry point of program
 * @argc: argument count (length of the argument array)
 * @argv: argument vector (string array holding arguments)
 * Return: Always 0
*/
int main(int argc, char **argv)
{
	SDL_instance map, display;
	int size = 16, gamestate = 0, arg;
	vector2 dimensions1 = {33 * size, 37 * size}, dimensions2 = {1260, 720};
	vector2 pos1 = {100, 150};
	vector2 pos2 = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};

	map.renderer = NULL;
	if (argc > 2)
		printf("Error: Too many arguments\n"), exit(99);
	if (argc == 2)
	{
		arg = SDL_strcmp(argv[1], "exp");
		if (arg > 0 || arg < 0)
			printf("Error: Unknown argument\n"), exit(100);
		else if (arg == 0)
		{
			if (init_window(&map, dimensions1, "map", pos1) != 0)
				return (1);
			pos2.x = 1000, pos2.y = 100;
		}
	}
	if (init_window(&display, dimensions2, "display", pos2) != 0)
		return (1);
	SDL_SetRenderDrawBlendMode(display.renderer, SDL_BLENDMODE_BLEND);
	while (1)
	{
		gamestate = title(display.renderer);
		if (gamestate == 1)
			game(map.renderer, display.renderer, dimensions1);
		if (gamestate == 0)
			break;
	}
	SDL_DestroyRenderer(map.renderer), SDL_DestroyRenderer(display.renderer);
	SDL_DestroyWindow(map.window), SDL_DestroyWindow(display.window);
	SDL_Quit();
	return (0);
}
