#include "gba_title_screen.h"
#include "zombie_brain.h"
#include "dave.h"

//speed
#define ZOMB_S 6
#define SPIT_S 6
#define HEAL_S 5
#define SNIP_S 3
#define TANK_S 4
#define GREN_S 4

//range
#define ZOMB_M 8
#define SPIT_M 8
#define HEAL_M 6
#define SNIP_M 8
#define TANK_M 5
#define GREN_M 6

//special attack range
#define ZOMB_SR 4
#define SPIT_SR 4
#define HEAL_SR 4
#define SNIP_SR 4
#define TANK_SR 1
#define GREN_SR 4


//life
#define ZOMB_L 1
#define SPIT_L 1
#define HEAL_L 60
#define SNIP_L 75
#define TANK_L 90
#define GREN_L 75

//power
#define ZOMB_P 100
#define SPIT_P 100
#define HEAL_P 1
#define SNIP_P 1
#define TANK_P 1
#define GREN_P 1

//special
#define HEAL_POWER 20
#define GRENADE_POWER 10
#define SNIPER_POWER 10
#define SHOTGUN_POWER 10
#define SPECIAL 1
#define IDLE 0

//status
#define UNDEAD 0
#define DEAD -1
#define ALIVE 1

//flipped
#define FLIPPED -1
#define NOT_FLIPPED 1

int getSpeed ( int index )
{
	int rval = 100;
	if ( mysprites[index].isTank )
		rval = TANK_S;
	else if ( mysprites[index].isGren )
		rval = GREN_S;
	else if ( mysprites[index].isSnip )
		rval = SNIP_S;
	else if ( mysprites[index].isHeal )
		rval = HEAL_S;
	else if ( mysprites[index].isZomb )
		rval = ZOMB_S;
	else if ( mysprites[index].isSpit )
		rval = SPIT_S;
	return rval;
}

int getRange(int index)
{
	int rval = 1;
	if ( mysprites[index].isTank )
		rval = TANK_M;
	else if ( mysprites[index].isGren )
		rval = GREN_M;
	else if ( mysprites[index].isSnip )
		rval = SNIP_M;
	else if ( mysprites[index].isHeal )
		rval = HEAL_M;
	else if ( mysprites[index].isZomb )
		rval = ZOMB_M;
	else if ( mysprites[index].isSpit )
		rval = SPIT_M;
	return rval;
}

int getAttackPower(int index)
{
	int rval = 1;
	if ( mysprites[index].isTank )
		rval = TANK_P;
	else if ( mysprites[index].isGren )
		rval = GREN_P;
	else if ( mysprites[index].isSnip )
		rval = SNIP_P;
	else if ( mysprites[index].isHeal )
		rval = HEAL_P;
	else if ( mysprites[index].isZomb )
		rval = ZOMB_P;
	else if ( mysprites[index].isSpit )
		rval = SPIT_P;
	return rval;
}

int getSpecialRange(int index)
{
	int rval = 1;
	if ( mysprites[index].isTank )
		rval = TANK_SR;
	else if ( mysprites[index].isGren )
		rval = GREN_SR;
	else if ( mysprites[index].isSnip )
		rval = SNIP_SR;
	else if ( mysprites[index].isHeal )
		rval = HEAL_SR;
	else if ( mysprites[index].isZomb )
		rval = ZOMB_SR;
	else if ( mysprites[index].isSpit )
		rval = SPIT_SR;
	return rval;
}



void drawStart(void);
void drawEnd(void);

void drawStart(void)
{

    SetMode(MODE_3 | BG2_ENABLE);
    int x, y, n = 0;
    WaitVBlank();
    for(y = 0; y < 160; y++)
    {
        for(x = 0; x < 240; x++)
        {
           DrawPixel3(x, y, gba_title_screenBitmap[n]);
            n++;
        }
    }
	
	int seed = 0;
	while(!keyReleased(BUTTON_START))
	{
		seed++;
		keyPoll();
	}
	

	
	for(y = 0; y < 160; y++)
    {
        for(x = 0; x < 240; x++)
        {
           DrawPixel3(x, y, RGB(0,0,0));
            n++;
        }
    }
	srand( seed );
}


void drawEnd(void)
{
    SetMode(MODE_3 | BG2_ENABLE);
    int x, y, n = 0;
    WaitVBlank();
    for(y = 0; y < 160; y++)
    {
        for(x = 0; x < 240; x++)
        {
           DrawPixel3(x, y, zombie_brainBitmap[n]);
            n++;
        }
    }
	
	while(!keyReleased(BUTTON_A))
	{

		keyPoll();
	}
	

	
	for(y = 0; y < 160; y++)
    {
        for(x = 0; x < 240; x++)
        {
           DrawPixel3(x, y, RGB(0,0,0));
            n++;
        }
    }
	
}
