void ui_updateStatus();
void ui_draw();

void ui_updateStatus()
{
	int i;
	for ( i = 110; i < 114; i++)
	{
		mysprites[i].x=0;
		mysprites[i].y=16 + (i-110) * 32;
		//sprites[i].attribute2 = findAnimOffset(i-110);
		sprite_setImage(i, findAnimOffset ( i-110));
	}
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