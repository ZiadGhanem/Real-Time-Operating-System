/*
 * rtos_list.c
 *
 *  Created on: May 15, 2021
 *      Author: ziadyasser
 */

#include "rtos_list.h"

/*
 * This function initializes the list
 * Inputs:
 * 	RTOS_list -> The RTOS list
 * Return:
 * 	None
 */
void RTOS_listInit(RTOS_list_t* RTOS_list)
{
	/* Set the the current index as end item */
	RTOS_list->pIndex = (RTOS_listItem_t*) &(RTOS_list->endItem);

	/* Set the next and previous of the end item as itself */
	RTOS_list->endItem.pNext = (RTOS_listItem_t*) &(RTOS_list->endItem);
	RTOS_list->endItem.pPrev = (RTOS_listItem_t*) &(RTOS_list->endItem);

	/* Set the number of items in the list to zero */
	RTOS_list->numListItems = 0;
}

/*
 * This function adds an item after the current index
 * Inputs:
 * 	RTOS_list -> The RTOS list
 * 	RTOS_listItem -> The RTOS list item to be inserted
 * Return:
 * 	None
 */
void RTOS_listAppend(RTOS_list_t* RTOS_list, RTOS_listItem_t* RTOS_listItem)
{
	/* Make the next of the new item the next of the current index item of list */
	RTOS_listItem->pNext = RTOS_list->pIndex->pNext;
	/* Make the previous of the new item the current index item of the list */
	RTOS_listItem->pPrev = RTOS_list->pIndex;
	/* Make the previous of the item that was the next the new item */
	RTOS_listItem->pNext->pPrev = RTOS_listItem;
	/* Make the next item of the current index item the new list item */
	RTOS_list->pIndex->pNext = RTOS_listItem;
	/* Update the current index to the new item */
	RTOS_list->pIndex = RTOS_listItem;
	/* Set the list of the new item as the list */
	RTOS_listItem->pList = RTOS_list;
	/* Increment the number of items in the list */
	RTOS_list->numListItems++;
}

/*
 * This function removes an item from a list
 * Inputs:
 * 	RTOS_list -> The RTOS list
 * 	RTOS_listItem -> The RTOS list item to be removed
 * Return:
 * 	None
 */
void RTOS_listRemove(RTOS_list_t* RTOS_list, RTOS_listItem_t* RTOS_listItem)
{
	/* Make the next of previous item the next item */
	RTOS_listItem->pPrev->pNext = RTOS_listItem->pNext;
	/* Make the previous of the next item the previous item */
	RTOS_listItem->pNext->pPrev = RTOS_listItem->pPrev;
	/* Check if the removed item is the index of the list */
	if(RTOS_listItem == RTOS_list->pIndex)
	{
		/* Make the previous item the new index */
		RTOS_list->pIndex = RTOS_listItem->pPrev;
	}
	else
	{

	}
	/* Increment the number of items in the list */
	RTOS_list->numListItems--;
}

