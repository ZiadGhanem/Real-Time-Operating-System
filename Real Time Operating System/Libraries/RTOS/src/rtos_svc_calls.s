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


 .text

 .type RTOS_SVC_schedulerStart, %function
 RTOS_SVC_schedulerStart:
	SVC 0
	BX LR

 .section .text.RTOS_SVC_threadCreate
 .type RTOS_SVC_threadCreate, %function
 RTOS_SVC_threadCreate:
	SVC 1
	BX LR
