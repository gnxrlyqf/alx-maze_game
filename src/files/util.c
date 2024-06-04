#include "main.h"
#include <string.h>

/**
 * distance - calculates distance between two points
 * @a: point a
 * @b: point b
 *
 * Return: distance between two points
*/
float distance(coordsf a, coordsf b)
{
	return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

/**
 * distancei - calculates distance between two points (float)
 * @a: point a
 * @b: point b
 *
 * Return: distance between two points
*/

float distancei(coords a, coords b)
{
	return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}


/**
 * ftoi - rounds a float into an int
 * @x: float to round
 *
 * Return: rounded int
*/
int ftoi(float x)
{
	int int_part = (int)x;
	float decimal_part = x - int_part;

	if (decimal_part >= 0.5)
		return (ceil(x));
	else
		return (floor(x));
}

/**
 * patch - creates empty patch in grid
 * @grid: two dimensional array representing grid
 * @size: cell size
 * @pos: position to put the patch
*/
void patch(cell **grid, coords size, coords pos)
{
	int i, j;

	for (i = pos.x; i < pos.x + size.x; i++)
	{
		for (j = pos.y; j < pos.y + size.y; j++)
		{
			grid[i][j].state = 0;
		}
	}
}

/**
 * concatenate - concatenates three strings
 * @a: string a
 * @b: string b
 * @c: string c
 *
 * Return: concatenation of the three strings
*/
char *concatenate(char *a, char *b, char *c)
{
	int size = strlen(a) + strlen(b) + strlen(c) + 1;
	char *str = malloc(size);

	strcpy(str, a);
	strcat(str, b);
	strcat(str, c);
	return (str);
}
