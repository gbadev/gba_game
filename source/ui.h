void ui_updateStatus();
void ui_draw();

extern void tty_printChar ( int , int, char );

void ui_updateStatus()
{
	int i;
	//updating sprite status frames on left side
	for ( i = 110; i < 114; i++)
	{
		mysprites[i].x=0;
		mysprites[i].y=16 + (i-110) * 32;
		//sprites[i].attribute2 = findAnimOffset(i-110);
		sprite_setImage(i, findAnimOffset ( i-110));
	}
	
	//printing hp
	//tank
	char tens = mysprites[110].hp/10 + '0';
	char ones = mysprites[110].hp%10+ '0';
	tty_printChar(0, 4, tens);
 	tty_printChar(1, 4, ones);
	
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
	ui_updateStatus();
}