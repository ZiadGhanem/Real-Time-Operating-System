/*
 * rtos_list.c
 *
 *  Created on: May 15, 2021
 *      Author: ziadyasser
 */

#include "rtos.h"

/*
 * This function initializes the list
 * Inputs:
 * 	pList -> The RTOS list
 * Return:
 * 	None
 */
void RTOS_listInit(RTOS_list_t* pList)
{
	ASSERT(pList != NULL);

	/* Set the the current index as end item */
	pList->pIndex = (RTOS_listItem_t*) &(pList->endItem);

	/* Set the next and previous of the end item as itself */
	pList->endItem.pNext = (RTOS_listItem_t*) &(pList->endItem);
	pList->endItem.pPrev = (RTOS_listItem_t*) &(pList->endItem);

	/* Set the number of items in the list to zero */
	pList->numListItems = 0;
}

/*
 * This function adds an item after the current index
 * Inputs:
 * 	pList -> The RTOS list
 * 	pListItem -> The RTOS list item to be inserted
 * Return:
 * 	None
 */
void RTOS_listAppend(RTOS_list_t* pList, RTOS_listItem_t* pListItem)
{
	ASSERT(pList != NULL);
	ASSERT(pListItem != NULL);

	/* Make the next of the new item the next of the current index item of list */
	pListItem->pNext = pList->pIndex->pNext;
	/* Make the previous of the new item the current index item of the list */
	pListItem->pPrev = pList->pIndex;
	/* Make the previous of the item that was the next the new item */
	pListItem->pNext->pPrev = pListItem;
	/* Make the next item of the current index item the new list item */
	pList->pIndex->pNext = pListItem;
	/* Update the current index to the new item */
	pList->pIndex = pListItem;
	/* Set the list of the new item as the list */
	pListItem->pList = pList;
	/* Increment the number of items in the list */
	pList->numListItems++;
}

/*
 * This function removes an item from a list
 * Inputs:
 * 	pListItem -> The RTOS list item to be removed
 * Return:
 * 	None
 */
void RTOS_listRemove(RTOS_listItem_t* pListItem)
{
	ASSERT(pListItem != NULL);
	RTOS_list_t* pList = pListItem->pList;
	ASSERT(pListItem != (RTOS_listItem_t*) &pList->endItem);

	/* Make the next of previous item the next item */
	pListItem->pPrev->pNext = pListItem->pNext;
	/* Make the previous of the next item the previous item */
	pListItem->pNext->pPrev = pListItem->pPrev;
	/* Check if the removed item is the index of the list */
	if(pListItem == pList->pIndex)
	{
		/* Make the previous item the new index */
		pList->pIndex = pListItem->pPrev;
	}
	else
	{

	}
	/* Make the list of the item NULL */
	pListItem->pList = NULL;
	/* Increment the number of items in the list */
	pList->numListItems--;
}

