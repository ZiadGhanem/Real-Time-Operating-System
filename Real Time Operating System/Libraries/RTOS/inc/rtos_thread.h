/*
 * rtos_thread.h
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_THREAD_H_
#define RTOS_INC_RTOS_THREAD_H_


/** RTOS_stack_t
 * stack -> The stack of the process
 */
typedef struct{
	uint64_t stack[MAX_STACK_SIZE];
}RTOS_stack_t;

/** RTOS_thread_t
 * pStack -> Pointer to the stack of the thread
 * priority -> Priority level of the thread (Lower is higher)
 * listItem -> The list item to be inserted in the list
 */
typedef struct{
	uint32_t pStack;
	uint32_t priority;
	RTOS_listItem_t listItem;
	uint32_t threadId;
	uint32_t delay_systicks;
}RTOS_thread_t;

extern void RTOS_SVC_threadCreate(RTOS_thread_t* pThread, RTOS_stack_t* pStack, void* pFunction, uint32_t priority);
extern void RTOS_SVC_threadDelay(uint32_t systicks);

void RTOS_threadReadyListsInit(void);
void RTOS_threadCreate(RTOS_thread_t* pThread, RTOS_stack_t* pStack, void* pFunction, uint32_t priority);
RTOS_thread_t* RTOS_threadGetRunning(void);
RTOS_list_t* RTOS_threadGetList(uint32_t priority);
void RTOS_threadSetTopPriority(uint32_t priority);
uint32_t RTOS_threadGetTopPriority(void);
void RTOS_threadSwitch(void);
void RTOS_threadDelay(uint32_t systicks);
void RTOS_threadUnblock(void);

#endif /* RTOS_INC_RTOS_THREAD_H_ */
