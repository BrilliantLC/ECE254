To test our code, we initialized a memory pool with 20 boxes.

We let task 3 allocate 16 boxes, and let task 4 allocate 6 boxes.
But at this point we're running out of memory, so only 4 out of 6 are allocated. Task 4 is blocked. So are task 5 and 6.

After the wait timer has gone off, the task with the highest priority resumes, is this case task 5 with a priority of 4. Task 5 allocates 10 boxes.

Now the tasks 4 and 6 are of the same priority. Thus the one that's been waiting the longest will resume first (ie. task4), allocating the remaining 2 boxes that it weren't able to allocated earlier. Finally task 6 gets to allocated its 4 boxes.

At the end we used up all 20 blocks of memory in the pool (10 for task5, 6 for task4, and 4 for task6).