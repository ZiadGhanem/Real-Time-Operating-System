/*
 * rtos_scheduler.h
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_SCHEDULER_H_
#define RTOS_INC_RTOS_SCHEDULER_H_


extern void RTOS_SVC_schedulerStart(void);
void RTOS_schedulerStart(void);
void RTOS_SysTick_Handler(void);
uint32_t RTOS_schedulerGetSystickCount(void);


#endif /* RTOS_INC_RTOS_SCHEDULER_H_ */
