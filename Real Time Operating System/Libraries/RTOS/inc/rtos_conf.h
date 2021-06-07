/*
 * rtos_conf.h
 *
 *  Created on: May 17, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_CONF_H_
#define RTOS_INC_RTOS_CONF_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Scheduler frequency */
#define SYSTICK_FREQUENCY	1000UL
/* Number of allowed priority levels */
#define MAX_PRIORITY_LEVEL	16UL
/* Maximum process stack size in double words*/
#define MAX_STACK_SIZE	1024UL


#endif /* RTOS_INC_RTOS_CONF_H_ */
