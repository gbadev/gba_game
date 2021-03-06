//sprite.h
//poop
#include <math.h>

#include "master_pal_spr.h"
#include "animations.h"
#include "dave_tank_sprites.h"
#include "dave_zombie_Sprites.h"
#include "scott_char.h"
#include "scott_zomb.h"
#include "sean_grenadier.h"
#include "thomas_all_chars.h"



extern int getRange(int index);
extern Node *  head;
extern void ui_updateStatus();
extern void tc_updateTc();

//init sprite memory
void sprite_init();

//init player characters
void sprite_init_tank( int x, int y );
void sprite_init_gren( int x, int y );
void sprite_init_snip( int x, int y );
void sprite_init_heal( int x, int y );
//init zombies
void sprite_init_zomb(int index, int x, int y );
void sprite_init_spit( int index, int x, int y );

void sprite_setPos( int, int , int  );
void sprite_setTilePos( int index, int x, int y );
void sprite_setImage ( int i, int thisImage );
void sprite_resetImage ( int i );

void sprite_draw( int , int , int  );
void sprite_updateAll();
void sprite_moveUp(int);
void sprite_moveDown(int);
void sprite_moveLeft(int);
void sprite_moveRight(int);

void sprite_moveCursor ( int i );
int PathFinder ( Stack * sptr, int this_x, int this_y, int end_x, int end_y,int * pathfound);
void sprite_findPath(int i, int start_x, int start_y, int end_x, int end_y );

void sprite_Attack(int index, int x, int y);

int findAnimOffset(int );
int findSpriteIndex(int x, int y);
int findGotHitOffset ( int attk );

void sprite_zombie_move ( int curr );
int sprite_zombie_attack(int curr);
int sprite_zombie_spit ( int curr );

//special attacks
void sprite_gren_special ( int curr, int end_x, int end_y );
void sprite_snip_special ( int index, int x, int y );
void sprite_tank_special ( int curr, int end_x, int end_y );
void sprite_heal_special ( int index, int x, int y );

int getTankSpecialOffset(int index);
void sprite_die ( int index );


#define TANK_START 0
#define ZOMB_START 20
#define SNIP_START 37
#define SPIT_START 54
#define GREN_START 71
#define HEAL_START 88

#define GRENADE_START 108
#define SPITBALL_START 111
#define BLOOD_START 114
#define HEALANIM_START 117
#define EXPLOSION_START 119

/*
	0 dave_tank_sprites - 2560
	1 dave_zombie_Sprites - 2176
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
	1792
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
        SpritePal[n] = master_pal_sprPal[n];

   
	
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
		mysprites[n].alive = 0;
        mysprites[n].facingDown = 0;
		mysprites[n].facingUp = 0;
		mysprites[n].facingLeft = 0;
		mysprites[n].facingRight = 0;
		mysprites[n].nextTurn = 0;
		mysprites[n].priority = 2;
		mysprites[n].hp=0;
		mysprites[n].maxHp = 0;
		mysprites[n].lastImage = -1;
		sprite_setPos(n, -160, -160);
		sprite_setImage(n, n);
		
		mysprites[n].attkCounter = 0;
		
		
    }
	
	for ( n = 100; n < 115; n++ )
	{
		mysprites[n].priority = 0;
	}
	
	for ( n = 115; n < 128; n++ )
	{
		mysprites[n].priority = 1;
	}
	
		//init cursor;
	sprite_setImage(127, 107);
	
    UpdateSpriteMemory();
}


//init player characters
void sprite_init_tank( int x, int y )
{
	sprite_setTilePos(0, x, y);
	mysprites[0].isTank = 1;
    mysprites[0].facingDown = 1;
	mysprites[0].nextTurn = TANK_S;
	mysprites[0].hp = TANK_L;
	mysprites[0].maxHp = TANK_L;
	mysprites[0].alive = 1;
	sprite_setImage(0, findAnimOffset(0));
	sprite_setImage(0, findAnimOffset(0));
	//mysprites[0].lastImage = (TANK_START * 8) | PRIORITY(mysprites[0].priority);
}
void sprite_init_gren( int x, int y )
{
	sprite_setTilePos(1, x, y);
	mysprites[1].isGren = 1;
    mysprites[1].facingDown = 1;
	mysprites[1].nextTurn = GREN_S;
	mysprites[1].hp = GREN_L;
	mysprites[1].maxHp = GREN_L;
	mysprites[1].alive = 1;
	sprite_setImage(1, findAnimOffset(1));	
	sprite_setImage(1, findAnimOffset(1));	
	//mysprites[1].lastImage = (GREN_START * 8) | PRIORITY(mysprites[1].priority);
}
void sprite_init_snip( int x, int y )
{
	sprite_setTilePos(2, x, y);
	mysprites[2].isSnip = 1;
    mysprites[2].facingDown = 1;
	mysprites[2].nextTurn = SNIP_S;
	mysprites[2].hp = SNIP_L;
	mysprites[2].maxHp = SNIP_L;
	mysprites[2].alive = 1;
	sprite_setImage(2, findAnimOffset(2));	
	sprite_setImage(2, findAnimOffset(2));	
	//mysprites[2].lastImage = (SNIP_START * 8) | PRIORITY(mysprites[2].priority);
}
void sprite_init_heal( int x, int y )
{
	sprite_setTilePos(3, x, y);
	mysprites[3].isHeal = 1;
    mysprites[3].facingDown = 1;
	mysprites[3].nextTurn = HEAL_S;
	mysprites[3].hp = HEAL_L;
	mysprites[3].maxHp = HEAL_L;
	mysprites[3].alive = 1;
	sprite_setImage(3, findAnimOffset(3));	
	sprite_setImage(3, findAnimOffset(3));	
	//mysprites[3].lastImage = (HEAL_START * 8) | PRIORITY(mysprites[3].priority);
}
//init zombies
void sprite_init_zomb(int index, int x, int y )
{
	sprite_setTilePos(index, x, y);
	mysprites[index].isZomb = 1;
    mysprites[index].facingDown = 1;
	mysprites[index].nextTurn = ZOMB_S;
	mysprites[index].hp = ZOMB_L;
	mysprites[index].maxHp = ZOMB_L;
	mysprites[index].alive = 1;
	sprite_setImage(index, ZOMB_START);
	sprite_setImage(index, ZOMB_START);
	//mysprites[index].lastImage = (ZOMB_START * 8) | PRIORITY(mysprites[index].priority);
}
void sprite_init_spit( int index, int x, int y )
{
	sprite_setTilePos(index, x, y);
	mysprites[index].isSpit = 1;
    mysprites[index].facingDown = 1;
	mysprites[index].nextTurn = SPIT_S;
	mysprites[index].hp = SPIT_L;
	mysprites[index].maxHp = SPIT_L;
	mysprites[index].alive = 1;
	sprite_setImage(index, SPIT_START);
	sprite_setImage(index, SPIT_START);
	//mysprites[index].lastImage = (SPIT_START * 8) | PRIORITY(mysprites[index].priority);
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
	mysprites[i].lastImage = sprites[i].attribute2;
	sprites[i].attribute2 = (thisImageIndex * 8) | PRIORITY(mysprites[i].priority);
	UpdateSpriteMemory();
}

void sprite_resetImage ( int i )
{
	//swap
	u16 temp = mysprites[i].lastImage;
	sprites[i].attribute2 = mysprites[i].lastImage;
	mysprites[i].lastImage = temp;
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
	{//special case for turn counter
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
		if ( i >=100 && i < 110 )
			sprite_draw( i, mysprites[i].x, mysprites[i].y );
		else if ( mysprites[i].x - myBg.x >= 0 && mysprites[i].y - myBg.y >= 0 &&
			mysprites[i].x - myBg.x < 240 && mysprites[i].y - myBg.y < 240 )
			sprite_draw ( i, mysprites[i].x - myBg.x, mysprites[i].y - myBg.y );
		else
			sprite_draw ( i, 240, 160);
	}
	UpdateSpriteMemory();
}

void sprite_moveCursor ( int i )
//I:	a sprite index
//O:	currently sprite is moved in direction of key presses...
//TODO:	make function work via single cursor input...need to implement findpath function
//R:
{
	int x = mysprites[127].x/8;
	int y = mysprites[127].y/8;
	sprite_updateAll();
	
	if ( keyReleased(BUTTON_UP ) && y > 0 && myBg.select[( y - 2 ) * myBg.mtw + x ] == fontMap[64] )
	{
		sprite_moveUp(127);
	}
	else if ( keyReleased(BUTTON_DOWN ) && y < myBg.mth && myBg.select[( y + 2 ) * myBg.mtw + x ] == fontMap[64] )
	{
		sprite_moveDown(127);
	}
	else if ( keyReleased(BUTTON_LEFT ) && x > 0 && myBg.select[ y * myBg.mtw + x - 2 ] == fontMap[64])
	{
		sprite_moveLeft(127);
	}
	else if ( keyReleased(BUTTON_RIGHT ) && x < myBg.mtw && myBg.select[ y * myBg.mtw + x + 2 ] == fontMap[64] )
	{
		sprite_moveRight(127);
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
	bg_centerOver(i);
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
	bg_centerOver(i);
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
	bg_centerOver(i);
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
	bg_centerOver(i);
}

void sprite_findPath(int i, int start_x, int start_y, int end_x, int end_y )
//I:	a sprite index, the x,y coords for start pos and end pos
//O:	a valid min length path is generated from start to end
//		sprite with index i is move along this path from start to end
//R:	none
{
	bg_centerOver( i );
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
	bg_centerOver( index );
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
		
		//deinc attacked hp
		mysprites[attk].hp -= getAttackPower(index) + (rand() % getAttackPower(index));
		if ( mysprites[attk].hp <= 0 )
			sprite_die (attk);
			
		//inc attk counter
		mysprites[index].attkCounter++;

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
	bg_centerOver( curr );
	int hasAttacked;
	if ( mysprites[curr].isZomb )
		hasAttacked = sprite_zombie_attack( curr );
	else 
		hasAttacked = sprite_zombie_spit (curr );
	
	
	
	int x, y, k;
	int end_x = mysprites[curr].x, end_y = mysprites[curr].y;
	float minDist = 99999;
	int foo, bar;
	float crap;
	Stack px, py; //potential moves
	stack_init ( &px);
	stack_init ( &py);
	bg_drawMoveableArea(curr, getRange(curr));
	if ( !hasAttacked)
	{
		float thisDist = 999999;
		for ( y = 0; y < myBg.mth; y+=2)
			for ( x = 0; x < myBg.mtw; x+=2)
				if ( myBg.select[y*myBg.mtw+x] == fontMap[64] )//check all moveable squares
					for ( k = 0; k < 4; k++)
					{
						
			
						foo = y - mysprites[k].y/8;
						bar = x - mysprites[k].x/8;
					
						crap = (foo*foo + bar*bar);
					
						thisDist = sqrtf(crap);
						if ( thisDist == minDist )
						{
							stack_push ( &px, x );
							stack_push ( &py, y );
						}
						else if ( thisDist < minDist )
						{
							while ( !stack_empty(&px )) 
							{
								stack_pop( &px );
								stack_pop( &py );
							}
							minDist = thisDist;
							
							stack_push(&px, x );
							stack_push(&py, y );
							

						}
					}
					
		int thisIndex = rand() % ( px.top + 1 );
		end_x = px.arr[thisIndex] * 8;
		end_y = py.arr[thisIndex] * 8;
		sprite_findPath ( curr, mysprites[curr].x, mysprites[curr].y, end_x, end_y );
		//free stacks
		stack_free ( & px );
		stack_free ( & py );
		
		if ( mysprites[curr].isZomb )
			sprite_zombie_attack( curr );
		else 
			sprite_zombie_spit (curr );
	}
	bg_clearMoveable();
}

int sprite_zombie_attack(int curr)
{
	bg_centerOver( curr );
	int rval = 0;
	int xi = mysprites[curr].x/8;
	int yi = mysprites[curr].y/8;
	
	Stack px, py; //potential attacks
	stack_init ( &px);
	stack_init ( &py);
	
	bg_drawAttackableSquares(curr );
	
	if ( isValidMapPosition ( xi+2, yi) && myBg.select[(yi)*myBg.mtw+(xi+2)] == fontMap[64]
		&&	bg_tileOccupiedByPlayer ( (xi+2)*8, yi*8))
	{
		stack_push ( &px, xi + 2 );
	    stack_push ( &py, yi );
		rval = 1;
	}
	else if ( isValidMapPosition ( xi-2, yi) && myBg.select[(yi)*myBg.mtw+(xi-2)] == fontMap[64]
		&&	bg_tileOccupiedByPlayer ( (xi-2)*8, yi*8))
	{
		stack_push ( &px, xi - 2 );
	    stack_push ( &py, yi );
		rval = 1;
	}
	else if ( isValidMapPosition ( xi, yi+2) && myBg.select[(yi+2)*myBg.mtw+(xi)] == fontMap[64]
		&&	bg_tileOccupiedByPlayer ( (xi)*8, (yi+2)*8))
	{
		stack_push ( &px, xi );
	    stack_push ( &py, yi + 2 );
		rval = 1;
	}
	else if ( isValidMapPosition ( xi, yi-2) && myBg.select[(yi-2)*myBg.mtw+(xi)] == fontMap[64]
		&&	bg_tileOccupiedByPlayer ( (xi)*8, (yi-2)*8))
	{
		stack_push ( &px, xi );
	    stack_push ( &py, yi - 2 );
		rval = 1;
	}
	
	if ( !stack_empty ( &px ))
	{
		int thisIndex = rand() % ( px.top + 1 );
		sprite_Attack( curr, (px.arr[thisIndex])*8, (py.arr[thisIndex])*8);
	}
	//free stacks
	stack_free ( & px );
	stack_free ( & py );
	
	bg_clearMoveable();
	
	return rval;
}

int sprite_zombie_spit ( int curr )
{
	bg_centerOver ( curr );
	int rval = 0;

	Stack px, py; //potential attacks
	stack_init ( &px);
	stack_init ( &py);
	
	int i;
	int x, y;
	int thisIndex = 0;
	int nextIndex = 1;	
	int thisSprite = 0;
	
	
	volatile int n;
	
	//draw selectable range for current sprite
	bg_drawSelectableRange( curr );
	
	//check all map squares for players that the spitter can attack
	for ( y = 0; y < myBg.mth; y+=2 )
		for ( x = 0; x < myBg.mtw; x+=2 )
			//if a the square is selectable and its occpuied by a human
			if ( myBg.select[y*myBg.mtw+x] == fontMap[64] && bg_tileOccupiedByPlayer ( x*8, y*8 ) )
			{
				//push this location on stacks
				stack_push ( &px, x );
				stack_push ( &py, y );
				//found a dude to attack
				rval = 1;
			}
	//if there is a dude to attack
	if ( !stack_empty ( &px ))
	{
		//animation start
		int start_x = mysprites[curr].x;
		int start_y = mysprites[curr].y;
		
		//randomly select a dude from the stack of attackable dudes
		if ( stack_size ( &px )  > 1 )
			thisSprite = rand() % ( stack_size(&px) );
		
		//animation ends at this dude
		int end_x = px.arr[thisSprite] * 8;
		int end_y = py.arr[thisSprite] * 8;	
		
		//this dudes sprite index is
		thisSprite = findSpriteIndex ( end_x, end_y);
	
		//play moving spit animation
		while (( start_x != end_x) || (start_y != end_y) )
		{
			int dx = end_x-start_x;
			int dy = end_y-start_y;
			if ( dx > 0 )
				start_x++;
			else if ( dx < 0 )
				start_x--;
			if ( dy > 0 )
				start_y++;
			else if ( dy < 0 )
				start_y--;
		
			thisIndex+=nextIndex;
			sprite_setImage( 125, (SPITBALL_START + ( thisIndex % 3) ));
			sprite_setPos( 125, start_x, start_y );	
		
			if ( thisIndex == 1 && nextIndex == 1)
				sprites[125].attribute1 = sprites[125].attribute1;
			else if ( thisIndex == 1 && nextIndex == -1)
				sprites[125].attribute1 = sprites[125].attribute1 | HORIZONTAL_FLIP;
			
			sprite_updateAll();
		
			if ( thisIndex == 0 || thisIndex == 2 )
				nextIndex *= -1;
			for ( n = 0; n < 10000; ++n);
		}
		
		//change sprite to got hit
		sprite_setImage(thisSprite, findGotHitOffset(thisSprite));
		//decrement hp
		mysprites[thisSprite].hp -= SPIT_P + (rand() % SPIT_P);
		if ( mysprites[thisSprite].hp <= 0 )
			sprite_die (thisSprite);
		
		
		//play spit animation over sprite
		for ( i = 0; i < 16; i++ )
		{
			sprite_setImage(125, SPITBALL_START + ( i%3 ));
			sprite_updateAll();
			for ( n = 0; n < 10000; ++n);
		}
		//restore prev frame
		sprite_resetImage(thisSprite);
	}
	
	//free stacks
	stack_free ( &px );
	stack_free ( &py );
	
	//clear assoicated ui sprites
	for ( i = 114; i < 127; ++i )
		sprite_setPos ( i, -160, -160 );
	
	bg_clearMoveable();
	
	return rval;
	
}

void sprite_gren_special ( int curr, int end_x, int end_y )
{
	bg_centerOver ( curr ); 
	
	int start_x = mysprites[curr].x;
	int start_y = mysprites[curr].y;
	
	int thisIndex = 0;
	int nextIndex = 1;
	
	volatile int n;	
	
	//show grenade flying animation
	while (( start_x != end_x) || (start_y != end_y) )
	{
		int dx = end_x-start_x;
		int dy = end_y-start_y;
		if ( dx > 0 )
			start_x++;
		else if ( dx < 0 )
			start_x--;
		if ( dy > 0 )
			start_y++;
		else if ( dy < 0 )
			start_y--;
		
		thisIndex+=nextIndex;
		sprite_setImage( 125, (GRENADE_START + ( thisIndex % 3) ));
		sprite_setPos( 125, start_x, start_y );
		
		if ( thisIndex == 1 && nextIndex == 1)
				sprites[125].attribute1 = sprites[125].attribute1;
		else if ( thisIndex == 1 && nextIndex == -1)
				sprites[125].attribute1 = sprites[125].attribute1 | HORIZONTAL_FLIP;
		sprite_updateAll();
		
		if ( thisIndex == 0 || thisIndex == 2 )
			nextIndex *= -1;
		

		for ( n = 0; n < 10000; ++n);
	}
	
	bg_clearMoveable();
	
	//find all sprites that got hit!
	end_x /= 8;
	end_y /= 8;
	
	Stack gotHit; //potential attacks
	stack_init ( &gotHit);
	
	//set explosion sprite locations
	sprite_setImage(125, EXPLOSION_START );
	if ( bg_tileOccupied ( end_x * 8 , end_y * 8 ) )
		stack_push ( &gotHit, findSpriteIndex(end_x*8, end_y*8 ));
	if ( isValidMapPosition( end_x + 2, end_y ))
	{
		sprite_setImage(124, EXPLOSION_START );
		sprite_setPos( 124, (end_x+2)*8, end_y*8);
		if ( bg_tileOccupied ( (end_x+2)*8, end_y*8 ))
			stack_push ( &gotHit, findSpriteIndex((end_x+2)*8, end_y*8 ));		
	}
	if ( isValidMapPosition( end_x - 2, end_y ))
	{
		sprite_setImage(123, EXPLOSION_START );
		sprite_setPos( 123, (end_x-2)*8, end_y*8);
		if ( bg_tileOccupied ( (end_x-2)*8, end_y*8 ))
			stack_push ( &gotHit, findSpriteIndex((end_x-2)*8, end_y*8 ));		
	}
	if ( isValidMapPosition( end_x, end_y + 2 ))
	{
		sprite_setImage(122, EXPLOSION_START );
		sprite_setPos( 122, (end_x)*8, (end_y+2)*8);
		if ( bg_tileOccupied ( (end_x)*8, (end_y+2)*8 ))
			stack_push ( &gotHit, findSpriteIndex((end_x)*8, (end_y+2)*8 ));		
	}
	if ( isValidMapPosition( end_x, end_y - 2 ))
	{
		sprite_setImage(121, EXPLOSION_START );
		sprite_setPos( 121, (end_x)*8, (end_y-2)*8);
		if ( bg_tileOccupied ( (end_x)*8, (end_y-2)*8 ))
			stack_push ( &gotHit, findSpriteIndex((end_x)*8, (end_y-2)*8 ));		
	}
	
	//init stack of sprites to change back
	Stack changeBack;
	stack_init ( &changeBack );
	
	//change sprites to got hit anims, decrement life
	while (!stack_empty (&gotHit) )
	{
		int thisSprite = stack_pop ( &gotHit );
		sprite_setImage(thisSprite, findGotHitOffset ( thisSprite )); 
		stack_push ( &changeBack, thisSprite );
	}
	//play explosion animation
	int i, j;
	for ( i = 0; i < 16; ++i )
	{
		for ( j = 121; j <= 125; ++j)
			sprite_setImage(j, EXPLOSION_START + ( i%2 )); //change to GRENADE_START once it exists
		sprite_updateAll();
		for ( n = 0; n < 20000; ++n);
	}	
	
	//reset sprites to previous image.
	while (!stack_empty (&changeBack) )
	{
		int thisSprite = stack_pop ( &changeBack );
		sprite_resetImage(thisSprite ); 
		sprite_updateAll();
		mysprites[thisSprite].hp -= GRENADE_POWER;
		if ( mysprites[thisSprite].hp <= 0 )
			sprite_die (thisSprite);
	}
	
	//decrement attk counter
	mysprites[curr].attkCounter--;
	ui_updateStatus();
	
	//free stacks
	stack_free ( &gotHit );
	stack_free ( &changeBack );
	
	//clear assoicated ui sprites
	for ( i = 114; i < 127; ++i )
		sprite_setPos ( i, -160, -160 );
	
	bg_clearMoveable();
}

void sprite_snip_special ( int index, int x, int y )
//passing it cursor pos
{
	bg_centerOver( index );
	
	Stack gotHit, changeBack;
	stack_init ( &gotHit );
	stack_init ( &changeBack );
	
	u16 prevAttackerFrame = sprites[index].attribute2;
	
	int numhit = 0;
	
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
			x = mysprites[index].x / 8 - 2;
			y = mysprites[index].y / 8;
			while ( isValidMapPosition(x,y) && myBg.select[y*myBg.mtw+x] != 0x1001 && numhit < 5 )
			{
				if ( bg_tileOccupied ( x * 8, y * 8 ) )
				{
					int thisSprite = findSpriteIndex ( x * 8, y * 8 );
					stack_push ( &gotHit, thisSprite );
					numhit++;
				}
				x-=2;
			}
		}
		else if ( mysprites[index].x < x )
		{//attacking right
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 1;
			mysprites[index].facingUp = 0;
			mysprites[index].facingDown = 0;
			offset += 10;
			x = mysprites[index].x / 8 + 2;
			y = mysprites[index].y / 8;
			while ( isValidMapPosition(x,y) && myBg.select[y*myBg.mtw+x] != 0x1001 && numhit < 5)
			{
				if ( bg_tileOccupied ( x * 8, y * 8 ) )
				{
					int thisSprite = findSpriteIndex ( x * 8, y * 8 );
					stack_push ( &gotHit, thisSprite );
					numhit++;
				}
				x+=2;
			}
			
		}
		else if ( mysprites[index].y > y )
		{//attacking up
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 0;
			mysprites[index].facingUp = 1;
			mysprites[index].facingDown = 0;
			offset += 14;
			x = mysprites[index].x / 8;
			y = mysprites[index].y / 8 - 2;
			while ( isValidMapPosition(x,y) && myBg.select[y*myBg.mtw+x] != 0x1001 && numhit < 5)
			{
				if ( bg_tileOccupied ( x * 8, y * 8 ) )
				{
					int thisSprite = findSpriteIndex ( x * 8, y * 8 );
					stack_push ( &gotHit, thisSprite );
					numhit++;
				}
				y-=2;
			}
		}
		else if ( mysprites[index].y < y )
		{//attacking down
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 0;
			mysprites[index].facingUp = 0;
			mysprites[index].facingDown = 1;
			offset += 12;
			x = mysprites[index].x / 8;
			y = mysprites[index].y / 8 + 2;
			while ( isValidMapPosition(x,y) && myBg.select[y*myBg.mtw+x] != 0x1001 && numhit < 5)
			{
				if ( bg_tileOccupied ( x * 8, y * 8 ) )
				{
					int thisSprite = findSpriteIndex ( x * 8, y * 8 );
					stack_push ( &gotHit, thisSprite );
					numhit++;
				}
				y+=2;
			}
		}
		
		
		
		int j;
		for ( j = 0; j < 16; j++)
		{
			//character attacking
			if ( j < 12 )
				//sprites[index].attribute2 =  (offset + (j%2)*8);
				sprite_setImage(index, (offset + ( j % 2) ));
			//attacked character got hit
			if ( j == 3 )
			{
				int startIndex = 121;
				while ( !stack_empty(&gotHit ) )
				{
					int attk = stack_pop ( & gotHit );
					stack_push( &changeBack , attk );
					//sprites[attk].attribute2 = findGotHitOffset ( attk );
					sprite_setImage(attk, findGotHitOffset(attk));
					sprite_setPos(startIndex, mysprites[attk].x, mysprites[attk].y);
					//sprites[126].attribute2 = BLOOD_START * 8;
					sprite_setImage(startIndex, BLOOD_START);
					startIndex++;
				}
			}
			//spurt blood
			if ( j > 3 )
			{
				int k;
				for ( k = 121; k <= 125; ++k )
					sprite_setImage(k, BLOOD_START + ( j%3 ));
			}
			sprite_updateAll();
			volatile int n;
			for ( n = 0; n < 10000; n++);
		}
		
		//reset sprites to previous image.
		while (!stack_empty (&changeBack) )
		{
			int thisSprite = stack_pop ( &changeBack );
			sprite_resetImage(thisSprite ); 
			mysprites[thisSprite].hp -= SNIPER_POWER;
			if ( mysprites[thisSprite].hp <= 0 )
				sprite_die (thisSprite);
			sprite_updateAll();
		}
		sprites[index].attribute2 = prevAttackerFrame;
		
		//decrement attk counter
		mysprites[index].attkCounter--;
		ui_updateStatus();
		
		//free stacks
		stack_free ( &gotHit );
		stack_free ( &changeBack );
	
		//clear assoicated ui sprites
		int i;
		for ( i = 114; i < 127; ++i )
			sprite_setPos ( i, -160, -160 );
	
		bg_clearMoveable();
	
	}

}

void sprite_tank_special ( int index, int x, int y )
{
	bg_centerOver( index );

	Stack gotHit, changeBack;
	stack_init ( &gotHit );
	stack_init ( &changeBack );
	
	int anims[3];
	
	int xi = mysprites[index].x/8;
	int yi = mysprites[index].y/8;
	
	u16 prevAttackerFrame = sprites[index].attribute2;
	
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
			if (isValidMapPosition ( xi -2, yi ) && bg_tileOccupied ( (xi - 2)*8, yi*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi - 2) * 8, yi * 8 ) );
			if (isValidMapPosition ( xi -2, yi + 2 ) && bg_tileOccupied ( (xi - 2)*8, (yi + 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi - 2) * 8, ( yi + 2 ) * 8 ) );
			if (isValidMapPosition ( xi -2, yi - 2 ) && bg_tileOccupied ( (xi - 2)*8, (yi - 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi - 2) * 8, ( yi - 2 ) * 8 ) );
		}
		else if ( mysprites[index].x < x )
		{//attacking right
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 1;
			mysprites[index].facingUp = 0;
			mysprites[index].facingDown = 0;
			offset += 10;
			if (isValidMapPosition ( xi +2, yi ) && bg_tileOccupied ( (xi + 2)*8, yi*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi + 2) * 8, yi * 8 ) );
			if (isValidMapPosition ( xi +2, yi + 2 ) && bg_tileOccupied ( (xi + 2)*8, (yi + 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi + 2) * 8, ( yi + 2 ) * 8 ) );
			if (isValidMapPosition ( xi +2, yi - 2 ) && bg_tileOccupied ( (xi + 2)*8, (yi - 2 )*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi + 2) * 8, ( yi - 2 ) * 8 ) );
		}
		else if ( mysprites[index].y > y )
		{//attacking up
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 0;
			mysprites[index].facingUp = 1;
			mysprites[index].facingDown = 0;
			offset += 14;
			if (isValidMapPosition ( xi, yi - 2 ) && bg_tileOccupied ( xi*8, (yi - 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi) * 8, (yi-2) * 8 ) );
			if (isValidMapPosition ( xi -2, yi - 2 ) && bg_tileOccupied ( (xi - 2)*8, (yi - 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi - 2) * 8, ( yi - 2 ) * 8 ) );
			if (isValidMapPosition ( xi +2, yi - 2 ) && bg_tileOccupied ( (xi + 2)*8, (yi - 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi + 2) * 8, ( yi - 2 ) * 8 ) );
		}
		else if ( mysprites[index].y < y )
		{//attacking down
			mysprites[index].facingLeft = 0;
			mysprites[index].facingRight = 0;
			mysprites[index].facingUp = 0;
			mysprites[index].facingDown = 1;
			offset += 12;
			if (isValidMapPosition ( xi, yi + 2 ) && bg_tileOccupied ( xi*8, (yi + 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi) * 8, ( yi + 2 ) * 8 ) );
			if (isValidMapPosition ( xi - 2, yi + 2 ) && bg_tileOccupied ( (xi - 2)*8, (yi + 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi - 2) * 8, ( yi + 2 ) * 8 ) );
			if (isValidMapPosition ( xi + 2, yi + 2 ) && bg_tileOccupied ( (xi + 2)*8, (yi + 2)*8 ) )
				stack_push ( &gotHit, findSpriteIndex ( (xi + 2) * 8, ( yi + 2 ) * 8 ) );
		}
	
		anims[0] = offset;
		anims[1] = offset + 1;
		anims[2] = getTankSpecialOffset(index);
		//play anims
		int j;
		for ( j = 0; j < 16; j++)
		{
			//character attacking
			if ( j < 12 )
				//sprites[index].attribute2 =  (offset + (j%2)*8);
				sprite_setImage(index, anims[j%3]);
			//attacked character got hit
			if ( j == 3 )
			{
				int startIndex = 121;
				while ( !stack_empty(&gotHit ) )
				{
					int attk = stack_pop ( & gotHit );
					stack_push( &changeBack , attk );
					//sprites[attk].attribute2 = findGotHitOffset ( attk );
					sprite_setImage(attk, findGotHitOffset(attk));
					sprite_setPos(startIndex, mysprites[attk].x, mysprites[attk].y);
					//sprites[126].attribute2 = BLOOD_START * 8;
					sprite_setImage(startIndex, BLOOD_START);
					startIndex++;
				}
			}
			//spurt blood
			if ( j > 3 )
			{
				int k;
				for ( k = 121; k <= 125; ++k )
					sprite_setImage(k, BLOOD_START + ( j%3 ));
			}
			sprite_updateAll();
			volatile int n;
			for ( n = 0; n < 10000; n++);
		}
		//reset sprites to previous image.
		while (!stack_empty (&changeBack) )
		{
			int thisSprite = stack_pop ( &changeBack );
			sprite_resetImage(thisSprite ); 
			mysprites[thisSprite].hp -= SHOTGUN_POWER;
			if ( mysprites[thisSprite].hp <= 0 )
				sprite_die (thisSprite);
			sprite_updateAll();
		}
		sprites[index].attribute2 = prevAttackerFrame;
		//decrement attk counter
		mysprites[index].attkCounter--;
		ui_updateStatus();
	}
	//free stacks
	stack_free ( &gotHit );
	stack_free ( &changeBack );
	
	//clear assoicated ui sprites
	int i;
	for ( i = 114; i < 127; ++i )
		sprite_setPos ( i, -160, -160 );
	
	bg_clearMoveable();
	
}

int getTankSpecialOffset(index)
{
	int rval = 0;
	if ( mysprites[index].facingLeft || mysprites[index].facingRight )
		rval = 17;
	else if ( mysprites[index].facingDown )
		rval = 18;
	else if ( mysprites[index].facingUp )
		rval = 19;
	return rval;
}

void sprite_heal_special ( int index, int x, int y )
{
	bg_centerOver( index );
	
	volatile int n;
	
	int thisSprite = findSpriteIndex ( x, y ) ;
	
	u16 prevAttackerFrame = sprites[index].attribute2;
	if ( thisSprite > -1 )
	{
		int j;
		for ( j = 0; j < 16; j++)
		{
			//healer healing
			if ( j < 12 )
				//sprites[index].attribute2 =  (offset + (j%2)*8);
				sprite_setImage(index, HEAL_START + 17 + ( j % 2 ));
			//Start heal anim
			if ( j == 3 )
			{
				//sprites[attk].attribute2 = findGotHitOffset ( attk );
				sprite_setPos(126, mysprites[thisSprite].x, mysprites[thisSprite].y);
				//sprites[126].attribute2 = BLOOD_START * 8;
				sprite_setImage(126, HEALANIM_START);
			}
			//spurt blood
			if ( j > 3 )
			{
				sprite_setImage(126, HEALANIM_START + ( j%2 ));
			}
			sprite_updateAll();
			for ( n = 0; n < 10000; n++);
		}
	
		sprites[index].attribute2 = prevAttackerFrame;
		
		mysprites[thisSprite].hp += HEAL_POWER;
			
		if ( mysprites[thisSprite].hp > mysprites[thisSprite].maxHp )
			mysprites[thisSprite].hp = mysprites[thisSprite].maxHp;
		//decrement attk counter
		mysprites[index].attkCounter--;
		ui_updateStatus();
	}
	
	int i;
	for ( i = 114; i < 127; ++i )
		sprite_setPos ( i, -160, -160 );

	bg_clearMoveable();
	
}

extern void removeZombie();
void sprite_die ( int index )
{
	if ( index >= 4 )
		removeZombie();
	
	mysprites[index].hp = 0;
	mysprites[index].alive = 0;
	sprite_setPos( index, -160, -160);
	linked_deleteByIndex (&head, index);
	ui_updateStatus();
	tc_updateTc ();
}