/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

/* defines */

/* global varaiables */
int *mblock;
int freeSpace;

struct Node
{
	void *startAddr;		 // starting address of the memory block
	size_t length;			 // size of memory block
	int status;			 // 0 for free 1 for allocated
	struct Node *next, prev; // pointers to next and previous nodes
}

struct Node *head;

/* Functions */


// add to the end of list
void append(size_t size, void *startAddr)
{
	Node *cursor = head;
	while (cursor->next != NULL)
	{
		cursor = cursor->next;
	}

	Node *newNode;
	cursor->next = newNode;
	newNode->prev = cursor;

	for (int m = 1; m <= 4; m++)
	{
		if ((size + m) % 4 == 0)
		{
			newNode->length = size + m;
		}
	}
	newNode->startAddr = startAddr;
	newNode->next = NULL;
	newNode->status = 1;
}

// round up size to multiple of 4
size_t makeSize(size_t size)
{
	if (size <= 0)
	{
		return -1;
	}

	for (int i = 0; i <= 3; i++)
	{
		if ((size + i) % 4 == 0)
		{
			return size + i;
		}
	}
}

bool isTail(Node *node)
{
	if (node->next != NULL)
	{
		return 0;
	} else return 1;
}

void merge(Node *n1, Node *n2)
{

}

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	if (size < 1)
	{
		printf("Invalid memory size!\n");
		return -1;
	}

	mblock = malloc(size);

	// assign head node starting address
	head.startAddr = &mblock;
	head.next = NULL;
	head.prev = NULL;
	head.status = 0;
	head.length = 0;

	freeSpace = size;
	return 0;
}

int worst_fit_memory_init(size_t size)
{
	if (size < 1)
	{
		return -1;
	}
	mblock = malloc(size);
	// track memory usage
	useCount = 0;
	return 0;
}

/* memory allocators */
// size = number of bytes requested from the allocator
void *best_fit_alloc(size_t size)
{
	size_t newSize = makeSize(size);

	if (newSize == -1 || newSize > sizeof(freeSpace))
	{
		printf("Invalid argument or not enough memory!");
		return NULL;
	}
	// first-time allocation
	if (isTail(head))
	{
		head->length = newSize;
		return head->startAddr;
	}

	Node *smallest, *lastNode;
	smallest->length = sizeof(mblock);
	bool smallestFound;
	Node *cursor = head;

	// iterate through the list to find the smallest free space
	while (!isTail(cursor))
	{
		if (cursor->status == 0 && cursor->length <= smallest->length)
		{
			// update smallest
			smallest = cursor;
			smallestFound = true;
		} else if (cursor->status == 1)
		{
			// update last allocated node for future use
			lastNode = cursor;
		}
		cursor = cursor->next;
	}

	// if smallest not found (no block with enough size before tail)
	if (!smallestFound)
	{
		append(newSize, lastNode->startAddr + lastNode->length)
	} else
	{
		// found smallest node: smallest
		smallest->status = 1;

	}

	return smallest->startAddr;
}

void *worst_fit_alloc(size_t size)
{
	// To be completed by students
	if (size <= 0 || size >= sizeof(freeSpace))
	{
		printf("Invalid argument!");
		return NULL;
	}
	return startMem;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr)
{
	Node *cursor = head;
	while (!isTail(cursor))
	{
		// when found the node containing ptr
		if (cursor->startAddr == ptr && cursor->status == 1)
		{
			// mark the block free
			cursor->status = 0;
			// increase free space
			freeSpace += cursor->length;

			// coalescenting
			if (prev->status == 0 && next->status == 0)
			{
				merge(all 3 nodes);
			} else if (prev->status == 0)
			{
				merge(prev, cursor);
			} else if (next->status == 0)
			{
				merge(next, cursor);
			}
			break;
		}
		cursor = cursor->next;
	}
	return;
}

void worst_fit_dealloc(void *ptr)
{

	// To be completed by students
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */
int best_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}
