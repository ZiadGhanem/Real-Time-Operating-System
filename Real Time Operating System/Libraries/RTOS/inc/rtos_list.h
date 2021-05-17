/*
 * rtos_list.h
 *
 *  Created on: May 15, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_LIST_H_
#define RTOS_INC_RTOS_LIST_H_

#include <stdint.h>

/** RTOS list item
 * pNext -> Pointer to the next item in the list
 * pPrev -> Pointer to the previous item in the list
 * pThread -> Pointer to the thread of this item
 */
struct listItem_t{
	struct listItem_t* pNext;
	struct listItem_t* pPrev;
	void* pList;
	void* pThread;
}listItem_t;

typedef struct listItem_t RTOS_listItem_t;

/** RTOS end of list item
 * pNext -> Pointer to the next item in the list
 * pPrev -> Pointer to the previous item in the list
 */
typedef struct{
	RTOS_listItem_t* pNext;
	RTOS_listItem_t* pPrev;
}RTOS_listEndItem_t;

/** RTOS list container
 * pIndex -> Pointer to the current index of the list
 * endItem -> End of list item object
 * numListItems -> Number of items in the list
 */
typedef struct{
	RTOS_listItem_t* pIndex;
	RTOS_listEndItem_t endItem;
	int32_t numListItems;
}RTOS_list_t;


void RTOS_listInit(RTOS_list_t* RTOS_list);
void RTOS_listAppend(RTOS_list_t* RTOS_list, RTOS_listItem_t* RTOS_listItem);
void RTOS_listRemove(RTOS_list_t* RTOS_list, RTOS_listItem_t* RTOS_listItem);



#endif /* RTOS_INC_RTOS_LIST_H_ */