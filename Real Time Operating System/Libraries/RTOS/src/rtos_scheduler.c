/*
 * rtos_scheduler.c
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#include "rtos_scheduler.h"


volatile uint32_t RTOS_systickCount;
volatile uint32_t RTOS_SVC_excReturn;
static RTOS_task_t RTOS_idleTask;
static RTOS_stack_t RTOS_idleTaskStack[IDLE_TASK_STACK_SIZE];

/*
 * This is the idle task function
 * Inputs:
 * 	None
 * Return:
 * 	None
 */
static void RTOS_idleTaskFunc(void)
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
	/* Create the idle task */
	RTOS_taskCreate(&RTOS_idleTask, RTOS_idleTaskStack, IDLE_TASK_STACK_SIZE, RTOS_idleTaskFunc, MAX_PRIORITY_LEVEL - 1);

	/* Switch to the top priority ready task */
	RTOS_taskSwitch();

	/* Get the new ready task */
	RTOS_task_t* pNewRunningTask = RTOS_taskGetRunning();

	/* Set the exception return value */
	RTOS_SVC_excReturn = MEM32WORD(pNewRunningTask->pStack);

	/* Set the Process Stack Pointer */
	__set_PSP(pNewRunningTask->pStack + 10 * 4);

	/* Set the control register */
	__set_CONTROL(MEM32WORD(pNewRunningTask->pStack + (1 << 2)));

	/* We don't need to pop any registers as the function has not been ran before */

	/* Instruction Synchronization Barrier flushes the pipeline in the processor,
    so that all instructions following the ISB are fetched from cache or
    memory, after the instruction has been completed. */
	__ISB();

	/* Reset SysTick Count */
	RTOS_systickCount = 0;

	/* Enable all interrupts */
	__set_BASEPRI(0);

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
 	/* Check for delayed tasks to be unblocked */
 	RTOS_taskDelayCheck();

	/* Invoke a pendSV exception */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

    /* Increment SysTick Count */
    RTOS_systickCount++;
}

/*
 * This function returns the systick count so far
 * Inputs:
 * 	None
 * Return:
 * 	None
 */
uint32_t RTOS_schedulerGetSystickCount(void)
{
	return RTOS_systickCount;
}
