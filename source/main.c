//defines
#include "dave.h"//must be loaded first!

//resources
#include "stack.h"
#include "linked_list.h"

//graphics
#include "scott.h"
#include "graphics/gravedemo.h"
#include "graphics/gravedemoshadow.h"
#include "graphics/showmoves.h"
#include "graphics/robot.h"
#include "graphics/font.h"


//source - needs to have graphics loaded before source
#include "bg.h"
#include "sprite.h"
#include "tc.h"
#include "ui.h"
#include "tty.h"


int main ( void )
{
	//crappy loop counter
    volatile int n;
	
	//x and y scrolling registers, index
	int y, x;
    x = -32;
	y = -16;
	int curr = 0;
	
	//init
	bg_init();
	sprite_init(); //initializes and loads sprite data
    
	//load()
	bg_load(&x, &y, gravedemoPal, gravedemoMap, gravedemoTiles , gravedemoshadowMap, 512, 512);
	//bg_load(&x,&y); //initializes and loads gravedemomap
	//sprite_load();
	
	tc_init();
	
	int mode = 0;
    //game loop
	tc_updateTc();
	ui_updateStatus();
    while ( 1 )
    {   
        //button polling
        CheckButtons();
		
		if (Pressed(BUTTON_L))	
		{
			bg_load(&x, &y, gravedemoPal, gravedemoMap, gravedemoTiles , gravedemoshadowMap, 512, 512);
		}
			
		if (Pressed(BUTTON_A) && mode == 0)
		{	//show moveable are for curr char
			tc_updateTc();
			ui_updateStatus();
			
			curr = tc_getNext();
			if ( curr < 4 )
			{
				sprite_setPos ( 127, mysprites[curr].x, mysprites[curr].y );
				bg_drawMoveableArea(curr, getRange(curr));
				mode = 1;
			}
			else
			{
				mode=1;
				sprite_zombie_move ( curr );
				mode=0;
			}
				
		}
		else if (Pressed(BUTTON_B) && mode == 1)
		{//move char to cursor
			int start_x, start_y, end_x, end_y;
			start_x = mysprites[curr].x;
			start_y = mysprites[curr].y;
			end_x = mysprites[127].x;
			end_y = mysprites[127].y ;
			
			sprite_findPath ( curr, start_x, start_y, end_x, end_y );
			bg_clearMoveable();
			sprite_setPos ( 127, -160, -160 );
			
			//draw attackable squares
			int result = bg_drawAttackableSquares(curr);
			if ( result )
			{
				sprite_setPos ( 127, mysprites[curr].x, mysprites[curr].y );
				mode = 2;
			}
			else
				mode = 0;
			tc_updateTc();
			ui_updateStatus();

		}
		else if (Pressed(BUTTON_A) && mode == 2 )
		{
			sprite_Attack(curr, (mysprites[127].x), (mysprites[127].y));
			mode = 0;
			tc_updateTc();
			ui_updateStatus();
		}
		/*else if (Pressed(BUTTON_B))
		{
			mode = 0;
		}*/
		if ( mode == 1 || mode == 2 )
		{//move cursor
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
