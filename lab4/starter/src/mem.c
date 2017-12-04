/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mem.h"

/* defines */

/* global varaiables */
size_t total_size;

typedef struct Node
{
	size_t length;			  // size of memory block
	int status;				  // 0 for free 1 for allocated
	struct Node *next, *prev; // pointers to next and previous nodes
} Node;

struct Node *head;

/* Functions */
// insert n2 after n1
void insert(Node *n1, Node *n2)
{
	// if n1 is at the end of list
	if (n1->next == NULL)
	{
		n1->next = n2;
		n2->next = NULL;
		n2->prev = n1;
	}
	else
	{
		n2->next = n1->next;
		n2->next->prev = n2;
		n1->next = n2;
		n2->prev = n1;
	}
}

// round up size to multiple of 4, taking size of Node into account
size_t makeSize(size_t size)
{
	size_t newSize;

	if (size <= 0)
	{
		return 0;
	}

	// if cannot be divided by 4
	if (size % 4 != 0)
	{
		size += (4 - (size % 4));
	}
	newSize = size + sizeof(Node);

	return newSize;
}

// check if node is tail of the linked list
bool isTail(Node *node)
{
	if (node->next != NULL)
	{
		return 0;
	}
	else
		return 1;
}

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	// should at least be able to contain one node + 1 byte
	if (size <= sizeof(Node) + 4)
	{
		printf("Invalid memory size!\n");
		return -1;
	}
	head = (Node *)malloc(size);

	total_size = size;

	// assign head node starting address
	head->next = NULL;
	head->prev = NULL;
	head->status = 0;
	head->length = size;

	return 0;
}

int worst_fit_memory_init(size_t size)
{
	// should at least be able to contain one node + 1 byte
	if (size <= sizeof(Node) + 4)
	{
		printf("Invalid memory size!\n");
		return -1;
	}

	head = (Node *)malloc(size);

	total_size = size;

	// assign head node starting address
	head->next = NULL;
	head->prev = NULL;
	head->status = 0;
	head->length = size;

	return 0;
}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	size_t newSize = makeSize(size);

	if (newSize == 0)
	{
		printf("Not enough memory!");
		return NULL;
	}

	Node *smallest = NULL;
	Node *newNode = NULL;
	Node *cursor = head;

	// iterate through the list to find the smallest free space
	while (cursor != NULL)
	{
		// if not allocated AND size is enough to hold newSize
		if (cursor->status == 0 && cursor->length >= newSize)
		{
			// update smallest(best) block
			if (smallest == NULL || cursor->length < smallest->length)
			{
				smallest = cursor;
			}
		}
		cursor = cursor->next;
	}

	// if smallest not found (no block with enough size before tail)
	if (smallest == NULL)
	{
		printf("Not enough memory!\n");
		return NULL;
	}

	// exact size or internal fragmentation
	if (smallest->length == newSize || smallest->length - newSize <= sizeof(Node))
	{
		// update status
		smallest->status = 1;
	}
	// else if the leftover space is enough to be useful for further allocation
	else
	{
		// init new block
		newNode = (Node *)((size_t)smallest + newSize);
		newNode->status = 0;
		newNode->length = smallest->length - newSize;

		// insert the new block to the linked list
		insert(smallest, newNode);

		// update smallest
		smallest->length = newSize;
		smallest->status = 1;
	}
	return smallest;
}

void *worst_fit_alloc(size_t size)
{
	size_t newSize = makeSize(size);

	if (newSize == 0)
	{
		printf("Not enough memory!");
		return NULL;
	}

	Node *largest = NULL;
	Node *newNode = NULL;
	Node *cursor = head;

	// iterate through the list to find the largest free space
	while (cursor != NULL)
	{
		// if not allocated AND size is enough to hold newSize
		if (cursor->status == 0 && cursor->length >= newSize)
		{
			// update largest(worst) block
			if (largest == NULL || cursor->length > largest->length)
			{
				largest = cursor;
			}
		}
		cursor = cursor->next;
	}

	// if largest not found (no block with enough size before tail)
	if (largest == NULL)
	{
		printf("Not enough memory!\n");
		return NULL;
	}

	// exact size or internal fragmentation
	if (largest->length == newSize || largest->length - newSize <= sizeof(Node))
	{
		// update status
		largest->status = 1;
	}
	// else if the leftover space is enough to be useful for further allocation
	else
	{
		// init new block
		newNode = (Node *)((size_t)largest + newSize);
		newNode->status = 0;
		newNode->length = largest->length - newSize;

		// insert the new block to the linked list
		insert(largest, newNode);

		// update largest
		largest->length = newSize;
		largest->status = 1;
	}
	return largest;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr)
{
	Node *cursor = head;
	Node *prev_node, *next_node;
	// iterate through all nodes
	while (cursor != NULL)
	{
		// when found the node containing ptr
		if (cursor == ptr)
		{
			if (cursor->status == 0)
			{
				printf("This memory is already free...\n");
				break;
			}
			else // status == 1
			{
				prev_node = cursor->prev;
				next_node = cursor->next;

				// de-allocation
				cursor->status = 0;

				// coalescenting
				// if not tail and next is free
				if (next_node != NULL)
				{
					if (next_node->status == 0 && next_node != NULL)
					{
						cursor->next = next_node->next;
						cursor->length += next_node->length;
						// if next is not tail
						if (!isTail(next_node))
						{
							next_node->next->prev = cursor;
						}
					}
				}
				// if not head and prev is free
				if (prev_node != NULL)
				{
					if (prev_node->status == 0 && prev_node != NULL)
					{
						prev_node->next = cursor->next;
						prev_node->length += cursor->length;
						// if current cursor is not tail
						if (!isTail(cursor))
						{
							cursor->next->prev = prev_node;
						}
						cursor = prev_node;
					}
				}
				/* This block for testing */
				// if (head->length == total_size)
				// {
				// 	printf("Deallocation finished, head size is: %ld.\n", head->length);
				// }
				return;
			}
		}
		cursor = cursor->next;
	}
	printf("Memory address not found!\n");
	return;
}

void worst_fit_dealloc(void *ptr)
{
	Node *cursor = head;
	Node *prev_node, *next_node;
	// iterate through all nodes
	while (cursor != NULL)
	{
		// when found the node containing ptr
		if (cursor == ptr)
		{
			if (cursor->status == 0)
			{
				printf("This memory is already free...\n");
				break;
			}
			else // status == 1
			{
				prev_node = cursor->prev;
				next_node = cursor->next;

				// de-allocation
				cursor->status = 0;

				// coalescenting
				// if not tail and next is free
				if (next_node != NULL)
				{
					if (next_node->status == 0 && next_node != NULL)
					{
						cursor->next = next_node->next;
						cursor->length += next_node->length;
						// if next is not tail
						if (!isTail(next_node))
						{
							next_node->next->prev = cursor;
						}
					}
				}
				// if not head and prev is free
				if (prev_node != NULL)
				{
					if (prev_node->status == 0 && prev_node != NULL)
					{
						//printf("merge with prev\n");
						prev_node->next = cursor->next;
						prev_node->length += cursor->length;
						// if current cursor is not tail
						if (!isTail(cursor))
						{
							cursor->next->prev = prev_node;
						}
						cursor = prev_node;
					}
				}
				/* This block for testing */
				// if (head->length == total_size)
				// {
				// 	printf("Deallocation finished, head size is: %ld.\n", head->length);
				// }
				return;
			}
		}
		cursor = cursor->next;
	}
	printf("Memory address not found!\n");
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */
int best_fit_count_extfrag(size_t size)
{
	Node *cursor = head;
	int count = 0;

	while (cursor != NULL)
	{
		// count free blocks of size less than the input size
		if (cursor->status == 0 && cursor->length < size)
		{
			count++;
		}
		cursor = cursor->next;
	}
	return count;
}

int worst_fit_count_extfrag(size_t size)
{
	Node *cursor = head;
	int count = 0;

	while (cursor != NULL)
	{
		if (cursor->status == 0 && cursor->length < size)
		{
			count++;
		}
		cursor = cursor->next;
	}
	return count;
}
