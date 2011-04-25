//sprite.h
//poop
#include <math.h>
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
void sprite_setImage ( int i, int thisImage );

void sprite_draw( int , int , int  );
void sprite_updateAll();
void sprite_moveUp(int);
void sprite_moveDown(int);
void sprite_moveLeft(int);
void sprite_moveRight(int);

//needs to be moved to sprite.h
void sprite_move ( int i );
void sprite_zombie_move ( int curr );

int PathFinder ( Stack * sptr, int this_x, int this_y, int end_x, int end_y,int * pathfound);
void sprite_findPath(int i, int start_x, int start_y, int end_x, int end_y );

int findAnimOffset(int );
int findSpriteIndex(int x, int y);
int findGotHitOffset ( int attk );

#define TANK_START 0
#define ZOMB_START 20
#define SNIP_START 37
#define SPIT_START 54
#define GREN_START 71
#define HEAL_START 88

#define BLOOD_START 114
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
	2176,
	2176,
	2432,
	1536
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
	for(i = 0; i < 7; ++i) //7 == number of sprite files
	{
		for(n = 0; n < the_sprites_lengths[i]; ++n)
		{
			SpriteData[totalSpriteData] = the_sprites[i][n];
			++totalSpriteData;
		}
		
	}

	

    
    UpdateSpriteMemory();
    

 
	//move sprites offscreen
    for(n = 0; n < 128; n++)
    {
		mysprites[n].x = -160;
		mysprites[n].y = -160;
		mysprites[n].isTank = 0;
		mysprites[n].isGren = 0;
		mysprites[n].isSnip = 0;
		mysprites[n].isHeal = 0;
		mysprites[n].isZomb = 0;
		mysprites[n].isSpit = 0;
        mysprites[i].facingDown = 0;
		mysprites[i].facingUp = 0;
		mysprites[i].facingLeft = 0;
		mysprites[i].facingRight = 0;
		mysprites[i].nextTurn = 0;
		mysprites[i].priority = 1;
		//sprites[n].attribute0 = -160; //using copy of OAM
        //sprites[n].attribute1 = -160;
		//sprites[n].attribute2 = n*8;
		sprite_setPos(n, -160, -160);
		sprite_setImage(n, n);
		
    }
	
	mysprites[0].isTank = 1; //init characters
	//sprites[0].attribute2 = 8 * TANK_START;
	sprite_setImage(0, TANK_START);
	mysprites[0].nextTurn = TANK_S;
	
	mysprites[1].isGren = 1;
	//sprites[1].attribute2 = 8 * GREN_START;
	sprite_setImage(1, GREN_START);
	mysprites[1].nextTurn = GRENADE_S;
	
	mysprites[2].isSnip = 1;
	//sprites[2].attribute2 = 8 * SNIP_START;
	sprite_setImage(2, SNIP_START);
	mysprites[2].nextTurn = RANGE_S;
	
	mysprites[3].isHeal = 1;
	//sprites[3].attribute2 = 8 * HEAL_START;
	sprite_setImage(3, HEAL_START);
	mysprites[3].nextTurn = HEAL_S;
	
	mysprites[4].isZomb = 1;
	//sprites[4].attribute2 = 8 * ZOMB_START;
	sprite_setImage(4, ZOMB_START);
	mysprites[4].nextTurn = ZOMBIE_S;
	
	mysprites[5].isSpit = 1;
	//sprites[5].attribute2 = 8 * SPIT_START;
	sprite_setImage(5, SPIT_START);
	mysprites[5].nextTurn = ZOMBIE_S;
	
	//chars init
    sprite_setTilePos ( 0, 0, 0 );
	sprite_setTilePos ( 1, 2, 0 );
	sprite_setTilePos ( 2, 0, 2 );
	sprite_setTilePos ( 3, 0, 8 );
	sprite_setTilePos ( 4, 8, 0 );
	sprite_setTilePos ( 5, 8, 8 );
	

	//sprites[127].attribute2 = 107*8; //init cursor;
	sprite_setImage(127, 107);
	
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

void sprite_setImage ( int i, int thisImageIndex )
{
	
	sprites[i].attribute2 = (thisImageIndex * 8) | PRIORITY(mysprites[i].priority);
	UpdateSpriteMemory();
}

void sprite_draw( int index, int x, int y )
//I:	a sprite index, and an x and y position
//O:	OAM mem is updated so that sprite i is dipslayed with upper left corner at x,y
//R:	none
{
	if ( index < 100 || index > 113 )
	{
		sprites[index].attribute1 = SIZE_16 | x | (mysprites[index].facingLeft * HORIZONTAL_FLIP);
		sprites[index].attribute0 = COLOR_256 | SQUARE | y ;
	}
	else
	{//special case for turn counter shit
		sprites[index].attribute1 = SIZE_16 | (mysprites[index].x);
		sprites[index].attribute0 = COLOR_256 | SQUARE | mysprites[index].y ;
	}
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
	
	if ( Pressed(BUTTON_UP ) && y > 0 && myBg.select[( y - 2 ) * myBg.mtw + x ] == fontMap[64] )
	{
		sprite_moveUp(127);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_UP ));
	}
	else if ( Pressed(BUTTON_DOWN ) && y < myBg.mth && myBg.select[( y + 2 ) * myBg.mtw + x ] == fontMap[64] )
	{
		sprite_moveDown(127);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_DOWN ));
	}
	else if ( Pressed(BUTTON_LEFT ) && x > 0 && myBg.select[ y * myBg.mtw + x - 2 ] == fontMap[64])
	{
		sprite_moveLeft(127);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_LEFT ));
	}
	else if ( Pressed(BUTTON_RIGHT ) && x < myBg.mtw && myBg.select[ y * myBg.mtw + x + 2 ] == fontMap[64] )
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
//I:	a sprite index
//O:	sprite is moved one 16x16 tile down, with animation
//R:	none
{
	volatile int n;
	int j;
	int offset = findAnimOffset(i);
	
	mysprites[i].facingDown = 1;
	mysprites[i].facingUp = 0;
	mysprites[i].facingLeft = 0;
	mysprites[i].facingRight = 0;
	
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].y++;
		if ( i != 127 )
			//sprites[i].attribute2 =  (offset + (j%3)*8);
			sprite_setImage(i, (offset+(j%3)));
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}

void sprite_moveUp(int i)
//I:	a sprite index
//O:	sprite is moved one 16x16 tile up, with animation
//R:	none
{
	volatile int n;
	int j;
	int offset = findAnimOffset(i);
	
	offset += 3;

	mysprites[i].facingDown = 0;
	mysprites[i].facingUp = 1;
	mysprites[i].facingLeft = 0;
	mysprites[i].facingRight = 0;
	
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].y--;
		if ( i != 127 )
			//sprites[i].attribute2 =  (offset + (j%3)*8);
			sprite_setImage(i, (offset+(j%3)));
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}

void sprite_moveRight(int i)
//I:	a sprite index
//O:	sprite is moved one 16x16 tile right, with animation
//R:	none
{
	volatile int n;
	int j;
	int offset = findAnimOffset(i);
	
	offset += 6;
	
	mysprites[i].facingDown = 0;
	mysprites[i].facingUp = 0;
	mysprites[i].facingLeft = 0;
	mysprites[i].facingRight = 1;

	
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].x++;
		//flip that bitch!
		sprites[i].attribute1 = sprites[i].attribute1 | HORIZONTAL_FLIP;
		if ( i != 127 )
			//sprites[i].attribute2 = (offset + (j%4)*8);
			sprite_setImage(i, (offset+(j%4)));
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}
void sprite_moveLeft(int i)
//I:	a sprite index
//O:	sprite is moved one 16x16 tile left, with animation
//R:	none
{
	volatile int n;
	int j;
	int offset = findAnimOffset(i);
	
	offset += 6;

	
	mysprites[i].facingDown = 0;
	mysprites[i].facingUp = 0;
	mysprites[i].facingLeft = 1;
	mysprites[i].facingRight = 0;
	
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].x--;
		if ( i != 127 )
			//sprites[i].attribute2 = (offset + (j%4)*8);
			sprite_setImage(i, (offset+(j%4)));
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}

void sprite_findPath(int i, int start_x, int start_y, int end_x, int end_y )
//I:	a sprite index, the x,y coords for start pos and end pos
//O:	a valid min length path is generated from start to end
//		sprite with index i is move along this path from start to end
//R:	none
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
	
	mysprites[127].x = -160;
	mysprites[127].y = -160;
	bg_clearMoveable();

}

int PathFinder ( Stack* sptr, int this_x, int this_y, int end_x, int end_y, int * pathfound )
//I:	a ptr to a stack, the x,y coords for the current and end pos, an integer flag used to end recursion
//O:		
//R:
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
void sprite_Attack(int index, int x, int y)
//I:	a sprite index, the x,y coordinates of the tile the sprite is attacking
//O:	attack animations are played for attacker and attacked sprites and blood spurts
//		attacked char's hp reduced
//		if attacked char's hp <= 0 character dies
//R:	none
{
	if (!(x == mysprites[index].x && y == mysprites[index].y ))
	{
		//find animation offset
		int offset = findAnimOffset(index);
		if ( mysprites[index].x > x )
		{//attacking left
			mysprites[index].facingLeft = 1;
			mysprites[index].facingRight = 0;
			mysprites[index].facingUp = 0;
			mysprites[index].facingDown = 0;
			offset += 10;
		}
		else if ( mysprites[index].x < x )
		{//attacking right
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 1;
			mysprites[index].facingUp = 0;
			mysprites[index].facingDown = 0;
			offset += 10;
		}
		else if ( mysprites[index].y > y )
		{//attacking up
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 0;
			mysprites[index].facingUp = 1;
			mysprites[index].facingDown = 0;
			offset += 14;
		}
		else if ( mysprites[index].y < y )
		{//attacking down
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 0;
			mysprites[index].facingUp = 0;
			mysprites[index].facingDown = 1;
			offset += 12;
		}
		int j;
		
		//find index of sprite getting attacked
		int attk = findSpriteIndex(x,y);
		u16 prevAttackerFrame = sprites[index].attribute2;
		u16 prevAttackedFrame = sprites[attk].attribute2;
		
		//play attack animations
		for ( j = 0; j < 16; j++)
		{
			//character attacking
			if ( j < 12 )
				//sprites[index].attribute2 =  (offset + (j%2)*8);
				sprite_setImage(index, (offset + ( j % 2) ));
			//attacked character got hit
			if ( j == 3 )
			{
				//sprites[attk].attribute2 = findGotHitOffset ( attk );
				sprite_setImage(attk, findGotHitOffset(attk));
				sprite_setPos(126, mysprites[attk].x, mysprites[attk].y);
				//sprites[126].attribute2 = BLOOD_START * 8;
				sprite_setImage(126, BLOOD_START);
			}
			//spurt blood
			if ( j > 3 )
				//sprites[126].attribute2 = BLOOD_START * 8 + ( j % 3 );
				sprite_setImage(126, BLOOD_START + ( j%3 ));
				
			sprite_updateAll();
			volatile int n;
			for ( n = 0; n < 10000; n++);
		}
		sprites[index].attribute2 = prevAttackerFrame;
		sprites[attk].attribute2 = prevAttackedFrame;
		sprite_setPos(126, -160,-160);
		sprite_setPos(127, -160,-160);

		sprite_updateAll();
		bg_clearMoveable();
	
	}
}

int findSpriteIndex(int x, int y)
//I:	x and y coords
//O:	checks to see if tile at x,y is occupied
//R:	1 if occupied, 0 otherwise
{
	int rval = -1;
	int i;
	for ( i = 0; i < 100 && rval == -1 ; i++)
		if ( x == mysprites[i].x && y == mysprites[i].y )
			rval = i;
	return rval;
}

int findAnimOffset ( int i )
//I:	a sprite index
//O:	offset to reach start frame of given sprite is calculated
//R:	offset to access start frame for given char
{
	int offset = 0;
	if ( mysprites[i].isTank )
		offset = TANK_START;
	else if ( mysprites[i].isGren )
		offset = GREN_START;
	else if ( mysprites[i].isSnip )
		offset = SNIP_START;
	else if ( mysprites[i].isHeal )
		offset = HEAL_START;
	else if( mysprites[i].isZomb )
		offset = ZOMB_START;
	else if ( mysprites[i].isSpit )
		offset = SPIT_START;
	return offset;
}

int findGotHitOffset ( int attk )
//I:	a sprite index
//O:	none
//R:	offset for attacked character 
{
	return findAnimOffset(attk) + 16;	
}
//poop

void sprite_zombie_move ( int curr )
{
	int x, y, k;
	int end_x = mysprites[curr].x, end_y = mysprites[curr].y;
	float minDist = 99999;
	int foo, bar;
	float crap;
	
	bg_drawMoveableArea(curr, getRange(curr));
	
	for ( y = 0; y < myBg.mth; y++)
		for ( x = 0; x < myBg.mtw; x++)
			if ( myBg.select[y*myBg.mtw+x] == fontMap[64] )//check all moveable squares
				for ( k = 0; k < 4; k++)
				{
					float thisDist = 999999;
					/*if ( j > (mysprites[k].x/16) )
						a = j - mysprites[k].x/16;
					else
						a = mysprites[k].x/16 - j;
					
					if ( i > (mysprites[k].y/16) )
						b = i - mysprites[k].y/16;
					else
						b = mysprites[k].y/16 - i;*/
					foo = y - mysprites[k].y/8;
					bar = x - mysprites[k].x/8;
					
					crap = (foo*foo + bar*bar);
					
					thisDist = sqrtf(crap);
					
					if ( thisDist <= minDist )
					{
						minDist = thisDist;
						end_x = x*8;
						end_y = y*8;
					}
				}
	sprite_findPath ( curr, mysprites[curr].x, mysprites[curr].y, end_x, end_y );
	
}