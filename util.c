#include "main.h"

float distance(float ax, float ay, float bx, float by)
{
	return (sqrt(pow(bx - ax, 2) + pow(by - ay, 2)));
}

int ftoi(float x)
{
	int int_part = (int)x;
	float decimal_part = x - int_part;

	if (decimal_part >= 0.5)
		return ceil(x);
	else
		return floor(x);
}

void quick_sort_sprite(sprite *arr, int low, int high) 
{ 
	if (low < high)
	{
		int partitionIndex = partition_sprite(arr, low, high); 
		quick_sort_sprite(arr, low, partitionIndex - 1); 
		quick_sort_sprite(arr, partitionIndex + 1, high); 
	} 
}

void quick_sort_wall(column *arr, int low, int high) 
{ 
	if (low < high)
	{
		int partitionIndex = partition_wall(arr, low, high); 
		quick_sort_wall(arr, low, partitionIndex - 1); 
		quick_sort_wall(arr, partitionIndex + 1, high); 
	} 
} 

int partition_sprite(sprite *arr, int low, int high) 
{
	sprite pivot = arr[low]; 
	int i = low; 
	int j = high; 

	while (i < j) { 
		while (arr[i].dist <= pivot.dist && i <= high - 1) { 
			i++; 
		} 
		while (arr[j].dist > pivot.dist && j >= low + 1) { 
			j--; 
		} 
		if (i < j) { 
			swap_sprite(&arr[i], &arr[j]); 
		} 
	} 
	swap_sprite(&arr[low], &arr[j]); 
	return j; 
}

int partition_wall(column *arr, int low, int high) 
{
	column pivot = arr[low]; 
	int i = low; 
	int j = high; 

	while (i < j) { 
		while (arr[i].dist >= pivot.dist && i <= high - 1) { 
			i++; 
		} 
		while (arr[j].dist < pivot.dist && j >= low + 1) { 
			j--; 
		} 
		if (i < j) { 
			swap_wall(&arr[i], &arr[j]); 
		} 
	} 
	swap_wall(&arr[low], &arr[j]); 
	return j; 
} 

void swap_sprite(sprite *a, sprite *b) 
{ 
	sprite temp = *a; 
	*a = *b; 
	*b = temp; 
}

void swap_wall(column *a, column *b) 
{ 
	column temp = *a; 
	*a = *b; 
	*b = temp; 
}