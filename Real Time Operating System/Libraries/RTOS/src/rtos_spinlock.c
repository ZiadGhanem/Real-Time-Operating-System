/*
 * rtos_spinlock.c
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */

#include "rtos_spinlock.h"

/*
 * This function initializes the spinlock
 * Inputs:
 *  pSpinLock -> Pointer to the SpinLock
 *  value -> Value to which the SpinLock will be initialized
 * Return:
 * 	None
 */
void RTOS_spinInit(RTOS_spinLock_t* pSpinLock, uint32_t value)
{
	ASSERT(pSpinLock != NULL);
	ASSERT((value == RTOS_SPINLOCK_USED) || (value == RTOS_SPINLOCK_FREE));
	/* Set the mutex value */
	pSpinLock->value = value;
}


/*
 * This function locks the SpinLock
 * Inputs:
 *  pMutex -> Pointer to the SpinLock
 * Return:
 * 	States whether the spinlock was acquired successfully or not
 */

RTOS_returnStatus RTOS_spinLock(RTOS_spinLock_t* pSpinLock)
{
	uint8_t terminate = 0;
	uint32_t value;
	RTOS_returnStatus returnStatus = RTOS_DELAY;

	/* Keep trying until this section does not get preempted */
	while(!terminate)
	{
		/* Load the spinlock value */
		value = __LDREXW(&pSpinLock->value);
		/* Check if the spinlock can be acquired */
		if(value == RTOS_SPINLOCK_FREE)
		{
			/* Attempt to store the spinlock */
			if( __STREXW(value, &pSpinLock->value) == 0)
			{
				/* Data Memory Barrier */
				__DMB();
				/* Set the spinlock as acquired */
				returnStatus = RTOS_SUCCESS;
				/* End loop */
				terminate = 1;
			}
			else
			{

			}
		}
		else
		{
			/* End loop */
			terminate = 1;

		}

	}

	return returnStatus;
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
