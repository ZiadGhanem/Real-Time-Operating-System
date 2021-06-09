/*
 * rtos.h
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_H_
#define RTOS_INC_RTOS_H_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "stm32f4xx.h"
#include "rtos_conf.h"

#define MEM32WORD(ADDRESS) (*((volatile uint32_t *)(ADDRESS)))

#define ASSERT(x) \
	if(!(x)) \
	{	\
		__disable_irq();	\
		while(1);	\
	}

typedef enum{
	RTOS_FAIL,
	RTOS_DELAY,
	RTOS_SUCCESS
}RTOS_returnStatus;

void RTOS_init(void);
void RTOS_SVC_Handler_Main(uint32_t* svc_args);

#endif /* RTOS_INC_RTOS_H_ */
