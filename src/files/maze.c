#include "main.h"
#include <time.h>

/**
 * maze - creates a maze in the grid using Prim's algorithm
 * @grid: two dimensional array of cells representing grid
 * @first: starting cell of the algorithm
 * @range: maximum position to traverse
*/
void maze(cell **grid, vector2 first, vector2 range)
{
	int i, index;
	vector2 f, offset[4] = {{2, 0}, {-2, 0}, {0, 2}, {0, -2}}, chosen;
	vector2 size = {3, 3}, posstart = {1, 1}, posend = {29, 33};
	frontier *head = malloc(sizeof(frontier));

	head->cell = first, head->next = NULL, head->prev = NULL;
	while (head)
	{
		srand(time(NULL));
		index = rand() % listlen(head);
		chosen = get(head, index)->cell;
		for (i = 0; i < 4; i++)
		{
			f.x = chosen.x + offset[i].x, f.y = chosen.y + offset[i].y;
			if (f.x >= 0 && f.y >= 2 &&
				f.x < range.x && f.y < range.y - 2 &&
				grid[f.x][f.y].state == 1) {
				grid[f.x][f.y].state = 0, grid[f.x][f.y].valid = 1;
				grid[f.x - (offset[i].x / 2)][f.y - (offset[i].y / 2)].state = 0;
				add(&head, f);
			}
		}
		delete(&head, index);
	}
	patch(grid, size, posstart);
	patch(grid, size, posend);
}

/**
 * add - adds a frontier node to the linked list
 * @head: double pointer to the head node
 * @new: data of the node to add
 *
 * Return: pointer to the newly added node
*/
frontier *add(frontier **head, vector2 new)
{
	frontier *node, *last;

	node = malloc(sizeof(frontier));
	if (node == NULL)
		return (NULL);
	node->cell = new;
	node->next = NULL;
	last = *head;
	if (*head == NULL)
		node->next = *head, *head = node;
	else
	{
		while (last->next)
			last = last->next;
		last->next = node;
	}
	node->prev = last;

	return (node);
}

/**
 * get - fetches a node based on index
 * @head: pointer to the head node
 * @index: index of the node to fetch
 *
 * Return: pointer to the node
*/
frontier *get(frontier *head, int index)
{
	int i = 0;

	if (!head)
		return (NULL);
	while (head)
	{
		if (i == index)
			break;
		i++;
		head = head->next;
	}
	return (head);
}

/**
 * delete - deletes a node from a linked list
 * @head: double pointer to the head node
 * @index: index of the node to delete
 *
 * Return: 0 (Success) 1 (Failure)
*/
int delete(frontier **head, unsigned int index)
{
	frontier *head1;
	frontier *head2;
	unsigned int count;

	head1 = *head;

	if (head1 != NULL)
		while (head1->prev != NULL)
			head1 = head1->prev;
	count = 0;

	while (head1 != NULL)
	{
		if (count == index)
		{
			if (count == 0)
			{
				*head = head1->next;
				if (*head != NULL)
					(*head)->prev = NULL;
			}
			else
			{
				head2->next = head1->next;

				if (head1->next != NULL)
					head1->next->prev = head2;
			}
			free(head1);
			return (1);
		}
		head2 = head1;
		head1 = head1->next;
		count++;
	}
	return (-1);
}

/**
 * listlen - calculates the length of a linked list
 * @h: head node of the linked list
 *
 * Return: length of the linked list
*/
int listlen(const frontier *h)
{
	int elements = 0;
	frontier *current = (frontier *)h;

	if (!current)
		return (0);
	while (current)
	{
		elements++;
		current = current->next;
	}
	return (elements);
}
