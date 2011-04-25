#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int compare_ints ( int a, int  b );

void linked_insert(Node **nodePtr, int index, int value)
{
	Node *newNode;
	Node *previous;
	Node *current;

	newNode = malloc(sizeof(Node));

	if(newNode != NULL) /*make sure memory was available*/
	{
		newNode -> index = index;
		newNode -> val = value;
		newNode -> next = NULL;

		previous = NULL;
		current = *nodePtr;

		/*Traverses list until end or larger data is found
		  If order doesn't matter, only append to the end
		  by removing second condition*/
		while(current != NULL && compare_ints(value, current->val) >= 0) /* value > current -> data */
		{
			previous = current;
			current = current -> next;
		}

		if(previous == NULL)
		{
			newNode -> next = *nodePtr;
			*nodePtr = newNode;
		}
		else
		{
			previous -> next = newNode;
			newNode -> next = current;
		}
	}
	else
	{
		/* printf("%d not inserted\n", value); */
	}
}

int linked_delete(Node **nodePtr, int value )
{
	Node *previous;
	Node *current;
	Node *temp;

	/*First node is to be deleted*/
	if(compare_ints(value, (*nodePtr)->val) == 0) /* value == (*nodePtr) -> data) */
	{
		temp = *nodePtr; /*retain pointer to memory to be freed*/
		*nodePtr = (*nodePtr) -> next;
        free(temp);
		return 1;
	}
	else
	{	
		/*Must retain the previous to connect it
		  to the node after the one that is
		  going to be deleted*/
		previous = *nodePtr;
		current = (*nodePtr) -> next; /*parens needed to force correct order of ops*/

		/*loop through nodes until the value is found*/
		while(current != NULL && compare_ints((current -> val), value) != 0) /* current -> data != value */
		{
			previous = current;
			current = current -> next;
		}

		/*delete current node if not NULL
		  if node is NULL then the value
		  was not in list*/
		if(current != NULL)
		{
			temp = current; /*retain ponter to memory to be freed*/
			previous -> next = current -> next;
            free(temp);
			return 1;
		}
	}	
	/*value not found, return -1 to show
	  no node removed*/
	return -1;

}

int linked_isEmpty(Node *nodePtr)
{
	return nodePtr == NULL;
}

void linked_print(Node *nodePtr)
{
	if(linked_isEmpty(nodePtr))
	{
		//printf("List is empty\n");
	}
	else
	{
		while(nodePtr != NULL)
		{
			/* printf("%d --> ", nodePtr -> data); */
			nodePtr = nodePtr -> next;
		}

		//printf("NULL\n");
	}
}


int compare_ints ( int  a, int  b )
{
	int result;
//	int A, B;
//	A = (( t_node * ) a)->val;
//	B = (( t_node * ) b)->val;
	if ( a == b )
		result = 0;
	else if ( a < b )
		result = -1;
	else 
		result = 1;
	return result;
}


