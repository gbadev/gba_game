#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node{
	int index;
	int val;
	struct node *next;
} Node;

void linked_insert(Node **nodePtr, int index, int value, int (*compare)(const void *, const void *));
int linked_delete(Node **nodePtr, int value, int (*compare)(const void *, const void *));
int linked_isEmpty(Node *nodePtr);
void linked_print(Node *nodePtr);

#endif

