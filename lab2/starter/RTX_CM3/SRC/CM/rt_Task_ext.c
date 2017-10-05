/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_TASK_ext.C
 *      Purpose: Interface functions for extended task management  
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_MemBox.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *      Local Functions
 *---------------------------------------------------------------------------*/
/*--------------------------- rt_tsk_count_get ------------------------------*/
/* added in ECE254 lab keil_rtx */
int rt_tsk_count_get(void)
{
	int id;
	int count = 0;
	for (id = 0; id < os_maxtaskrun; id++)
	{
		// if task is found, increment count
		if (os_active_TCB[id] != NULL)
			count++;
	}
	return count;
}

/*--------------------------- rt_tsk_get ------------------------------------*/
/* added in ECE254 lab keil_proc */
OS_RESULT rt_tsk_get(OS_TID task_id, RL_TASK_INFO *p_task_info)
{
	P_TCB p_tcb;

	// if task is idle, get task in idle TCB
	if (task_id == os_idle_TCB.task_id)
	{
		p_tcb = &os_idle_TCB;
	}
	else // if task is not idle, look for task in active TCB array
	{
		p_tcb = os_active_TCB[task_id - 1];
	}

	// if state is unknown
	if (p_tcb->state >= 10)
	{
		return OS_R_NOK;
	}
	else
	{
		// get task info from p_tcb
		p_task_info->task_id = p_tcb->task_id;
		p_task_info->state = p_tcb->state;
		p_task_info->prio = p_tcb->prio;
		p_task_info->ptask = p_tcb->ptask;

		// if status is not "RUNNING"
		if (p_tcb->state != 2)
		{
			// size of memory assigned to the stack
			U32 sizeInAddr = (U16)os_stackinfo;
			// address of the top of stack for tasks not running is stored in tsk_stack
			p_task_info->stack_usage = (U8)(((U32)(p_tcb->stack) + sizeInAddr - (U32)(p_tcb->tsk_stack)) * 100 / sizeInAddr);
		}
		else // if "RUNNING"
		{
			U32 sizeInAddr = (U16)os_stackinfo;
			//  rt_get_PSP() returns the address of the top of stack for running tasks
			p_task_info->stack_usage = (U8)(((U32)(p_tcb->stack) + sizeInAddr - (U32)rt_get_PSP()) * 100 / sizeInAddr);
		}

		return OS_R_OK;
	}
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/