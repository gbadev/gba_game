#include "dave.h"
//graphics
#include "graphics/gravedemo.h"
#include "graphics/gravedemoshadow.h"
#include "graphics/bluetile.h"
#include "graphics/robot.h"
//source
#include "sprite.h"
#include "bg.h"

int main ( void )
{
    volatile int n;
	int y, x, i;
    x = y = 0;

	bg_load(&x,&y);
	sprite_init();
    
    //game loop
    while ( 1 )
    {   
        //button polling
        CheckButtons();

		bg_scroll( &x, &y );
		sprite_update_all(&x, &y);
		
        WaitVBlank();
        for(n = 0; n < 1000; n++);
    }

    return 0;
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
