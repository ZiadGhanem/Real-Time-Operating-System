/*
******************************************************************************
File:     main.c
Info:     Generated by Atollic TrueSTUDIO(R) 9.3.0   2021-05-15

The MIT License (MIT)
Copyright (c) 2019 STMicroelectronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************
*/

/* Includes */
#include <stdint.h>
#include "stm32f4xx.h"
#include "rtos.h"

RTOS_thread_t thread[2];
RTOS_stack_t stack[2];
RTOS_semaphore_t semaphore;

void func_1(void)
{
	while(1)
	{
		GPIO_ToggleBits(GPIOG, (1 << 13));
		RTOS_SVC_threadDelay(500);
		RTOS_SVC_semaphoreSignal(&semaphore);
	}
}

void func_2(void)
{
	while(1)
	{
		GPIO_ToggleBits(GPIOG, (1 << 14));
		RTOS_SVC_semaphoreWait(&semaphore);
	}
}

int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_Init(GPIOG, &(GPIO_InitTypeDef){
			(1 << 13) | (1 << 14),
			GPIO_Mode_OUT,
			GPIO_Speed_50MHz,
			GPIO_OType_PP,
			GPIO_PuPd_NOPULL
	  });


	RTOS_init();

	RTOS_SVC_threadCreate(&thread[0], &stack[0], func_1, 1);
	RTOS_SVC_threadCreate(&thread[1], &stack[1], func_2, 1);

	RTOS_SVC_semaphoreInit(&semaphore, 0);

	RTOS_SVC_schedulerStart();

	while(1);
}
