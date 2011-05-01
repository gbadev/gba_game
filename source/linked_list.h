#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node{
	int index;
	int val;
	struct node *next;
} Node;

void linked_insert(Node **nodePtr, int index, int value);
int linked_delete(Node **nodePtr, int value);
int linked_deleteByIndex (Node **nodePtr, int value);
int linked_isEmpty(Node *nodePtr);
void linked_print(Node *nodePtr);

#endif

