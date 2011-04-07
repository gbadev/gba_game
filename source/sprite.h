//sprite.h

void sprites_init();


void sprites_init()
{
	int n, i;
	//copy in sprite palette
    for(n = 0; n < 256; n++)
        SpritePal[n] = robotPalette[n];

    //move sprites offscreen
    for(n = 0; n < 128; n++)
    {
		mysprites[n].x = -16;
		mysprites[n].y = -16;
        sprites[n].attribute0 = -16; //using copy of OAM
        sprites[n].attribute1 = -16;
    }
	
	//last spritedata index
    int max = 16 * 16 * 8 / 2;
    
	//read in sprite data
    for(n = 0; n < max; n++)
        SpriteData[n] = robotData[n];

    
    UpdateSpriteMemory();
    

    //attribute0: color mode, shape and y pos
	for ( i =0 ; i < 4; i++)
	{
		SetSpritePos( i, i*128, i*128);
		DrawMoveableArea ( i );
	}
    
	//attribute2: Image location
    sprites[0].attribute2 = 0;
    UpdateSpriteMemory();
}