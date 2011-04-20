

#include "dave.h"//must be loaded first!
//graphics
#include "scott.h"
#include "graphics/gravedemo.h"
#include "graphics/gravedemoshadow.h"
#include "graphics/showmoves.h"
#include "graphics/robot.h"
//source - needs to have graphics loaded before source
#include "bg.h"
#include "sprite.h"



int main ( void )
{
	//crappy loop counter
    volatile int n;
	
	//x and y scrolling registers, index
	int y, x, i;
    x = y = 0;
	int curr = 0;
	
	//init
	bg_init();
	sprite_init(); //initializes and loads sprite data
    
	//load()
	bg_load(&x, &y, gravedemoPal, gravedemoMap, gravedemoTiles , gravedemoshadowMap, 512, 512);
	//bg_load(&x,&y); //initializes and loads gravedemomap
	//sprite_load();
	
	int mode = 0;
    //game loop
    while ( 1 )
    {   
        //button polling
        CheckButtons();
		
		if (Pressed(BUTTON_L))	
		{
			bg_load(&x, &y, gravedemoPal, gravedemoMap, gravedemoTiles , gravedemoshadowMap, 512, 512);
		}
			
		if (Pressed(BUTTON_A) && mode == 0)
		{
			sprite_setPos ( 127, mysprites[curr].x, mysprites[curr].y );
			bg_drawMoveableArea(curr, 10);
			mode = 1;
		}
		else if (Pressed(BUTTON_B) && mode == 1 )
		{
			int start_x, start_y, end_x, end_y;
			start_x = mysprites[curr].x;
			start_y = mysprites[curr].y;
			end_x = mysprites[127].x;
			end_y = mysprites[127].y ;
			
			sprite_findPath ( curr, start_x, start_y, end_x, end_y );
			bg_clearMoveable();
			mode = 0;
			curr++;
			if ( curr > 2 )
				curr = 0;
		}
		
		if ( mode == 1 )
		{
			sprite_move(curr);

		}
		//scroll the background
		bg_scroll();
		
		//update all sprites
		sprite_updateAll();
        WaitVBlank();
        for(n = 0; n < 1000; n++);
    }

    return 0;
}
/* END OF FILE */
