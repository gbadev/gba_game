#include "linked_list.h"

typedef struct turn_node
{

} t_node;



Node *  head = NULL;
void tc_init()
{
	int i;
	for ( i = 0; i < (myBg.numStartZombies+4); i++)
	{
		linked_insert ( &head, i, mysprites[i].nextTurn);
	}
}

/*#define ZOMB_S 6
#define HEAL_S 5
#define SNIP_S 4
#define TANK_S 3
#define GREN_S 2*/

/* g, t, r, g, heal, z, z, g*/

int tc_getNext()
{
	//grab values from head
	int thisTurn = head->val;
	int index = head->index;
	
	//delete old node
	linked_delete(&head, thisTurn); 
	
	//update values
	thisTurn += getSpeed ( index );
	
	
	//add updated node
	linked_insert ( &head, index, thisTurn);
	return index;
}

void tc_updateTc ()
{
	int i = 0;
	Node * thisNode = head;
	
	int start = 16;
	
	for ( i = 0; i < 10 && thisNode; ++i )
	{
		mysprites[i+100].x = start + i * 16;
		mysprites[i+100].y = 0;
		//sprites[i+100].attribute2 = findAnimOffset ( thisNode->index );
		sprite_setImage(i+100, findAnimOffset ( thisNode->index));
		thisNode = thisNode->next;
	}
	UpdateSpriteMemory();	
}
