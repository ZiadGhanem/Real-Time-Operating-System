/*
 * rtos.h
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_H_
#define RTOS_INC_RTOS_H_

#include "stm32f4xx.h"
#include "rtos_conf.h"
#include "rtos_list.h"
#include "rtos_thread.h"

#define MEM32WORD(ADDRESS) (*((volatile uint32_t *)(ADDRESS)))

#define ASSERT(x) \
	if(!(x)) \
	{	\
		__disable_irq();	\
		while(1);	\
	}


#endif /* RTOS_INC_RTOS_H_ */
