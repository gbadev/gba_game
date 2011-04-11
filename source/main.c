#include "dave.h"//must be loaded first!
//graphics
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
			bg_drawMoveableArea(0, &x, &y, 8);
			mode = 1;
		}
		else if (Pressed(BUTTON_B) && mode == 1 )
		{
			bg_clearMoveable();
			mode = 0;
		}
		
		if ( mode == 1 )
		{
			sprite_move(curr);
		}
		//scroll the background
		bg_scroll( &x, &y );
		
		//update all sprites
		sprite_updateAll(&x, &y);
        WaitVBlank();
        for(n = 0; n < 1000; n++);
    }

    return 0;
}
/* END OF FILE */
