/*
 * rtos.c
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#include "rtos.h"

/*
 * This Function initializes the RTOS
 * Inputs:
 * 	None
 * Return:
 * 	None
 */
void RTOS_init(void)
{
	/* Disable interrupt requests during initialization */
	__disable_irq();

	/* Double word stack alignment */
	SCB->SCR |= SCB_CCR_STKALIGN_Msk;

	/* Setup system clock */
	SystemCoreClockUpdate();

	/* Configure NVIC */
	NVIC_SetPriorityGrouping(3);

	/* Configure SysTick */
	ASSERT(SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY) == 0);
	NVIC_SetPriority(SysTick_IRQn, 1);
	NVIC_EnableIRQ(SysTick_IRQn);


	/* Enable SVC interrupt and set its priority */
	NVIC_SetPriority(SVCall_IRQn, 0);
	NVIC_EnableIRQ(SVCall_IRQn);

	/* Enable PendSV interrupt and set its priority */
	NVIC_SetPriority(PendSV_IRQn, 0xFF);
	NVIC_EnableIRQ(PendSV_IRQn);

	/* Initialize ready lists */
	RTOS_threadReadyListsInit();

	/* Disable all interrupts except SVC */
	__set_BASEPRI(1);

	/* Enable interrupts */
	__enable_irq();
}


/*
 * This Function handles all supervisor calls
 * Inputs:
 * 	svc_args -> The supervisor calls arguments passed to the supervisor call
 * Return:
 * 	None
 */
void RTOS_SVC_Handler_Main(uint32_t* svc_args)
{
	/*
	 * Stack contains:
	 *  r0, r1, r2, r3, r12, r14, the return address and xPSR
	 * First argument (r0) is svc_args[0]
	 */
	uint32_t svc_number;
	svc_number = ((int8_t*)svc_args[6])[-2];

	switch(svc_number)
	{
		case 0:
			/* Start the scheduler */
			RTOS_schedulerStart();
			break;
		case 1:
			/* Create a thread */
			RTOS_threadCreate((RTOS_thread_t*)svc_args[0],
							(RTOS_stack_t*)svc_args[1],
							(void *)svc_args[2],
							(uint32_t)svc_args[3]);
			break;
		/* Unsupported supervisor call */
		default:
			ASSERT(0)
			break;
	}
}
