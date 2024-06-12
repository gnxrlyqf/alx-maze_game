#include "main.h"
#include <stdlib.h>
#include <time.h>

/**
 * spawn_entities - spawns key entities in random positions
 * @grid: two dimensional array of cells representing grid
 *
 * Return: array of entities
*/
entity *spawn_entities(cell **grid)
{
	vector2 offsets[4] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
	vector2 r;
	int i, j, count = 20, sides;
	entity *entities = malloc(sizeof(entity) * count);

	srand(time(NULL));
	for (i = 0; i < count; i++)
	{
		while (entities[i].exists != 1)
		{
			sides = 0;
			r.x = (rand() % 31) + 1;
			r.y = (rand() % ((33 + 1) - 3) + 3);
			if (grid[r.x][r.y].state == 0 && grid[r.x][r.y].valid == 1)
			{
				grid[r.x][r.y].valid = 0;
				for (j = 0; j < 4; j++)
				{
					if (grid[r.x + offsets[j].x][r.y + offsets[j].y].state == 1)
						sides++;
				}
				if (sides >= 3)
				{
					entities[i].behavior = 1;
					entities[i].pos.x = (r.x * 16) + 8;
					entities[i].pos.y = (r.y * 16) + 8;
					entities[i].exists = 1;
				}
			}
		}
	}
	return (entities);
}

/**
 * check_entities - checks if player picks up a key
 * @p: player struct
 * @keys: pointer to array of entities
 * @size: size of array
 *
 * Return: 1 (picked up) 0 (not picked up)
*/
int check_entities(player *p, entity **keys, int size)
{
	int i;
	vector2 pos = {(int)((*p).pos.x / 16), (int)((*p).pos.y / 16)};
	vector2 key;

	for (i = 0; i < size; i++)
	{
		key.x = (int)((*keys)[i].pos.x / 16);
		key.y = (int)((*keys)[i].pos.y / 16);
		if ((*keys)[i].exists == 1 && key.x == pos.x && key.y == pos.y)
		{
			(*keys)[i].exists = 0;
			(*p).keys++;
			return (1);
		}
	}
	return (0);
}
