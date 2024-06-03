#include "main.h"

/**
 * quick_sort_wall - sorts an array of columns using quick sort
 * @arr: array to sort
 * @low: start index of partition
 * @high: end index of partition
*/
void quick_sort_wall(column *arr, int low, int high)
{
	int partitionIndex;

	if (low < high)
	{
		partitionIndex = partition_wall(arr, low, high);
		quick_sort_wall(arr, low, partitionIndex - 1);
		quick_sort_wall(arr, partitionIndex + 1, high);
	}
}

/**
 * partition_wall - partitions array of columns
 * @arr: array to sort
 * @low: start index of partition
 * @high: end index of partition
 *
 * Return: index of swapped element
*/
int partition_wall(column *arr, int low, int high)
{
	column pivot = arr[low];
	int i = low, j = high;

	while (i < j)
	{
		while (arr[i].dist >= pivot.dist && i <= high - 1)
			i++;
		while (arr[j].dist < pivot.dist && j >= low + 1)
			j--;
		if (i < j)
			swap_wall(&arr[i], &arr[j]);
	}
	swap_wall(&arr[low], &arr[j]);
	return (j);
}
/**
 * swap_wall - swaps two elements in column array
 * @a: element a
 * @b: element b
*/
void swap_wall(column *a, column *b)
{
	column temp = *a;
	*a = *b;
	*b = temp;
}
