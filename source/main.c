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
	//init-load ( needs to be seperated still )
	bg_load(&x,&y); //initializes and loads gravedemomap
	sprite_init(); //initializes and loads sprite data
    int mode = 0;
    //game loop
    while ( 1 )
    {   
        //button polling
        CheckButtons();
		if (Pressed(BUTTON_A) && mode == 0)
		{
			DrawMoveableArea(0, &x, &y);
			mode = 1;
		}
		else if (Pressed(BUTTON_B) && mode == 1 )
		{
			ClearMoveableBg();
			mode = 0;
		}
		
		if ( mode == 1 )
		{
			bg_move_sprite(curr);
		}
		//scroll the background
		bg_scroll( &x, &y );
		
		//update all sprites
		sprite_update_all(&x, &y);
        WaitVBlank();
        for(n = 0; n < 1000; n++);
    }

    return 0;
}
/* END OF FILE */
