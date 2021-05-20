/*
 * rtos_fifo.c
 *
 *  Created on: May 20, 2021
 *      Author: ziadyasser
 */

#include "rtos.h"

/*
 * This function initializes the FIFO queue
 * Inputs:
 *  pFIFO -> Pointer to the FIFO queue
 * Return:
 * 	None
 */
void RTOS_FIFOInit(RTOS_FIFO_t* pFIFO)
{
	ASSERT(pFIFO != NULL);

	/* Set the the indices as end item */
	pFIFO->pPut = (RTOS_listItem_t*) &(pFIFO->endItem);
	pFIFO->pGet = (RTOS_listItem_t*) &(pFIFO->endItem);

	/* Set the next and previous of the end item as itself */
	pFIFO->endItem.pNext = (RTOS_listItem_t*) &(pFIFO->endItem);
	pFIFO->endItem.pPrev = (RTOS_listItem_t*) &(pFIFO->endItem);

	/* Set the number of items in the list to zero */
	pFIFO->numFIFOItems = 0;
}

/*
 * This function adds an item to the FIFO queue
 * Inputs:
 *  pFIFO -> Pointer to the FIFO queue
 * Return:
 * 	None
 */
void RTOS_FIFOAppend(RTOS_FIFO_t* pFIFO, RTOS_listItem_t* pListItem)
{
	ASSERT(pFIFO != NULL);
	ASSERT(pListItem != NULL);

	/* Make the next of the new item the next of the current index item of list */
	pListItem->pNext = pFIFO->pPut->pNext;
	/* Make the previous of the new item the current index item of the list */
	pListItem->pPrev = pFIFO->pPut;
	/* Make the previous of the item that was the next the new item */
	pListItem->pNext->pPrev = pListItem;
	/* Make the next item of the current index item the new list item */
	pFIFO->pPut->pNext = pListItem;
	/* Update the current index to the new item */
	pFIFO->pPut = pListItem;
	/* Set the list of the new item as the FIFO */
	pListItem->pList = pFIFO;
	/* Increment the number of items in the FIFO */
	pFIFO->numFIFOItems++;
}


/*
 * This function removes an item from the FIFO queue
 * Inputs:
 *  pFIFO -> Pointer to the FIFO queue
 * Return:
 * 	FIFO queue removed item
 */
RTOS_listItem_t* RTOS_FIFORemove(RTOS_FIFO_t* pFIFO)
{
	ASSERT(pFIFO != NULL);
	ASSERT(pFIFO->numFIFOItems > 0);

	// Do not remove end item
	if(pFIFO->pGet == (RTOS_listItem_t*) &(pFIFO->endItem))
	{
		pFIFO->pGet = pFIFO->pGet->pNext;
	}
	else
	{

	}

	RTOS_listItem_t* pListItem = pFIFO->pGet;
	/* Make the next of previous item the next item */
	pListItem->pPrev->pNext = pListItem->pNext;
	/* Make the previous of the next item the previous item */
	pListItem->pNext->pPrev = pListItem->pPrev;
	/* Increment the get pointer */
	pFIFO->pGet = pFIFO->pGet->pNext;
	/* Set the FIFO of the item as NULL */
	pListItem->pList = NULL;
	/* Decrement the number of items in the FIFO */
	pFIFO->numFIFOItems--;
	/* Handle the case where the FIFO became empty */
	if(pFIFO->numFIFOItems == 0)
	{
		pFIFO->pPut = (RTOS_listItem_t*) &(pFIFO->endItem);
		pFIFO->pGet = pFIFO->pPut;
	}
	else
	{

	}

	return pListItem;

}
