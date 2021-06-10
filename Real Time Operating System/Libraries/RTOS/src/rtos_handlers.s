/*
 * rtos_handlers.s
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */


 .syntax unified
 .cpu cortex-m4
 .thumb


 .global RTOS_SVC_Handler
 .global RTOS_PendSV_Handler

 .text

 .type RTOS_SVC_Handler, %function
 RTOS_SVC_Handler:
 	TST LR, #4
	ITE EQ
	MRSEQ R0, MSP
	MRSNE R0, PSP
	LDR R1, =RTOS_SVC_excReturn
	STR LR, [R1]
	BL RTOS_SVC_Handler_Main
	LDR R1, =RTOS_SVC_excReturn
	LDR LR, [R1]
	BX LR

 .type RTOS_PendSV_Handler, %function
 RTOS_PendSV_Handler:
 	/****************** Saving the current task's context ********************/
 	/* Get PSP */
 	MRS R1, PSP
	/* Check if the currently running task uses FPU
	 * If so so push FPU registers (s16 - s32)
	**/
	TST LR, 0x10	/* Test bit 5 in LR, if it is zero then FPU is enabled */
	IT	EQ	/* If zero flag is cleared */
	VSTMDBEQ R1!, {S16-S31}
	/* Save EXC_RETURN into R2 */
	MOV	R2, LR
	/* Save CONTROL into R3 */
	MRS R3, CONTROL
	/* Instruction Synchronization Barrier */
	ISB
	/* Push {R2 -> R11} into the task's stack*/
	STMDB R1!, {R2-R11}
	/* Save the task's stack pointer */
	BL RTOS_taskGetRunning
	STR R1, [R0]

	/****************** Switch tasks ********************/
	/* Disable all interrupts excepts SVC as PendSV can be interrupted */
	MOV R0, #1
	MSR BASEPRI, R1
 	/* Get the next task */
 	BL RTOS_taskSwitch
 	/* Enable all interrupts */
	MOV R0, #0
	MSR BASEPRI, R0
 	/* Set the process stack pointer to the task's stack */
 	BL RTOS_taskGetRunning

	/****************** Restoring the next task's context ********************/
 	LDR R1, [R0]
 	/* POP {R2 -> R11} from the task's stack*/
 	LDMIA R1!, {R2-R11}
 	/* Move R3 into CONTROL */
 	MSR CONTROL, R3
 	/* Instruction Synchronization Barrier */
 	ISB
 	/* MOVE R2 into Link Register */
 	MOV LR, R2
 	/* Check if the currently running task uses FPU
	 * If so so pop FPU registers (s16 - s32)
	**/
	TST LR, 0x10	/* Test bit 5 in LR, if it is zero then FPU is enabled */
	IT	EQ	/* If zero flag is cleared */
	VLDMIAEQ R1!, {S16-S31}
	/* Save PSP */
	MSR PSP, R1
	/* Return from interrupt */
	BX LR
