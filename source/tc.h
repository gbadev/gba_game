#include "linked_list.h"

typedef struct turn_node
{

} t_node;

int compare_t_nodes ( void * a, void * b );

int compare_t_nodes ( void * a, void * b )
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

Node *  head = NULL;
void tc_init()
{
	int i;
	for ( i = 0; i < (myBg.numStartZombies+4); i++)
	{
		linked_insert ( &head, i, mysprites[i].nextTurn, &compare_t_nodes);
	}
}

/*#define ZOMBIE_S 6
#define HEAL_S 5
#define RANGE_S 4
#define TANK_S 3
#define GRENADE_S 2*/

/* g, t, r, g, heal, z, z, g*/
int getSpeed ( int index )
{
	int rval = 100;
	if ( mysprites[index].isTank )
		rval = TANK_S;
	else if ( mysprites[index].isGren )
		rval = GRENADE_S;
	else if ( mysprites[index].isSnip )
		rval = RANGE_S;
	else if ( mysprites[index].isHeal )
		rval = HEAL_S;
	else if ( mysprites[index].isZomb )
		rval = ZOMBIE_S;
	else if ( mysprites[index].isSpit )
		rval = ZOMBIE_S;
	return rval;
}

int getRange(int index)
{
	int rval = 1;
	if ( mysprites[index].isTank )
		rval = TANK_M;
	else if ( mysprites[index].isGren )
		rval = GRENADE_M;
	else if ( mysprites[index].isSnip )
		rval = RANGE_M;
	else if ( mysprites[index].isHeal )
		rval = HEAL_M;
	else if ( mysprites[index].isZomb )
		rval = ZOMBIE_M;
	else if ( mysprites[index].isSpit )
		rval = ZOMBIE_M;
	return rval;
}
	

int tc_getNext()
{
	//grab values from head
	int thisTurn = head->val;
	int index = head->index;
	
	//delete old node
	linked_delete(&head, thisTurn, &compare_t_nodes); 
	
	//update values
	thisTurn += getSpeed ( index );
	
	
	//add updated node
	linked_insert ( &head, index, thisTurn, &compare_t_nodes);
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
		sprites[i+100].attribute2 = findAnimOffset ( thisNode->index );
		thisNode = thisNode->next;
	}
	UpdateSpriteMemory();	
}
