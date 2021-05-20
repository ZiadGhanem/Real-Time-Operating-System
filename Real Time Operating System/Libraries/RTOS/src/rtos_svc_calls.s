/*
 * rtos_svc_calls.s
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

 .syntax unified
 .cpu cortex-m4
 .thumb

 .global RTOS_SVC_schedulerStart
 .global RTOS_SVC_threadCreate
 .global RTOS_SVC_threadDelay
 .global RTOS_SVC_semaphoreInit
 .global RTOS_SVC_semaphoreWait
 .global RTOS_SVC_semaphoreSignal


 .text

 .type RTOS_SVC_schedulerStart, %function
 RTOS_SVC_schedulerStart:
	SVC 0
	BX LR

 .type RTOS_SVC_threadCreate, %function
 RTOS_SVC_threadCreate:
	SVC 1
	BX LR

 .type RTOS_SVC_threadDelay, %function
 RTOS_SVC_threadDelay:
 	SVC 2
 	BX LR

 .type RTOS_SVC_semaphoreInit, %function
 RTOS_SVC_semaphoreInit:
 	SVC 3
 	BX LR

 .type RTOS_SVC_semaphoreWait, %function
 RTOS_SVC_semaphoreWait:
 	SVC 4
 	BX LR

 .type RTOS_SVC_semaphoreSignal, %function
 RTOS_SVC_semaphoreSignal:
 	SVC 5
 	BX LR

