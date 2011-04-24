void ui_updateStatus();

void ui_updateStatus()
{
	int i;
	for ( i = 110; i < 114; i++)
	{
		mysprites[i].x=0;
		mysprites[i].y=16 + (i-110) * 32;
		sprites[i].attribute2 = findAnimOffset(i-110);
	}
}