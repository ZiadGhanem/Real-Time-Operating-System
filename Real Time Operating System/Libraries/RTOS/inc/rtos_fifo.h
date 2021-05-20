/*
 * rtos_fifo.h
 *
 *  Created on: May 20, 2021
 *      Author: ziadyasser
 */

#ifndef RTOS_INC_RTOS_FIFO_H_
#define RTOS_INC_RTOS_FIFO_H_


/** RTOS list container
 * pPut -> Pointer to the location where the last item was added
 * pGet -> Pointer to the location to get the item
 * endItem -> End of list item object
 * numListItems -> Number of items in the list
 */
typedef struct{
	RTOS_listItem_t* pPut;
	RTOS_listItem_t* pGet;
	RTOS_listEndItem_t endItem;
	uint32_t numFIFOItems;
}RTOS_FIFO_t;

void RTOS_FIFOInit(RTOS_FIFO_t* pFIFO);
void RTOS_FIFOAppend(RTOS_FIFO_t* pFIFO, RTOS_listItem_t* pListItem);
RTOS_listItem_t* RTOS_FIFORemove(RTOS_FIFO_t* pFIFO);


#endif /* RTOS_INC_RTOS_FIFO_H_ */
