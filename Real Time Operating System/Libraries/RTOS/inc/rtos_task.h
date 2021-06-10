/*
 * rtos_task.h
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_TASK_H_
#define RTOS_INC_RTOS_TASK_H_

#include "rtos.h"
#include "rtos_list.h"
#include "rtos_scheduler.h"

/** RTOS_stack_t
 *The stack of the process
 */
typedef uint64_t RTOS_stack_t;

/* RTOS_taskState_t */
typedef enum{
	RTOS_TASKRUNNING,
	RTOS_TASKREADY,
	RTOS_TASKBLOCKED,
	RTOS_TASKTERMINATED
}RTOS_taskState_t;

/** RTOS_task_t
 * pStack -> Pointer to the stack of the task
 * priority -> Priority level of the task (Lower is higher)
 * listItem -> The list item to be inserted in the ready lists
 * eventListItem -> The list item to be inserted in the event list
 * taskId -> Number of the task
 * taskState -> The current state of the task
 */
typedef struct{
	uint32_t pStack;
	uint32_t priority;
	RTOS_listItem_t listItem;
	RTOS_listItem_t eventListItem;
	int32_t taskId;
	RTOS_taskState_t taskState;
}RTOS_task_t;

extern void RTOS_SVC_taskCreate(RTOS_task_t* pTask, RTOS_stack_t* pStack, uint32_t stackSize, void* pFunction, uint32_t priority);
extern void RTOS_SVC_taskDelay(uint32_t waitTicks);
extern void RTOS_SVC_taskTerminate(RTOS_task_t* pTask);

void RTOS_taskListsInit(void);
void RTOS_taskCreate(RTOS_task_t* pTask, RTOS_stack_t* pStack, uint32_t stackSize, void* pFunction, uint32_t priority);
RTOS_task_t* RTOS_taskGetRunning(void);
void RTOS_taskAddToReadyList(RTOS_task_t* pTask);
void RTOS_taskAddToTimerList(RTOS_task_t* pTask, uint32_t waitTicks);
void RTOS_taskSwitch(void);
void RTOS_taskDelay(uint32_t waitTicks);
void RTOS_taskDelayCheck(void);
void RTOS_taskTerminate(RTOS_task_t* pTask);

#endif /* RTOS_INC_RTOS_TASK_H_ */
