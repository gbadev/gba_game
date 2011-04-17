//sprite.h
//poop
#include "stack.h"

void sprite_init();

void sprite_setPos( int, int , int  );
void sprite_setTilePos( int index, int x, int y );
void sprite_draw( int , int , int  );
void sprite_updateAll();
void sprite_moveUp(int);
void sprite_moveDown(int);
void sprite_moveLeft(int);
void sprite_moveRight(int);

//needs to be moved to sprite.h
void sprite_move ( int i );

int PathFinder ( Stack * sptr, int this_x, int this_y, int end_x, int end_y, int moves );
void sprite_findPath(int i, int start_x, int start_y, int end_x, int end_y );

void sprite_init()
//I:	none
//O:	sprite stuff is initialized
//R:	none
{
	int n, i;
	//copy in sprite palette
    for(n = 0; n < 256; n++)
        SpritePal[n] = robotPalette[n];

    //move sprites offscreen
    for(n = 0; n < 128; n++)
    {
		mysprites[n].x = -16;
		mysprites[n].y = -16;
        sprites[n].attribute0 = -16; //using copy of OAM
        sprites[n].attribute1 = -16;
    }
	
	//last spritedata index
    int max = 16 * 16 * 48 / 2;
    
	//read in sprite data
    for(n = 0; n < max; n++)
        SpriteData[n] = robotData[n];

    
    UpdateSpriteMemory();
    
	//TODO : remove this crap
    sprite_setTilePos ( 0, 0, 0 );
	sprite_setTilePos ( 1, 9, 9 );

	
    sprites[0].attribute2 = 0;
	sprites[1].attribute2 = 8;
	sprites[127].attribute2 = 0;
    UpdateSpriteMemory();
}

void sprite_setPos( int index, int x, int y )
//I:	a sprite index, a position given in x and y coordinates
//O:	the ith sprites x and y positions are set to x and y
//R:	none
{
	mysprites[index].x = x;
	mysprites[index].y = y;
}

void sprite_setTilePos( int index, int x, int y )
//I:	a sprite index, a position given in x and y TILE coordinates
//O:	the ith sprites x and y positions are set to x and y TILE coords
//R:	none
{
	mysprites[index].x = x * 16;
	mysprites[index].y = y * 16;
}

void sprite_draw( int index, int x, int y )
//I:	a sprite index, and an x and y position
//O:	OAM mem is updated so that sprite i is dipslayed with upper left corner at x,y
//R:	none
{
	sprites[index].attribute1 = SIZE_16 | x;
	sprites[index].attribute0 = COLOR_256 | SQUARE | y;
}


void sprite_updateAll()
//I:	none
//O:	all 128 sprites are updated on the screen.
//		if sprites are not visible on the screen they are moved to offscreen
//R:	none
{
	int i;
	for ( i = 0; i < 128; i++ )
	{
		//if ( isValidMapPosition( mysprites[i].x - myBg.x, mysprites[i].y - myBg.y ) )
		if ( mysprites[i].x - myBg.x >= 0 && mysprites[i].y - myBg.y >= 0 &&
			mysprites[i].x - myBg.x < 240 && mysprites[i].y - myBg.y < 240 )
			sprite_draw ( i, mysprites[i].x - myBg.x, mysprites[i].y - myBg.y );
		else
			sprite_draw ( i, 240, 160);
	}
	UpdateSpriteMemory();
}

void sprite_move ( int i )
//I:	a sprite index
//O:	currently sprite is moved in direction of key presses...
//TODO:	make function work via single cursor input...need to implement findpath function
//R:
{
	
	int x = mysprites[127].x/8;
	int y = mysprites[127].y/8;
	sprite_updateAll();
	
	if ( Pressed(BUTTON_UP ) && y > 0 && myBg.select[( y - 2 ) * myBg.mtw + x ] == showmovesMap[2] )
	{
		sprite_moveUp(127);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_UP ));
	}
	else if ( Pressed(BUTTON_DOWN ) && y < myBg.mth && myBg.select[( y + 2 ) * myBg.mtw + x ] == showmovesMap[2] )
	{
		sprite_moveDown(127);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_DOWN ));
	}
	else if ( Pressed(BUTTON_LEFT ) && x > 0 && myBg.select[ y * myBg.mtw + x - 2 ] == showmovesMap[2] )
	{
		sprite_moveLeft(127);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_LEFT ));
	}
	else if ( Pressed(BUTTON_RIGHT ) && x < myBg.mtw && myBg.select[ y * myBg.mtw + x + 2 ] == showmovesMap[2] )
	{
		sprite_moveRight(127);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_RIGHT ));
	}

}

//whoami(sprite) + direction + slide number
void sprite_moveDown(int i)
{
	volatile int n;
	int j;
	
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].y++;
		sprites[i].attribute2 =  (0*8 + (j%4)*8);
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}

void sprite_moveUp(int i)
{
	volatile int n;
	int j;
	int k = sprites[i].attribute2;
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].y--;
		sprites[i].attribute2 =  (0*8 + (j%4)*8);
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}

void sprite_moveRight(int i)
{
	volatile int n;
	int j;
	int k = sprites[i].attribute2;
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].x++;
		sprites[i].attribute2 = (0*8 + (j%4)*8);
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}
void sprite_moveLeft(int i)
{
	volatile int n;
	int j;
	int k = sprites[i].attribute2;
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].x--;
 		sprites[i].attribute2 = (0*8 + (j%4)*8);
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}



int pathfound;
void sprite_findPath(int i, int start_x, int start_y, int end_x, int end_y )
{
	int start = myBg.movesleft[start_y/2 * myBg.mtw/2 + start_x/2];
	int end = myBg.movesleft[end_y/2 * myBg.mtw/2 + end_x/2];
	int moves = start - end;
	
	Stack s_moves;
	stack_init ( &s_moves );
	
	pathfound = 0;
	int dummy = PathFinder ( &s_moves, start_x, start_y, end_x, end_y, moves );
	
	int curr_move;
	if ( dummy )
	{
		while ( !stack_empty ( & s_moves ) )
		{
			curr_move = stack_pop(&s_moves);
			if ( curr_move == 4 )
			{
				sprite_moveRight(i);
			}
			else if ( curr_move == 1 )
			{
				sprite_moveLeft(i);
			}
			else if ( curr_move == 2 )
			{
				sprite_moveUp(i);
			}
			else if ( curr_move == 3 )
			{
				sprite_moveUp(i);
			}
	
		}
	}
	stack_free ( &s_moves );
	

}

int PathFinder ( Stack* sptr, int this_x, int this_y, int end_x, int end_y, int moves )
{
	if ( !pathfound )
	{
		if ( this_x == end_x && this_y == end_y )
		{	
			pathfound = 1;
			return 1;
		}
		else
		{
			if ( moves)
			{
				if (isValidMapPosition ( this_x+2, this_y ) &&
					PathFinder (sptr,this_x+2, this_y, end_x, end_y, --moves) )
				{
					//4 - right
					stack_push( sptr, 4);
					return 1;
				}
				if (isValidMapPosition ( this_x-2, this_y ) && 
					PathFinder (sptr,this_x-2, this_y, end_x, end_y, --moves) )
				{
					//1 - left
					stack_push( sptr, 1 );
					return 1;
				}
				if (isValidMapPosition ( this_x, this_y+2 ) && 
					PathFinder (sptr,this_x, this_y+2, end_x, end_y, --moves) )
				{
					//2 - up
					stack_push( sptr, 2 );
					return 1;
				}
				if (isValidMapPosition ( this_x, this_y-2 ) && 
					PathFinder (sptr,this_x, this_y-2, end_x, end_y, --moves) )
				{
					//3 - down
					stack_push( sptr, 3 );
					return 1;
				}
			}
		}
	}
	return 0;
}
//poop
