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
 .global RTOS_SVC_taskCreate
 .global RTOS_SVC_taskDelay
 .global RTOS_SVC_taskTerminate
 .global RTOS_SVC_semaphoreInit
 .global RTOS_SVC_semaphoreWait
 .global RTOS_SVC_semaphoreSignal
 .global RTOS_SVC_mutexInit
 .global RTOS_SVC_mutexLock
 .global RTOS_SVC_mutexUnlock
 .global RTOS_SVC_spinInit
 .global RTOS_SVC_spinLock
 .global RTOS_SVC_spinUnlock
 .global RTOS_SVC_mailBoxInit
 .global RTOS_SVC_mailBoxSend
 .global RTOS_SVC_mailBoxReceive


 .text

 .type RTOS_SVC_schedulerStart, %function
 RTOS_SVC_schedulerStart:
	SVC 0
	BX LR

 .type RTOS_SVC_taskCreate, %function
 RTOS_SVC_taskCreate:
	SVC 1
	BX LR

 .type RTOS_SVC_taskDelay, %function
 RTOS_SVC_taskDelay:
 	SVC 2
 	BX LR

 .type RTOS_SVC_taskTerminate, %function
 RTOS_SVC_taskTerminate:
 	SVC 3
 	BX LR

 .type RTOS_SVC_semaphoreInit, %function
 RTOS_SVC_semaphoreInit:
 	SVC 4
 	BX LR

 .type RTOS_SVC_semaphoreWait, %function
 RTOS_SVC_semaphoreWait:
 	SVC 5
 	BX LR

 .type RTOS_SVC_semaphoreSignal, %function
 RTOS_SVC_semaphoreSignal:
 	SVC 6
 	BX LR

 .type RTOS_SVC_mutexInit, %function
 RTOS_SVC_mutexInit:
 	SVC 7
 	BX LR

 .type RTOS_SVC_mutexLock, %function
 RTOS_SVC_mutexLock:
 	SVC 8
 	BX LR

 .type RTOS_SVC_mutexUnlock, %function
 RTOS_SVC_mutexUnlock:
 	SVC 9
 	BX LR

 .type RTOS_SVC_spinInit, %function
 RTOS_SVC_spinInit:
 	SVC 10
 	BX LR

 .type RTOS_SVC_spinLock, %function
 RTOS_SVC_spinLock:
 	SVC 11
 	BX LR

 .type RTOS_SVC_spinUnlock, %function
 RTOS_SVC_spinUnlock:
 	SVC 12
 	BX LR

 .type RTOS_SVC_mailBoxInit, %function
 RTOS_SVC_mailBoxInit:
 	SVC 13
 	BX LR

 .type RTOS_SVC_mailBoxSend, %function
 RTOS_SVC_mailBoxSend:
 	SVC 14
 	BX LR

 .type RTOS_SVC_mailBoxReceive, %function
 RTOS_SVC_mailBoxReceive:
 	SVC 15
 	BX LR
