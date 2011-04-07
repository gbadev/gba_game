#include "dave.h"

#include "graphics/gravedemo.h"
#include "graphics/gravedemoshadow.h"
#include "graphics/bluetile.h"
#include "graphics/robot.h"

#include "sprite.h"
#include "bg.h"

void SetSpritePos( int index, int x, int y );
void DrawSprite( int index, int x, int y );

void DrawMoveableArea ( int);
void DrawMoveableSquares ( int, int, int );
void DrawMoveableSquare ( int, int);

int main ( void )
{
    //calculate map tile width, height
    mtw = map_width / 8; 
    mth = map_height /8;
    volatile int n;
	int y, x,i;
    x = y = 0;


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
    int max = 16 * 16 * 8 / 2;
    //read in sprite data
    for(n = 0; n < max; n++)
        SpriteData[n] = robotData[n];

    
    UpdateSpriteMemory();
    

    //attribute0: color mode, shape and y pos
	for ( i =0 ; i < 4; i++)
	{
		SetSpritePos( i, i*128, i*128);
		//DrawMoveableArea ( i );
	}
    
	//attribute2: Image location
    sprites[0].attribute2 = 0;
    UpdateSpriteMemory();
	
	
	bg_load();
	//sprites_init();
	
    //counters


    //set scrolling regitsters to upper right corner of bg
    x = REG_BG3HOFS = 0;
	REG_BG2HOFS = 0 ;
    y = REG_BG3VOFS = 0;
	REG_BG2VOFS = 0 ;
    
    //vars for game loop
    //int bgstart, bgoffset, mstart, moffset, bgindex, mindex;
    
	


	
	
    //game loop
    while ( 1 )
    {
        //WaitVBlank();
        
        //button polling
        CheckButtons();

        //process y - axis movement first
        //move up
        if( Pressed ( BUTTON_UP ))
        {
            if ( y > 0)
            {
                y--;
                delta_y--;
            }
        }
        //move down
        if( Pressed ( BUTTON_DOWN))
        {
            if ( (y + screen_height) <  map_height )
            {
                y++;
                delta_y++;
            }
        }

        //find map indexes for current position
        yi = y / 8; //bg y coordinate
        xi = x / 8; //bg x coordinate

        //if you have moved 8 pixels down
        if (delta_y == 8  )
        {   
			bg_scroll_down();		
        }

        else if (delta_y == -8 )
        {
			bg_scroll_up();
        }

        //process x movement
        //move left
        if( Pressed ( BUTTON_LEFT ))
        {
            if ( x > 0)
            {
                x--;
                delta_x--;
            }
        }
        //move right
        if( Pressed ( BUTTON_RIGHT ))
        {
            if ( ( x + screen_width ) < map_width)
            {
                x++;
                delta_x++;
            }
        }
        
        //caculate new xi
        xi = x / 8; //bg x coordinate


        if (delta_x == 8 )
        {
			bg_scroll_right();
        }

        else if (delta_x == -8 )
        {
			bg_scroll_left();
        }

        //update scrolling registers
        REG_BG3VOFS = y ;
		REG_BG2VOFS = y ;
        REG_BG3HOFS = x ;
		REG_BG2HOFS = x ;
		
		for ( i = 0; i < 128; i++ )
		{
			if ( mysprites[i].x - x >= 0 && mysprites[i].y - y >= 0 &&
				mysprites[i].x - x < 240 && mysprites[i].y - y < 240 )
				DrawSprite ( i, mysprites[i].x - x, mysprites[i].y - y );
			else
				DrawSprite ( i, 240, 160);
		}
		UpdateSpriteMemory();
        WaitVBlank();
        for(n = 0; n < 1000; n++);
    }

    return 0;
}

int isValidMapPosition ( int x, int y)
//I:    a position, given by x and y coords
//O:    none
//R:    true if index is valid map index, false
{
    int result = 1;
    if ( x < 0 || y < 0 || x >= mtw || y >= mth )
        result = 0;
    return result;
}

void SetSpritePos( int index, int x, int y )
{
	mysprites[index].x = x;
	mysprites[index].y = y;
}

void DrawSprite( int index, int x, int y )
{
	sprites[index].attribute1 = SIZE_16 | x;
	sprites[index].attribute0 = COLOR_256 | SQUARE | y;
}

void DrawMoveableArea ( int i)
{
	int x = mysprites[i].x, y = mysprites[i].y;
	x/=8;
	y/=8;
	DrawMoveableSquares ( x, y, 3 );
	
}
void DrawMoveableSquares ( int x, int y, int moves )
{

	if ( gravedemoshadowMap[y * mtw + x ] != 0x1001 && x >= 0 && y >= 0 && x < 32 && y < 32)
	{
		DrawMoveableSquare ( x, y );
		if ( moves )
		{
			DrawMoveableSquares ( x - 2, y , moves - 1 );
			DrawMoveableSquares ( x + 2, y , moves - 1 );
			DrawMoveableSquares ( x, y - 2 , moves - 1 );
			DrawMoveableSquares ( x, y  + 2, moves - 1 );
		}
	}
}
void DrawMoveableSquare ( int x, int y )
{
	bg2map[y*32 + x] = bluetileMap[0];
	bg2map[y*32 + x + 1] = bluetileMap[0];
	bg2map[(y+1)*32+ x] = bluetileMap[0];
	bg2map[(y+1)*32+ x+1] = bluetileMap[0];
}

/* END OF FILE */
