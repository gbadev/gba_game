//gamestate.h

void gs_init();

typedef struct tagGSHandler
{
	int superMode;
	int currLvlIndex;
	int mode;
	int hasMoved;
	int hasAttacked;
	int canSpecial;
	int curr;
	

}GSHandler;
GSHandler myGame;

void gs_init()
{
	myGame.superMode = 0;
	myGame.mode = 0;
	myGame.curr = 0;
	myGame.currLvlIndex = 0;
	myGame.hasMoved = 1;
	myGame.hasAttacked = 1;
	myGame.canSpecial = 0;
}

void gs_goToNextLvl()
{
	sprite_init();
	myGame.currLvlIndex++;
	if ( myGame.currLvlIndex >= 4 )
	{
		drawEnd ();
		drawStart ();
		bg_init();
		myGame.currLvlIndex = 0;
		sprite_init();
	}
	
	bg_load ( myLvl[myGame.currLvlIndex] );
	map_initCharacters();
	map_initZombies ();
	tc_init();
	
}

int gs_getCurrLvlIndex ( )
{
	return myGame.currLvlIndex;
}

int gs_getHasMoved()
{
	return myGame.hasMoved;
}

int gs_getHasAttacked()
{
	return myGame.hasAttacked;
}

int gs_getCanSpecial()
{
	return myGame.canSpecial;
}

void gs_updateGame()
{
	if ( myGame.hasMoved && myGame.hasAttacked	)
	{
		tc_updateTc();
		myGame.curr = tc_getNext();
		myGame.hasMoved = 0;	
		myGame.hasAttacked = 0;
		myGame.canSpecial = mysprites[myGame.curr].attkCounter;
		myGame.mode = 0;
		ui_updateStatus();
	}
		
	
		
		
	if ( myGame.curr < 4 )
	{	
	
		if ( myGame.mode == 0 )
		{
			//A is movement
			if ( keyReleased(BUTTON_A) && !myGame.hasMoved )
			{
				//draw moveable area
				bg_drawMoveableArea(myGame.curr, getRange(myGame.curr));
				sprite_setPos( 127, mysprites[myGame.curr].x, mysprites[myGame.curr].y);
				myGame.mode = 1;//execute move
			}
			//B is attack
			else if ( keyReleased(BUTTON_B) &&  !myGame.hasAttacked )
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
			
			}
			//L is special
			else if (keyReleased(BUTTON_L) && !myGame.hasAttacked && mysprites[myGame.curr].attkCounter >= 1 )
			{
				sprite_setPos( 127, mysprites[myGame.curr].x, mysprites[myGame.curr].y);
				if ( mysprites[myGame.curr].isTank )
					bg_drawSelectableRange(myGame.curr);
				else if ( mysprites[myGame.curr].isGren )
					bg_drawSelectableRange(myGame.curr);
				else if ( mysprites[myGame.curr].isSnip )
					bg_drawSniperRange(myGame.curr);
				else if ( mysprites[myGame.curr].isHeal )
					bg_drawSelectableRange(myGame.curr);
				myGame.mode = 3;
			}
			//R is end turn
			else if (keyReleased(BUTTON_R) )
			{
				myGame.hasMoved = 1;
				myGame.hasAttacked  = 1;
				myGame.mode = 0;
			}
			else if ( keyHeld ( BUTTON_UP ) || keyHeld ( BUTTON_DOWN ) || 
				keyHeld ( BUTTON_LEFT ) || keyHeld ( BUTTON_RIGHT ) )
			{
				bg_scroll();
			}
		}
		else if ( myGame.mode == 1 	)//execute move
		{
			if ( keyReleased ( BUTTON_A ) )
			{
				if ( (mysprites[myGame.curr].x != mysprites[127].x) || (mysprites[myGame.curr].y != mysprites[127].y) )
				{
					myGame.hasMoved = 1;
					sprite_findPath( myGame.curr, mysprites[myGame.curr].x, mysprites[myGame.curr].y, mysprites[127].x, mysprites[127].y );
				}
				sprite_setPos ( 127, -160, -160 );
				bg_clearMoveable ();
				myGame.mode = 0;
				ui_updateStatus();
			}
			
			if ( keyReleased ( BUTTON_B ) ) //cancel
			{
				sprite_setPos ( 127, -160, -160 );
				bg_clearMoveable ();
				myGame.mode = 0;
			}
		}
		else if ( myGame.mode == 2 	)//execute attack
		{
			if ( keyReleased ( BUTTON_A ) )
			{
				if ( (mysprites[myGame.curr].x != mysprites[127].x) || (mysprites[myGame.curr].y != mysprites[127].y ))
				{
					//attack
					sprite_Attack(myGame.curr, (mysprites[127].x), (mysprites[127].y));
					myGame.hasAttacked = 1;
				}
				
				//move cursor
				sprite_setPos ( 127, -160, -160 );
				
				bg_clearMoveable();
				myGame.mode = 0;
				ui_updateStatus();
			}
			
			if ( keyReleased ( BUTTON_B ) ) //cancel
			{
				sprite_setPos ( 127, -160, -160 );
				bg_clearMoveable ();
				myGame.mode = 0;
			}
		}
		else if ( myGame.mode == 3 	)//execute special
		{
			if ( keyReleased ( BUTTON_A ) )
			{
				if ( (mysprites[myGame.curr].x != mysprites[127].x) || (mysprites[myGame.curr].y != mysprites[127].y ))
				{
					if ( mysprites[myGame.curr].isTank )
						sprite_tank_special ( myGame.curr, mysprites[127].x, mysprites[127].y );
					else if ( mysprites[myGame.curr].isGren )
						sprite_gren_special ( myGame.curr, mysprites[127].x, mysprites[127].y );
					else if ( mysprites[myGame.curr].isSnip )
						sprite_snip_special ( myGame.curr, mysprites[127].x, mysprites[127].y );
					else if ( mysprites[myGame.curr].isHeal )
						sprite_heal_special ( myGame.curr, mysprites[127].x, mysprites[127].y );
					myGame.hasAttacked = 1;
				}
				//move cursor
				sprite_setPos ( 127, -160, -160 );
				myGame.hasAttacked = 1;
				bg_clearMoveable();
				myGame.mode = 0;
				ui_updateStatus();
			}
			if ( keyReleased ( BUTTON_B ) ) //cancel
			{
				sprite_setPos ( 127, -160, -160 );
				bg_clearMoveable ();
				myGame.mode = 0;
			}
		}
	}
	else
	{
		sprite_zombie_move( myGame.curr );
		myGame.hasMoved = 1;
		myGame.hasAttacked = 1;
		ui_updateStatus();
	}
	if (myGame.mode != 0)
	{
		sprite_moveCursor(myGame.curr);
	}
	//check if player has lost
	if (!( mysprites[0].alive || mysprites[1].alive || mysprites[2].alive || mysprites[3].alive ))
	{
		drawEnd ();
		drawStart ();

		bg_init();
		myGame.currLvlIndex = -1;
		gs_goToNextLvl();
	}
	
}
		
		
		
		
/*			
			
			
			/////////////////////////////////////////////////////////
				
			{
			if ( myGame.mode == 0 && !myGame.hasMoved)
			{	
				//draw moveable area
				bg_drawMoveableArea(myGame.curr, getRange(myGame.curr));
				sprite_setPos( 127, mysprites[myGame.curr].x, mysprites[myGame.curr].y);
				myGame.mode = 1;
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
}*/