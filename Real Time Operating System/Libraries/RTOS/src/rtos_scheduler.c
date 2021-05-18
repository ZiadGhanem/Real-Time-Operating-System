/*
 * rtos_scheduler.c
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#include "rtos.h"


volatile uint32_t RTOS_systickCount;
volatile uint32_t RTOS_SVC_excReturn;
static RTOS_thread_t RTOS_idleThread;
static RTOS_stack_t RTOS_idleThreadStack;

/*
 * This is the idle thread function
 * Inputs:
 * 	None
 * Return:
 * 	None
 */
static void RTOS_idleThreadFunc(void)
{
	while(1);
}

/*
 * This Function starts the scheduler
 * Inputs:
 * 	None
 * Return:
 * 	None
 */
void RTOS_schedulerStart(void)
{
	/* Create the idle thread */
	RTOS_threadCreate(&RTOS_idleThread, &RTOS_idleThreadStack, RTOS_idleThreadFunc, MAX_PRIORITY_LEVEL - 1);

	/* Switch to the top priority ready thread */
	RTOS_threadSwitch();

	/* Get the new ready thread */
	RTOS_thread_t* pNewRunningThread = RTOS_threadGetRunning();

	/* Set the exception return value */
	RTOS_SVC_excReturn = MEM32WORD(pNewRunningThread->pStack);

	/* Set the Process Stack Pointer */
	__set_PSP(pNewRunningThread->pStack + 10 * 4);

	/* Set the control register */
	__set_CONTROL(MEM32WORD(pNewRunningThread->pStack + (1 << 2)));

	/* We don't need to pop any registers as the function has not been ran before */

	/* Instruction Synchronization Barrier flushes the pipeline in the processor,
    so that all instructions following the ISB are fetched from cache or
    memory, after the instruction has been completed. */
	__ISB();

	/* Enable all interrupts */
	__set_BASEPRI(0);

	/* Reset SysTick Count */
	RTOS_systickCount = 0;
}

/*
 * Systick handler
 * Inputs:
 * 	None
 * Return:
 * 	None
 */
void RTOS_SysTick_Handler(void)
{
	/* Invoke a pendSV exception */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    /* Increment SysTick Count */
    RTOS_systickCount++;
}
