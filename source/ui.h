void ui_updateStatus();
void ui_draw();


//accessors and helpers
extern int gs_getHasMoved();
extern int gs_getHasAttacked();
extern int gs_getCanSpecial();
extern void tty_printString (  int x, int y, char * str ,int len);
extern void tty_printChar ( int , int, char );

void ui_updateStatus()
{
	//ui_draw();
	int i;
	//updating sprite status frames on left side
	for ( i = 110; i < 114; i++)
	{
		mysprites[i].x=0;
		mysprites[i].y=16 + (i-110) * 32;
		//sprites[i].attribute2 = findAnimOffset(i-110);
		sprite_setImage(i, findAnimOffset ( i-110));
	}
	
	if ( !gs_getHasMoved() )
		tty_printString ( 0, 0, "MOVE", 4);
	else
		tty_printString ( 0, 0, "    ", 4);
	if ( !gs_getHasAttacked() )
		tty_printString ( 0, 1, "ATTK", 4);
	else
		tty_printString ( 0, 1, "    ", 4);
	if ( gs_getCanSpecial() )
		tty_printString ( 0, 1, "SPECIAL!!!", 10);
	else
		tty_printString ( 4, 1, "      ", 6);
	
	//printing hp
	//tank
	char tens = mysprites[0].hp/10 + '0';
	char ones = mysprites[0].hp%10+ '0';
	tty_printChar(0, 4, tens);
 	tty_printChar(1, 4, ones);
	tty_printChar(0, 5, 'S');
	tty_printChar(1, 5, mysprites[0].attkCounter + '0');
	
	tens = mysprites[1].hp/10 + '0';
	ones = mysprites[1].hp%10 + '0';
	tty_printChar(0, 8, tens);
 	tty_printChar(1, 8, ones);
	tty_printChar(0, 9, 'S');
	tty_printChar(1, 9, mysprites[1].attkCounter+ '0');
	tens = mysprites[2].hp/10 + '0';
	ones = mysprites[2].hp%10+ '0';
	tty_printChar(0, 12, tens);
 	tty_printChar(1, 12, ones);
	tty_printChar(0, 13, 'S');
	tty_printChar(1, 13, mysprites[2].attkCounter+ '0');
	tens = mysprites[3].hp/10 + '0';
	ones = mysprites[3].hp%10+ '0';
	tty_printChar(0, 16, tens);
 	tty_printChar(1, 16, ones);
	tty_printChar(0, 17, 'S');
	tty_printChar(1, 17, mysprites[3].attkCounter+ '0');
	
	/*tty_printChar(0, 8, (char)(mysprites[111].hp/10));
 	tty_printChar(1, 8, (char)(mysprites[111].hp%10));
	
	tty_printChar(0, 12, (char)(mysprites[112].hp/10));
 	tty_printChar(1, 12, (char)(mysprites[112].hp%10));
	
	tty_printChar(0, 16, (char)(mysprites[113].hp/10));
 	tty_printChar(1, 16, (char)(mysprites[113].hp%10));*/

}

void ui_draw()
{
	int i, j;
	for (i = 0; i<2; i++)
		for ( j = 0; j < 32; ++j)
			bg1map[i * 32 + j ] = fontMap[1];
	for (i = 2; i<32; i++) 
		for ( j = 0; j < 2; ++j)
			bg1map[i * 32 + j ] = fontMap[1];

	
	tty_printString ( 6, 0, "CURR", 4);

	ui_updateStatus();
}