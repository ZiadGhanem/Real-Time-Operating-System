/*
 * rtos_thread.h
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_THREAD_H_
#define RTOS_INC_RTOS_THREAD_H_

#include "rtos.h"
#include "rtos_list.h"
#include "rtos_scheduler.h"

/** RTOS_stack_t
 *The stack of the process
 */
typedef uint64_t RTOS_stack_t;

/* RTOS_taskState_t */
typedef enum{
	RTOS_THREADRUNNING,
	RTOS_THREADREADY,
	RTOS_THREADBLOCKED,
	RTOS_THREADTERMINATED
}RTOS_threadState_t;

/** RTOS_thread_t
 * pStack -> Pointer to the stack of the thread
 * priority -> Priority level of the thread (Lower is higher)
 * listItem -> The list item to be inserted in the ready lists
 * eventListItem -> The list item to be inserted in the event list
 * threadId -> Number of the thread
 * threadState -> The current state of the thread
 */
typedef struct{
	uint32_t pStack;
	uint32_t priority;
	RTOS_listItem_t listItem;
	RTOS_listItem_t eventListItem;
	int32_t threadId;
	RTOS_threadState_t threadState;
}RTOS_thread_t;

extern void RTOS_SVC_threadCreate(RTOS_thread_t* pThread, RTOS_stack_t* pStack, uint32_t stackSize, void* pFunction, uint32_t priority);
extern void RTOS_SVC_threadDelay(uint32_t waitTicks);
extern void RTOS_SVC_threadTerminate(RTOS_thread_t* pThread);

void RTOS_threadListsInit(void);
void RTOS_threadCreate(RTOS_thread_t* pThread, RTOS_stack_t* pStack, uint32_t stackSize, void* pFunction, uint32_t priority);
RTOS_thread_t* RTOS_threadGetRunning(void);
void RTOS_threadAddToReadyList(RTOS_thread_t* pThread);
void RTOS_threadAddToTimerList(RTOS_thread_t* pThread, uint32_t waitTicks);
void RTOS_threadSwitch(void);
void RTOS_threadDelay(uint32_t waitTicks);
void RTOS_threadDelayCheck(void);
void RTOS_threadTerminate(RTOS_thread_t* pThread);

#endif /* RTOS_INC_RTOS_THREAD_H_ */
