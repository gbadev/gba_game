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


//sound 
#include "sean.h"
#include "sound.h"



int main ( void )
{
	//crappy loop counter
    volatile int n;
	
	init_sound();
	
	PlaySound( &bigGunSound );
	drawStart();

	
	//init
	bg_init();
	sprite_init(); //initializes and loads sprite data
	maps_init();
	gs_init();
	
    
	myGame.currLvlIndex = -1;
	myGame.mode = 0;
	myGame.hasMoved = 1;
	myGame.hasAttacked = 1;
	
	//load()

	tc_updateTc();
	ui_updateStatus();
	
	gs_goToNextLvl();

	//game loop
    while ( 1 )
    {   
        //button polling
        keyPoll();
		
		if ( myLvl[gs_getCurrLvlIndex()].numZombies <= 0 || keyReleased ( BUTTON_START ))
			gs_goToNextLvl();
			
		gs_updateGame();
		
		//update all sprites
		sprite_updateAll();
        WaitVBlank();
        for(n = 0; n < 1000; n++);
    }

    return 0;
}
/* END OF FILE */
