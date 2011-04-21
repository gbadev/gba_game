//sprite.h
//poop
#include "stack.h"
#include "graphics/sprites/master_pal.h"
#include "graphics/sprites/animations.h"
#include "graphics/sprites/dave_tank_sprites.h"
#include "graphics/sprites/dave_zombie_sprites.h"
#include "graphics/sprites/scott_char.h"
#include "graphics/sprites/scott_zomb.h"
#include "graphics/sprites/sean_grenadier.h"
#include "graphics/sprites/thomas_all_chars.h"

#include "graphics/sprites/master_pal.c"
#include "graphics/sprites/animations.c"
#include "graphics/sprites/dave_tank_sprites.c"
#include "graphics/sprites/dave_zombie_sprites.c"
#include "graphics/sprites/scott_char.c"
#include "graphics/sprites/scott_zomb.c"
#include "graphics/sprites/sean_grenadier.c"
#include "graphics/sprites/thomas_all_chars.c"

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

int PathFinder ( Stack * sptr, int this_x, int this_y, int end_x, int end_y,int * pathfound);
void sprite_findPath(int i, int start_x, int start_y, int end_x, int end_y );



/*
	0 dave_tank_sprites - 2560
	1 dave_zombie_sprites - 2176
	2 scott_char - 2176
	3 scott_zomb - 4352
	4 sean_grenadier - 2048
	5 thomas_all_chars - 2304
	6 animations - 3072
	****Append Tiles to the above to get array name
	****Append TilesLen to get length of tiles
*/


/*
	Below was an attempt at an object for data of sprite.
	It kept giving me a stupid error when I compiled so I guess
	I will fall back to the array or array.  Same diff I guess.
	Dave, If you like it and want to spend a few minutes trying it
	go ahead, else you can delete it.  Thanks, Thomas Cross.
*/
/********************************************************************
struct spriteDataObj {
	const unsigned short data;	//sprite
	int length;					//elements in data array
}the_sprites[7];
the_sprites[0].data++;
//the_sprites[0] = dave_tank_spritesTiles;
//the_sprites[0].length = 16128;
*********************************************************************/

const unsigned int the_sprites_lengths[7] = {
	2560,
	2176,
	2176,
	4352,
	2048,
	2304,
	3072
};
const unsigned short *the_sprites[7] = {
	dave_tank_spritesTiles,
	dave_zombie_spritesTiles,
	scott_charTiles,
	scott_zombTiles,
	sean_grenadierTiles,
	thomas_all_charsTiles,
	animationsTiles
};

void sprite_init()
//I:	none
//O:	sprite stuff is initialized
//R:	none
{
	int n, i;
	//copy in sprite palette
    for(n = 0; n < 42; n++)  //Changed from 256 to 42 due to grit processing.
        SpritePal[n] = master_palPal[n];

    //move sprites offscreen
    for(n = 0; n < 128; n++)
    {
		mysprites[n].x = -160;
		mysprites[n].y = -160;
        sprites[n].attribute0 = -160; //using copy of OAM
        sprites[n].attribute1 = -160;
    }
	
	/*OLD WAY
	//last spritedata index
    int max = 16 * 16 * 48 / 2;
	max = the_sprites_lengths[0];
	for(n = 0; n < max; ++n)
		SpriteData[n] = the_sprites[0][n];
    */
	

	//This loop does not include sprite data.  
	//Because it seems to be corrupting the sprite.
	//read in sprite data
	int totalSpriteData = 0;
	for(i = 0; i < 6; ++i) //7 == number of sprite files
	{
		for(n = 0; n < the_sprites_lengths[i]; ++n)
		{
			SpriteData[totalSpriteData] = the_sprites[i][n];
			++totalSpriteData;
		}
		
	}

	

    
    UpdateSpriteMemory();
    
	//TODO : remove this crap
    sprite_setTilePos ( 0, 0, 0 );
	sprite_setTilePos ( 1, 2, 0 );
	sprite_setTilePos ( 2, 0, 2 );

	
    sprites[0].attribute2 = 0;
	sprites[1].attribute2 = 8;
	sprites[2].attribute2 = 16;
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




void sprite_findPath(int i, int start_x, int start_y, int end_x, int end_y )
{
	//convert to movesleft tile indexes
	start_x/=16;
	start_y/=16;
	end_x/=16;
	end_y/=16;
	
	Stack s_moves;
	stack_init ( &s_moves );
	
	int pathfound = 0;
	int dummy = PathFinder ( &s_moves, start_x, start_y, end_x, end_y, &pathfound );
	
	int curr_move; 
	if ( dummy  )
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
				sprite_moveDown(i);
			}
	
		}
	}
	stack_free ( &s_moves );
	

}

int PathFinder ( Stack* sptr, int this_x, int this_y, int end_x, int end_y, int * pathfound )
{
	if ( !(*pathfound) )
	{
		if ( this_x == end_x && this_y == end_y )
		{	
			*pathfound = 1;
			return 1;
		}
		else
		{
			//if ( moves >= 0)
			//{
				if ((this_x + 1) >= 0 && (this_x + 1) < myBg.mtw/2 && this_y >= 0 && this_y < myBg.mth/2 &&
					myBg.movesleft[this_y*myBg.mtw/2 + this_x ] == ( myBg.movesleft[(this_y)*myBg.mtw/2 + (this_x+1) ] +1)&&
					PathFinder (sptr,this_x+1, this_y, end_x, end_y,pathfound) )
				{
					//4 - right
					stack_push( sptr, 4);
					return 1;
				}
				if ((this_x - 1 ) >= 0 && (this_x - 1 )< myBg.mtw/2 && this_y >= 0 && this_y < myBg.mth/2 &&
					myBg.movesleft[this_y*myBg.mtw/2 + this_x ] == (myBg.movesleft[(this_y)*myBg.mtw/2 + (this_x-1) ] +1) &&
					PathFinder (sptr,this_x-1, this_y, end_x, end_y,pathfound) )
				{
					//1 - left
					stack_push( sptr, 1 );
					return 1;
				}
				if (this_x >= 0 && this_x < myBg.mtw/2 && (this_y -1) >= 0 && (this_y -1)  < myBg.mth/2 &&
					myBg.movesleft[this_y*myBg.mtw/2 + this_x ] == (myBg.movesleft[(this_y-1)*myBg.mtw/2 + (this_x) ]+1) &&
					PathFinder (sptr,this_x, this_y-1, end_x, end_y,pathfound) )
				{
					//2 - up
					stack_push( sptr, 2 );
					return 1;
				}
				if (this_x >= 0 && this_x < myBg.mtw/2 && (this_y + 1) >= 0 && (this_y +1) < myBg.mth/2 &&
					myBg.movesleft[this_y*myBg.mtw/2 + this_x ] == (myBg.movesleft[(this_y+1)*myBg.mtw/2 + (this_x) ]+1)&&
					PathFinder (sptr,this_x, this_y+1, end_x, end_y,pathfound) )
				{
					//3 - down
					stack_push( sptr, 3 );
					return 1;
				}
			//}
		}
	}
	return 0;
}
//poop
