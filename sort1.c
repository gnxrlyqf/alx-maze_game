#include "main.h"

/**
 * quick_sort_sprite - sorts an array of sprites using quick sort
 * @arr: array to sort
 * @low: start index of partition
 * @high: end index of partition
*/
void quick_sort_sprite(sprite *arr, int low, int high)
{
	int partitionIndex;

	if (low < high)
	{
		partitionIndex = partition_sprite(arr, low, high);
		quick_sort_sprite(arr, low, partitionIndex - 1);
		quick_sort_sprite(arr, partitionIndex + 1, high);
	}
}

/**
 * partition_sprite - partitions array of sprites
 * @arr: array to sort
 * @low: start index of partition
 * @high: end index of partition
 *
 * Return: index of swapped element
*/
int partition_sprite(sprite *arr, int low, int high)
{
	sprite pivot = arr[low];
	int i = low, j = high;

	while (i < j)
	{
		while (arr[i].dist <= pivot.dist && i <= high - 1)
			i++;
		while (arr[j].dist > pivot.dist && j >= low + 1)
			j--;
		if (i < j)
			swap_sprite(&arr[i], &arr[j]);
	}
	swap_sprite(&arr[low], &arr[j]);
	return (j);
}

/**
 * swap_sprite - swaps two elements in sprite array
 * @a: element a
 * @b: element b
*/
void swap_sprite(sprite *a, sprite *b)
{
	sprite temp = *a;
	*a = *b;
	*b = temp;
}
