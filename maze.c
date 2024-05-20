#include "main.h"
#include <time.h>

void maze(cell **grid, coords first, coords range)
{
	int i, index;
	coords focal, offset[4] = {{2, 0}, {-2, 0}, {0, 2}, {0, -2}}, chosen;
	frontier *head = malloc(sizeof(frontier)), *current;
	head->cell = first, head->next = NULL, head->prev = NULL;
	while(head)
	{
		srand(time(NULL));
		index = rand() % listlen(head);
		chosen = get(head, index)->cell;
		for (i = 0; i < 4; i++)
		{
			focal.x = chosen.x + offset[i].x, focal.y = chosen.y + offset[i].y;
			if (focal.x >= 0 && focal.y >= 5 && focal.x < range.x && focal.y < range.y - 5 && grid[focal.x][focal.y].state == 1)
			{
				grid[focal.x][focal.y].state = 0;
				grid[focal.x - (offset[i].x / 2)][focal.y - (offset[i].y / 2)].state = 0;
				add(&head, focal);
			}
		}
		delete(&head, index);
	}
	coords size = {7, 4}, posstart = {13, 1}, posend = {13, 50};
	patch(grid, size, posstart);
	patch(grid, size, posend);

}

frontier *add(frontier **head, coords new)
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
