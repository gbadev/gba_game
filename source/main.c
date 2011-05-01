//defines
#include "dave.h"//must be loaded first!

//resources
#include "stack.h"
#include "linked_list.h"

//graphics
#include "scott.h"
#include "showmoves.h"
#include "font.h"
#include "map.h"


//source - needs to have graphics loaded before source
#include "bg.h"
#include "sprite.h"
#include "tc.h"
#include "ui.h"
#include "tty.h"
#include "gamestate.h"

int main ( void )
{
	//crappy loop counter
    volatile int n;
	
	//x and y scrolling registers, index
	int y, x;
    x = -32;
	y = -16;
	//int curr = 0;
	
	//init
	bg_init();
	sprite_init(); //initializes and loads sprite data
	maps_init();
    
	//load()
	bg_load(myLvl[0]);
	//bg_load(&x,&y); //initializes and loads gravedemomap
	//sprite_load();
	
	tc_init();
	
	//int mode = 0;
    //game loop
	tc_updateTc();
	ui_updateStatus();
	
	myGame.mode = 0;
	myGame.hasMoved = 1;
	myGame.hasAttacked = 1;
	
    while ( 1 )
    {   
        //button polling
        keyPoll();
		
		if ( myGame.hasMoved && myGame.hasAttacked	)
		{
			tc_updateTc();
			myGame.curr = tc_getNext();
			myGame.hasMoved = 0;	
			myGame.hasAttacked = 0;
			myGame.mode = 0;
		}
		
		ui_updateStatus();
		
		
		if ( myGame.curr < 4 )
		{	
			//A is movement
			if (keyReleased(BUTTON_A))
			{
				if ( myGame.mode == 0 && !myGame.hasMoved)
				{	
					//draw moveable area
					bg_drawMoveableArea(myGame.curr, getRange(myGame.curr));
					sprite_setPos( 127, mysprites[myGame.curr].x, mysprites[myGame.curr].y);
					myGame.mode = 1;
				}
				else if ( myGame.mode == 1 )
				{	
					int start_x, start_y, end_x, end_y;
					start_x = mysprites[myGame.curr].x;
					start_y = mysprites[myGame.curr].y;
					end_x = mysprites[127].x;
					end_y = mysprites[127].y ;
			
					sprite_findPath ( myGame.curr, start_x, start_y, end_x, end_y );
					bg_clearMoveable();
					sprite_setPos ( 127, -160, -160 );	
					myGame.hasMoved = 1;
					myGame.mode = 0;
				}

			}
			//B is attack
			if (keyReleased(BUTTON_B))
			{
				if ( myGame.mode == 0 && !myGame.hasAttacked )
				{	
					//draw normal attack area
					if (bg_drawAttackableSquares(myGame.curr))
					{
						sprite_setPos( 127, mysprites[myGame.curr].x, mysprites[myGame.curr].y);
						myGame.mode = 2;
					}
				}
				else if ( myGame.mode == 2 )
				{	
					sprite_Attack(myGame.curr, (mysprites[127].x), (mysprites[127].y));
					bg_clearMoveable();
					sprite_setPos ( 127, -160, -160 );
					myGame.hasAttacked = 1;
					myGame.mode = 0;
				}					
			}
			if (keyReleased(BUTTON_L))	
			{
				if ( myGame.mode == 0 )
				{	
					//draw normal attack area
					sprite_setPos( 127, mysprites[myGame.curr].x, mysprites[myGame.curr].y);
					bg_drawSelectableRange ( myGame.curr);
					//bg_drawSniperRange ( myGame.curr );
					//bg_draw
					myGame.mode = 3;
				}
				else if ( myGame.mode == 3 )
				{
					bg_clearMoveable();
					//sprite_gren_special ( myGame.curr, mysprites[127].x, mysprites[127].y );
					//sprite_snip_special ( myGame.curr, mysprites[127].x, mysprites[127].y );
					//sprite_tank_special ( myGame.curr, mysprites[127].x, mysprites[127].y );
					sprite_heal_special ( myGame.curr, mysprites[127].x, mysprites[127].y );
					sprite_setPos ( 127, -160, -160 );
					myGame.mode = 0;
					myGame.hasMoved = 1;
					myGame.hasAttacked = 1;
				}
					
			}
		}
		else
		{
			sprite_zombie_move( myGame.curr );
			myGame.hasMoved = 1;
			myGame.hasAttacked = 1;
		}
		if (myGame.mode )
		{
			sprite_moveCursor(myGame.curr);
		}
			
		/*
		if (Pressed(BUTTON_A) && mode == 0)
		{	//show moveable are for curr char
			tc_updateTzzxc();
			
			
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
		else if (Pressed(BUTTON_B))
		{
			mode = 0;
		}
		
		if (Pressed(BUTTON_L) && mode == 0)	
		{
			bg_drawSelectableRange ( curr );
			mode = 3;
			
		}
		
		if (Pressed(BUTTON_L) && mode == 0)	
		{
			//bg_drawSelectableRange ( curr );
			bg_clearMoveable();
			mode = 2;
			
		}
		
		if ( mode == 1 || mode == 2 )
		{//move cursor
			sprite_moveCursor(curr);

		}*/
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
