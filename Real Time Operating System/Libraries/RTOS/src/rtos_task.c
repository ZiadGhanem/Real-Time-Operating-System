/*
 * rtos_task.c
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#include <rtos_task.h>

/* Current top task priority */
static uint32_t currentTopPriority = MAX_PRIORITY_LEVEL - 1;
/* Pointer to the current running task */
static RTOS_task_t* pRunningTask = NULL;
/* The ID of the current running task */
static int32_t runningTaskID = -1;
/* RTOS ready lists */
static RTOS_list_t RTOS_readyList[MAX_PRIORITY_LEVEL];
/* RTOS delayed list */
static RTOS_list_t RTOS_delayList;
/* Task ID numbering */
static int32_t numTasks = 0;

/*
 * This function initializes the used lists
 * Inputs:
 *  None
 * Return:
 * 	None
 */
void RTOS_taskListsInit(void)
{
	/* Initialize the ready lists */
	uint32_t i;
	for(i = 0; i < MAX_PRIORITY_LEVEL; i++)
		RTOS_listInit(&RTOS_readyList[i]);

	/* Initialize the delayed list */
	RTOS_listInit(&RTOS_delayList);
}

/*
 * This function creates a new task and its it to the desired list
 * Inputs:
 *  pTask -> The RTOS task object
 *  pStack -> The Stack of the new task
 * 	pFunction -> The RTOS task function
 * 	priority -> The task priority level
 * Return:
 * 	None
 */
void RTOS_taskCreate(RTOS_task_t* pTask, RTOS_stack_t* pStack, uint32_t stackSize, void* pFunction, uint32_t priority)
{
	ASSERT(pTask != NULL);
	ASSERT(pStack != NULL);
	ASSERT(stackSize <= MAX_STACK_SIZE);
	ASSERT(pFunction != NULL);
	ASSERT((priority < MAX_PRIORITY_LEVEL) && (priority >= 0));

	/* Stack: xPSR, PC, LR, R12, R3, R1, R1, R0 are automatically pushed,
	 * 		 while the rest are pushed manually during task switching
	 * EXC_RETURN
	 * CONTROL
	 * R4
	 * R5
	 * R6
	 * R7
	 * R8
	 * R9
	 * R10
	 * R11
	 * S16 - S31, If FPU is on
	 * R0
	 * R1
	 * R2
	 * R3
	 * R12
	 * LR
	 * PC
	 * xPSR
	 */

	/* To ensure that the stack is 8 byte aligned */
	stackSize = (stackSize / 8) * 8;

	/* Initialize the stack pointer */
	pTask->pStack = (uint32_t)pStack + stackSize * 8 - 18 * 4;

	/* For testing */
	MEM32WORD(pTask->pStack + (2 << 2)) = 0x4;
	MEM32WORD(pTask->pStack + (3 << 2)) = 0x5;
	MEM32WORD(pTask->pStack + (4 << 2)) = 0x6;
	MEM32WORD(pTask->pStack + (5 << 2)) = 0x7;
	MEM32WORD(pTask->pStack + (6 << 2)) = 0x8;
	MEM32WORD(pTask->pStack + (7 << 2)) = 0x9;
	MEM32WORD(pTask->pStack + (8 << 2)) = 0x10;
	MEM32WORD(pTask->pStack + (9 << 2)) = 0x11;
	MEM32WORD(pTask->pStack + (10 << 2)) = 0x1;
	MEM32WORD(pTask->pStack + (11 << 2)) = 0x2;
	MEM32WORD(pTask->pStack + (12 << 2)) = 0x3;
	MEM32WORD(pTask->pStack + (13 << 2)) = 0x4;
	MEM32WORD(pTask->pStack + (14 << 2)) = 0x12;

	/* Initialize EXC_RETURN (Return to task mode using PSP) */
	MEM32WORD(pTask->pStack) = 0xFFFFFFFDUL;

	/* Initialize the CONTROL Register (No FPU, PSP, Unprivileged mode)*/
	MEM32WORD(pTask->pStack + (1 << 2)) = 0x3;

	/* Initialize the program counter */
	MEM32WORD(pTask->pStack + (16 << 2)) = (uint32_t)pFunction;

	/* Initialize the xPSR register to only Thumb mode*/
	MEM32WORD(pTask->pStack + (17 << 2)) = 0x01000000;

	/* Set the priority level */
	pTask->priority = priority;

	/* Set task ID */
	pTask->taskId = numTasks;
	numTasks++;

	/* Set the task state to ready */
	pTask->taskState = RTOS_TASKREADY;

	/* Set the task's list item task pointer */
	pTask->listItem.pTask = pTask;
	pTask->eventListItem.pTask = pTask;

	/* Set the task's list item to none */
	pTask->listItem.pList = NULL;
	pTask->eventListItem.pList = NULL;

	/* Add the task to the ready list */
	RTOS_listInsertEnd(&RTOS_readyList[priority], &pTask->listItem);

	/* Check if the new task has a new highest priority */
	if(priority < currentTopPriority)
	{
		currentTopPriority = priority;
	}
	else
	{

	}

	/* Check if the scheduler has started and the currently running task has a lower priority */
	if((pRunningTask != NULL) && (priority < pRunningTask->priority))
	{
		/* Invoke a pendSV exception */
	    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{

	}

}


/*
 * This function returns the running task
 * Inputs:
 *  None
 * Return:
 * 	Pointer to the running task
 */
RTOS_task_t* RTOS_taskGetRunning(void)
{
	return pRunningTask;
}

/*
 * This function adds the task to its ready list
 * Inputs:
 *  pTask -> The task to be inserted in the ready list
 * Return:
 * 	None
 */
void RTOS_taskAddToReadyList(RTOS_task_t* pTask)
{
	RTOS_listItem_t* pListItem = &pTask->listItem;
	/* Add the task to ready list */
	RTOS_listInsertEnd(&RTOS_readyList[pTask->priority], pListItem);
	/* Set the task state as ready */
	pTask->taskState = RTOS_TASKREADY;
	/* Check if the task has a new highest priority */
	if(pTask->priority < currentTopPriority)
	{
		/* Set the new top priority */
		currentTopPriority = pTask->priority;
	}
	else
	{

	}

	if(pRunningTask != NULL &&
	(pTask->priority < pRunningTask->priority))
	{
		/* Invoke a pendSV exception */
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}
	else
	{
	}
}

/*
 * This function adds the task to the timer's list
 * Inputs:
 *  pTask -> The task to be inserted in the ready list
 *  waitTicks -> Number of systicks to be delayed
 * Return:
 * 	None
 */
void RTOS_taskAddToTimerList(RTOS_task_t* pTask, uint32_t waitTicks)
{
	/* Get the list item */
	RTOS_listItem_t* pListItem = & pTask->listItem;

	/* Set the delay amount */
	pListItem->orderValue = RTOS_schedulerGetSystickCount() + waitTicks;

	/* Add the task to the delayed list */
	RTOS_listInsert(&RTOS_delayList, pListItem);

	/* Set the task state to blocked */
	pRunningTask->taskState = RTOS_TASKBLOCKED;
}


/*
 * This function switches the currently running task
 * Inputs:
 *  None
 * Return:
 * 	None
 */
void RTOS_taskSwitch(void)
{
	/* Check if the current top priority list became empty */
	while(RTOS_readyList[currentTopPriority].numListItems == 0)
	{
		currentTopPriority++;
	}

	/* Get the next task in the list */
	RTOS_readyList[currentTopPriority].pIndex = RTOS_readyList[currentTopPriority].pIndex->pNext;

	/* Check if the current index is the end item */
	if(RTOS_readyList[currentTopPriority].pIndex == (RTOS_listItem_t*) &RTOS_readyList[currentTopPriority].endItem)
	{
		/* Increment the index */
		RTOS_readyList[currentTopPriority].pIndex = RTOS_readyList[currentTopPriority].pIndex->pNext;
	}

	RTOS_task_t* pTask = RTOS_readyList[currentTopPriority].pIndex->pTask;

	/* Set the currently running task */
	pRunningTask = pTask;
	runningTaskID = pTask->taskId;

	/* Set the task state to running */
	pTask->taskState = RTOS_TASKRUNNING;
}


/*
 * This function delays the currently running task
 * Inputs:
 *  waitTicks -> Number of system ticks to be delayed
 * Return:
 * 	None
 */
void RTOS_taskDelay(uint32_t waitTicks)
{
	/* Remove the current task from ready list */
	RTOS_listItem_t* pListItem = & pRunningTask->listItem;
	RTOS_listRemove(pListItem);

	/* Add the task to timer list */
	RTOS_taskAddToTimerList(pListItem->pTask, waitTicks);

	/* Invoke a pendSV exception */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

/*
 * This function checks if any of the blocked tasks can be transferred to the ready list
 * Inputs:
 *  None
 * Return:
 * 	None
 */
void RTOS_taskDelayCheck(void)
{
	if(RTOS_delayList.numListItems > 0)
	{
		/* Check for delayed tasks */
		RTOS_listItem_t* pListItem = RTOS_delayList.endItem.pPrev;
		if(pListItem->orderValue <= RTOS_schedulerGetSystickCount())
		{
			/* Remove the current task from delay list */
			RTOS_listRemove(pListItem);

			/* Add the task to the ready list */
			RTOS_taskAddToReadyList(pListItem->pTask);
		}
		else
		{

		}
	}
	else
	{
	}
}



/*
 * This function terminates a task
 * Inputs:
 *  pTask -> Pointer to the task to be deleted
 * Return:
 * 	None
 */
void RTOS_taskTerminate(RTOS_task_t* pTask)
{

	if(pTask == NULL)
	{
		pTask = pRunningTask;
	}
	else
	{

	}

	/* Remove the tasks items from any lists */
	if(pTask->listItem.pList != NULL)
	{
		RTOS_listRemove(& pTask->listItem);
	}
	else
	{

	}


	if(pTask->eventListItem.pList != NULL)
	{
		RTOS_listRemove(& pTask->eventListItem);
	}
	else
	{

	}

	/* Set the task as terminated */
	pTask->taskState = RTOS_TASKTERMINATED;

	/* If the task is the current running task */
	if(pTask == pRunningTask)
	{
		/* Invoke a pendSV exception */
	    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
	}

}
