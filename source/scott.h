//#include "graphics/overworld.h"
#include "dave.h"
//speed
#define ZOMB_S 2
#define HEAL_S 5
#define SNIP_S 3
#define TANK_S 4
#define GREN_S 4

//range
#define ZOMB_M 8
#define HEAL_M 6
#define SNIP_M 8
#define TANK_M 5
#define GREN_M 6

//life
#define ZOMB_L 1
#define HEAL_L 50
#define SNIP_L 75
#define TANK_L 150
#define GREN_L 100

//power
#define ZOMB_P 10
#define HEAL_P 1
#define SNIP_P 1
#define TANK_P 1
#define GREN_P 1

//special
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
		rval = ZOMB_S;
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
		rval = ZOMB_M;
	return rval;
}



void draw_world(void);

void draw_world(void)
{
    SetMode(MODE_3 | BG2_ENABLE);
    int x, y, n = 0;
    WaitVBlank();
    for(y = 0; y < 160; y++)
    {
        for(x = 0; x < 240; x++)
        {
 //           DrawPixel3(x, y, overworldBitmap[n]);
            n++;
        }
    }
	
	for(x = 0; x < 150000; x++);
	
	CheckButtons();
	while(!Pressed(BUTTON_A))
	{
		
	}
}
