/*
 * rtos_spinlock.c
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */

#include "rtos.h"

/*
 * This function initializes the spinlock
 * Inputs:
 *  pSpinLock -> Pointer to the SpinLock
 *  value -> Value to which the SpinLock will be initialized
 * Return:
 * 	None
 */
void RTOS_spinInit(RTOS_spinLock_t* pSpinLock, int32_t value)
{
	ASSERT((value == RTOS_SPINLOCK_USED) || (value == RTOS_SPINLOCK_FREE));
	/* Set the mutex value */
	pSpinLock->value = value;
}


/*
 * This function locks the SpinLock
 * Inputs:
 *  pMutex -> Pointer to the SpinLock
 * Return:
 * 	None
 */

void RTOS_spinLock(RTOS_spinLock_t* pSpinLock)
{
	while(1)
	{
		/* Read the spinlock value */
		/* If the spinlock is free */
		if((int32_t)__LDREXW((uint32_t*) &pSpinLock->value) == RTOS_SPINLOCK_FREE)
		{
			/* Set the spinlock as used, If we failed to set the spinlock value then just keep trying */
			if(__STREXW(RTOS_SPINLOCK_USED, (uint32_t*) &pSpinLock->value) == 0)
			{
				/* Data Memory Barrier */
				__DMB();
				return;
			}
			else
			{

			}
		}
		else
		{

		}
	}
}


/*
 * This function unlocks the spinlock
 * Inputs:
 *  pSpinLock -> Pointer to the spinlock
 * Return:
 * 	None
 */
void RTOS_spinUnlock(RTOS_spinLock_t* pSpinLock)
{
	/* Data Memory Barrier */
	__DMB();
	/* Unlock the spinlock */
	pSpinLock->value = RTOS_SPINLOCK_FREE;
}
