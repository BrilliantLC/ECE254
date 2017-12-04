/**
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"

int main(int argc, char *argv[])
{
	int num = 0;
	int algo = 0; // default algorithm to test is best fit
	int test = 0; // 0 or 1, 2 test cases
	void *p2, *p4_1, *p4_2, *p8_1, *p8_2, *p16, *p32, *p64_1, *p64_2, *p256_1;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <0/1>. 0 for best fit and 1 for worst fit \n", argv[0]);
		exit(1);
	}
	else if (!strcmp(argv[1], "1") || !strcmp(argv[1], "0"))
	{
		algo = atoi(argv[1]);
		test = atoi(argv[2]);
	}
	else
	{
		fprintf(stderr, "Invalid argument, please specify 0 or 1\n");
		exit(1);
	}

	if (algo == 0)
	{
		best_fit_memory_init(1024); // initizae 1KB, best fit
		if (test == 0)				// 12 operations
		{
			printf("Start test...\n\n");
			p8_1 = best_fit_alloc(8); // allocate 8B
			printf("best fit8: p=%p\n", p8_1);

			p16 = best_fit_alloc(16); // allocate 16B
			printf("best fit16: p=%p\n", p16);

			p32 = best_fit_alloc(32); // allocate 32B
			printf("best fit32: p=%p\n", p32);

			p64_1 = best_fit_alloc(64);
			printf("best fit64: p=%p\n", p64_1);

			if (p16 != NULL) // deallocate 16B
			{
				best_fit_dealloc(p16);
				printf("deallocated16\n");
			}
			if (p32 != NULL) // deallocate 32B
			{
				best_fit_dealloc(p32);
				printf("deallocated32\n");
			}

			p8_2 = best_fit_alloc(8); // allocate 8B
			printf("best fit8: p=%p\n", p8_2);

			p64_2 = best_fit_alloc(64);
			printf("best fit64: p=%p\n", p64_2);

			num = best_fit_count_extfrag(73);
			printf("num = %d\n", num);

			if (p8_1 != NULL)
			{
				best_fit_dealloc(p8_1);
				printf("deallocated8\n");
			}
			if (p8_2 != NULL)
			{
				best_fit_dealloc(p8_2);
				printf("deallocated8\n");
			}

			if (p64_1 != NULL)
			{
				best_fit_dealloc(p64_1);
				printf("deallocated64\n");
			}
			if (p64_2 != NULL)
			{
				best_fit_dealloc(p64_2);
				printf("deallocated64\n\nTest succeeded.\n");
			}
		}
		else if (test == 1)
		{
			printf("Start test...\n\n");
			p8_1 = best_fit_alloc(8); // allocate 8B
			printf("best fit8: p=%p\n", p8_1);
			p16 = best_fit_alloc(16);
			printf("best fit16: p=%p\n", p16);
			p4_1 = best_fit_alloc(4);
			printf("best fit4: p=%p\n", p4_1);
			p2 = best_fit_alloc(2);
			printf("best fit2: p=%p\n", p2);
			p256_1 = best_fit_alloc(256);
			printf("best fit256: p=%p\n", p256_1);
			p32 = best_fit_alloc(32);
			printf("best fit32: p=%p\n", p32);
			p64_1 = best_fit_alloc(64);
			printf("best fit64: p=%p\n", p64_1);
			if (p16 != NULL) // deallocate 16B
			{
				best_fit_dealloc(p16);
				printf("deallocated16\n");
			}
			if (p2 != NULL)
			{
				best_fit_dealloc(p2);
				printf("deallocated2\n");
			}
			p8_2 = best_fit_alloc(8); // allocate 8B
			printf("best fit8: p=%p\n", p8_2);
			if (p256_1 != NULL)
			{
				best_fit_dealloc(p256_1);
				printf("deallocated256\n");
			}
			p64_2 = best_fit_alloc(64);
			printf("best fit64: p=%p\n", p64_2);
			p4_2 = best_fit_alloc(4);
			printf("best fit4: p=%p\n", p4_2);
			num = best_fit_count_extfrag(289);
			printf("num = %d\n", num);
			if (p8_1 != NULL)
			{
				best_fit_dealloc(p8_1);
				printf("deallocated8\n");
			}
			if (p4_1 != NULL)
			{
				best_fit_dealloc(p4_1);
				printf("deallocated4\n");
			}
			if (p32 != NULL)
			{
				best_fit_dealloc(p32);
				printf("deallocated32\n");
			}
			if (p64_1 != NULL)
			{
				best_fit_dealloc(p64_1);
				printf("deallocated64\n");
			}
			if (p8_2 != NULL)
			{
				best_fit_dealloc(p8_2);
				printf("deallocated8\n");
			}
			if (p64_2 != NULL)
			{
				best_fit_dealloc(p64_2);
				printf("deallocated64\n");
			}
			if (p4_2 != NULL)
			{
				best_fit_dealloc(p4_2);
				printf("deallocated4\n\nTest succeeded.\n");
			}
		}
	}
	else if (algo == 1)
	{
		worst_fit_memory_init(1024);
		if (test == 0)
		{
			printf("Start test...\n\n");
			p8_1 = worst_fit_alloc(8); // allocate 8B
			printf("worst fit8: p=%p\n", p8_1);

			p16 = worst_fit_alloc(16); // allocate 16B
			printf("worst fit16: p=%p\n", p16);

			p32 = worst_fit_alloc(32); // allocate 32B
			printf("worst fit32: p=%p\n", p32);

			p64_1 = worst_fit_alloc(64);
			printf("worst fit64: p=%p\n", p64_1);

			if (p16 != NULL) // deallocate 16B
			{
				worst_fit_dealloc(p16);
				printf("deallocated16\n");
			}
			if (p32 != NULL) // deallocate 32B
			{
				worst_fit_dealloc(p32);
				printf("deallocated32\n");
			}

			p8_2 = worst_fit_alloc(8);
			printf("worst fit8: p=%p\n", p8_2);

			p64_2 = worst_fit_alloc(64);
			printf("worst fit64: p=%p\n", p64_2);

			num = worst_fit_count_extfrag(112);
			printf("num = %d\n", num);

			if (p8_1 != NULL)
			{
				worst_fit_dealloc(p8_1);
				printf("deallocated8\n");
			}
			if (p8_2 != NULL)
			{
				worst_fit_dealloc(p8_2);
				printf("deallocated8\n");
			}

			if (p64_1 != NULL)
			{
				worst_fit_dealloc(p64_1);
				printf("deallocated64\n");
			}
			if (p64_2 != NULL)
			{
				worst_fit_dealloc(p64_2);
				printf("deallocated64\n\nTest succeeded.\n");
			}
		}
		else if (test == 1)
		{
			printf("Start test...\n\n");
			p8_1 = worst_fit_alloc(8);
			printf("worst fit8: p=%p\n", p8_1);
			p16 = worst_fit_alloc(16);
			printf("worst fit16: p=%p\n", p16);
			p4_1 = worst_fit_alloc(4);
			printf("worst fit4: p=%p\n", p4_1);
			p2 = worst_fit_alloc(2);
			printf("worst fit2: p=%p\n", p2);
			p256_1 = worst_fit_alloc(256);
			printf("worst fit256: p=%p\n", p256_1);
			p32 = worst_fit_alloc(32);
			printf("worst fit32: p=%p\n", p32);
			p64_1 = worst_fit_alloc(64);
			printf("worst fit64: p=%p\n", p64_1);
			if (p16 != NULL)
			{
				worst_fit_dealloc(p16);
				printf("deallocated16\n");
			}
			if (p2 != NULL)
			{
				worst_fit_dealloc(p2);
				printf("deallocated2\n");
			}
			p8_2 = worst_fit_alloc(8);
			printf("worst fit8: p=%p\n", p8_2);
			if (p256_1 != NULL)
			{
				worst_fit_dealloc(p256_1);
				printf("deallocated256\n");
			}
			p64_2 = worst_fit_alloc(64);
			printf("worst fit64: p=%p\n", p64_2);
			p4_2 = worst_fit_alloc(4);
			printf("worst fit4: p=%p\n", p4_2);
			num = worst_fit_count_extfrag(289);
			printf("num = %d\n", num);
			if (p8_1 != NULL)
			{
				worst_fit_dealloc(p8_1);
				printf("deallocated8\n");
			}
			if (p4_1 != NULL)
			{
				worst_fit_dealloc(p4_1);
				printf("deallocated4\n");
			}
			if (p32 != NULL)
			{
				worst_fit_dealloc(p32);
				printf("deallocated32\n");
			}
			if (p64_1 != NULL)
			{
				worst_fit_dealloc(p64_1);
				printf("deallocated64\n");
			}
			if (p8_2 != NULL)
			{
				worst_fit_dealloc(p8_2);
				printf("deallocated8\n");
			}
			if (p64_2 != NULL)
			{
				worst_fit_dealloc(p64_2);
				printf("deallocated64\n");
			}
			if (p4_2 != NULL)
			{
				worst_fit_dealloc(p4_2);
				printf("deallocated4\n\nTest succeeded.\n");
			}
		}
	}
	else
	{
		fprintf(stderr, "Should not reach here!\n");
		exit(1);
	}

	return 0;
}
