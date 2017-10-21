/**
 * @brief: ECE254 Keil ARM RL-RTX Task Management Lab starter file that calls os_tsk_get()
 * @file: main_task_exp.c
 * @date: 2015/09/13
 */
/* NOTE: This release is for students to use */

#include <LPC17xx.h>
#include "uart_polling.h"
#include <RTL.h>
#include "../../RTX_CM3/INC/RTL_ext.h" /* extended interface header file */
#include <stdio.h>
#include <string.h>

#define NUM_FNAMES 8

struct func_info
{
  void (*p)();   /* function pointer */
  char name[16]; /* name of the function */
};

extern void os_idle_demon(void);
__task void task1(void);
__task void task2(void);
__task void task3(void);
__task void task4(void);
__task void task5(void);
__task void task6(void);
__task void init(void);

char *state2str(unsigned char state, char *str);
char *fp2name(void (*p)(), char *str);

OS_MUT g_mut_uart;
OS_TID g_tid = 255;

int g_counter = 0; // a global counter
char g_str[16];
char g_tsk_name[16];

// added for lab2
_declare_box(mympool, 12, 20);

struct func_info g_task_map[NUM_FNAMES] = \
{
    /* os_idle_demon function ptr to be initialized in main */
    {NULL, "os_idle_demon"},
    {task1, "task1"},
    {task2, "task2"},
    {task3, "task3"},
    {task4, "task4"},
    {task5, "task5"},
    {task6, "task6"},
    {init, "init"}};

/* no local variables defined, use one global var */
__task void task1(void)
{
  for (;;)
  {
    g_counter++;
  }
}

/*--------------------------- task2 -----------------------------------*/
/* checking states of all tasks in the system                          */
/*---------------------------------------------------------------------*/
__task void task2(void)
{
  U8 i = 1;
  RL_TASK_INFO task_info;

  while (1)
  {
    os_mut_wait(g_mut_uart, 0xFFFF);
    printf("TID\tNAME\t\tPRIO\tSTATE   \t%%STACK\n");
    os_mut_release(g_mut_uart);

    for (i = 0; i < 7; i++)
    { // this is a lazy way of doing loop.
      if (os_tsk_get(i + 1, &task_info) == OS_R_OK)
      {
        os_mut_wait(g_mut_uart, 0xFFFF);
        printf("%d\t%s\t\t%d\t%s\t%d%%\n",
               task_info.task_id,
               fp2name(task_info.ptask, g_tsk_name),
               task_info.prio,
               state2str(task_info.state, g_str),
               task_info.stack_usage);
        os_mut_release(g_mut_uart);
      }
    }

    if (os_tsk_get(0xFF, &task_info) == OS_R_OK)
    {
      os_mut_wait(g_mut_uart, 0xFFFF);
      printf("%d\t%s\t\t%d\t%s\t%d%%\n",
             task_info.task_id,
             fp2name(task_info.ptask, g_tsk_name),
             task_info.prio,
             state2str(task_info.state, g_str),
             task_info.stack_usage);
      os_mut_release(g_mut_uart);
    }
    os_dly_wait(0xFFFF);
  }
}

// task3 allocates 16 boxes, then waits a bit before freeing them
__task void task3(void)
{
  int i;
  void *myptr[16];
  OS_RESULT r;

  // allocated boxes
  os_mut_wait(g_mut_uart, 0xFFFF);
  printf("\n(3)Allocating memory...\n");
  os_mut_release(g_mut_uart);
  for (i = 0; i < 16; i++)
  {
    myptr[i] = os_mem_alloc(mympool);
    os_mut_wait(g_mut_uart, 0xFFFF);
    printf("(3)Allocated memory @%p!\n", myptr[i]);
    os_mut_release(g_mut_uart);
  }

  // wait for other tasks
  os_dly_wait(15);

  os_mut_wait(g_mut_uart, 0xFFFF);
  printf("(3)Freeing memory...\n");
  os_mut_release(g_mut_uart);
  // free all addresses allocated
  for (i = 0; i < 16; i++)
  {
    r = os_mem_free(mympool, myptr[i]);
    if (r == OS_R_OK)
    {
      os_mut_wait(g_mut_uart, 0xFFFF);
      printf("(3)Freed memory @%p!\n", myptr[i]);
      os_mut_release(g_mut_uart);
    }
    else  // if freeing memory has failed
    {
      printf("(3)Invalid operation\n");
      break;
    }
  }
  printf("(3)Finished\n\n");
  os_tsk_delete_self();
}

// task4 allocates 6 boxes
__task void task4(void)
{
  int i;
  void *myptr[6];

  os_dly_wait(2);

  os_mut_wait(g_mut_uart, 0xFFFF);
  printf("\n(4)Allocating memory...\n");
  os_mut_release(g_mut_uart);
  for (i = 0; i < 6; i++)
  {
    myptr[i] = os_mem_alloc(mympool);
    os_mut_wait(g_mut_uart, 0xFFFF);
    printf("(4)Allocated memory @%p!\n", myptr[i]);
    os_mut_release(g_mut_uart);
  }
  printf("(4)Finished\n\n");
  os_tsk_delete_self();
}

// task5 allocates 10 boxes
__task void task5(void)
{
  int i;
  void *myptr[10];

  os_dly_wait(5);

  os_mut_wait(g_mut_uart, 0xFFFF);
  printf("\n(5)Allocating memory...\n");
  os_mut_release(g_mut_uart);
  for (i = 0; i < 10; i++)
  {
    myptr[i] = os_mem_alloc(mympool);
    os_mut_wait(g_mut_uart, 0xFFFF);
    printf("(5)Allocated memory @%p!\n", myptr[i]);
    os_mut_release(g_mut_uart);
  }
  printf("(5)Finished\n\n");
  os_tsk_delete_self();
}

// task5 allocates 4 boxes
__task void task6(void)
{
  int i;
  void *myptr[4];

  os_dly_wait(8);

  os_mut_wait(g_mut_uart, 0xFFFF);
  printf("\n(6)Allocating memory...\n");
  os_mut_release(g_mut_uart);
  for (i = 0; i < 4; i++)
  {
    myptr[i] = os_mem_alloc(mympool);
    os_mut_wait(g_mut_uart, 0xFFFF);
    printf("(6)Allocated memory @%p!\n", myptr[i]);
    os_mut_release(g_mut_uart);
  }
  printf("(6)Finished\nTest passed!\n");
  os_tsk_delete_self();
}

/*--------------------------- init ------------------------------------*/
/* initialize system resources and create other tasks                  */
/*---------------------------------------------------------------------*/
__task void init(void)
{
  _init_box(mympool, sizeof(mympool), 12);

  os_mut_init(&g_mut_uart);
  os_mut_wait(g_mut_uart, 0xFFFF);

  printf("init: TID = %d\n", os_tsk_self());

  g_tid = os_tsk_create(task1, 1); /* task 1 at priority 1 */
  printf("init: created task1 with TID %d\n", g_tid);

  g_tid = os_tsk_create(task2, 6); /* task 2 at priority 6 */
  printf("init: created task2 with TID %d\n", g_tid);

  g_tid = os_tsk_create(task3, 5); /* task 3 at priority 5 */
  printf("init: created task3 with TID %d\n", g_tid);

  g_tid = os_tsk_create(task4, 3); /* task 4 at priority 3 */
  printf("init: created task4 with TID %d\n", g_tid);

  g_tid = os_tsk_create(task5, 4); /* task 5 at priority 4 */
  printf("init: created task5 with TID %d\n", g_tid);

  g_tid = os_tsk_create(task6, 3); /* task 6 at priority 3 */
  printf("init: created task6 with TID %d\n\n", g_tid);

  os_mut_release(g_mut_uart);
  os_tsk_delete_self(); /* task MUST delete itself before exiting */
}

/**
 * @brief: convert state numerical value to string represenation      
 * @param: state numerical value (macro) of the task state
 * @param: str   buffer to save the string representation, 
 *               buffer to be allocated by the caller
 * @return:the string starting address
 */
char *state2str(unsigned char state, char *str)
{
  switch (state)
  {
  case INACTIVE:
    strcpy(str, "INACTIVE");
    break;
  case READY:
    strcpy(str, "READY   ");
    break;
  case RUNNING:
    strcpy(str, "RUNNING ");
    break;
  case WAIT_DLY:
    strcpy(str, "WAIT_DLY");
    break;
  case WAIT_ITV:
    strcpy(str, "WAIT_ITV");
    break;
  case WAIT_OR:
    strcpy(str, "WAIT_OR");
    break;
  case WAIT_AND:
    strcpy(str, "WAIT_AND");
    break;
  case WAIT_SEM:
    strcpy(str, "WAIT_SEM");
    break;
  case WAIT_MBX:
    strcpy(str, "WAIT_MBX");
    break;
  case WAIT_MUT:
    strcpy(str, "WAIT_MUT");
    break;
  default:
    strcpy(str, "UNKNOWN");
  }
  return str;
}

/** 
 * @brief: get function name by function pointer
 * @param: p the entry point of a function (i.e. function pointer)
 * @param: str the buffer to return the function name
 * @return: the function name string starting address
 */
char *fp2name(void (*p)(), char *str)
{
  int i;
  unsigned char is_found = 0;

  for (i = 0; i < NUM_FNAMES; i++)
  {
    if (g_task_map[i].p == p)
    {
      str = strcpy(str, g_task_map[i].name);
      is_found = 1;
      break;
    }
  }
  if (is_found == 0)
  {
    strcpy(str, "ghost");
  }
  return str;
}

int main(void)
{
  SystemInit(); /* initialize the LPC17xx MCU */
  uart0_init(); /* initilize the first UART */

  /* fill the fname map with os_idle_demon entry point */
  g_task_map[0].p = os_idle_demon;

  printf("Calling os_sys_init()...\n");
  os_sys_init(init); /* initilize the OS and start the first task */
}
