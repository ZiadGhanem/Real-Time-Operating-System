/*
 * rtos_mutex.h
 *
 *  Created on: Jun 7, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_MUTEX_H_
#define RTOS_INC_RTOS_MUTEX_H_

#define RTOS_MUTEX_USED (int32_t)0
#define RTOS_MUTEX_FREE (int32_t)1

/** RTOS Mutex
 * mutexList -> The mutex blocked list
 * value -> The value of the mutex
 */
typedef struct{
	RTOS_list_t mutexList;
	int32_t value;
}RTOS_mutex_t;

void RTOS_SVC_mutexInit(RTOS_mutex_t* pMutex, int32_t value);
void RTOS_SVC_mutexLock(RTOS_mutex_t* pMutex);
void RTOS_SVC_mutexUnlock(RTOS_mutex_t* pMutex);

void RTOS_mutexInit(RTOS_mutex_t* pMutex, int32_t value);
void RTOS_mutexLock(RTOS_mutex_t* pMutex);
void RTOS_mutexUnlock(RTOS_mutex_t* pMutex);


#endif /* RTOS_INC_RTOS_MUTEX_H_ */
