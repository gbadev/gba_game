//sprite.h

void sprite_init();

void sprite_setPos( int, int , int  );
void sprite_draw( int , int , int  );
void sprite_updateAll(int *, int *);

//needs to be moved to sprite.h
void sprite_move ( int i );

void sprite_init()
//I:
//O:
//R:
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
    
	//TODO : remove this crap
    sprite_setPos ( 0, 0, 0 );
	sprite_setPos ( 1, 10, 10 );

	
    sprites[0].attribute2 = 0;
	sprites[1].attribute2 = 8;
    UpdateSpriteMemory();
}

void sprite_setPos( int index, int x, int y )
//I:
//O:
//R:
{
	mysprites[index].x = x * 8;
	mysprites[index].y = y * 8;
}

void sprite_draw( int index, int x, int y )
//I:
//O:
//R:
{
	sprites[index].attribute1 = SIZE_16 | x;
	sprites[index].attribute0 = COLOR_256 | SQUARE | y;
}


void sprite_updateAll(int *x, int *y)
//I:
//O:
//R:
{
	int i;
	for ( i = 0; i < 128; i++ )
	{
		if ( mysprites[i].x - *x >= 0 && mysprites[i].y - *y >= 0 &&
			mysprites[i].x - *x < 240 && mysprites[i].y - *y < 240 )
			sprite_draw ( i, mysprites[i].x - *x, mysprites[i].y - *y );
		else
			sprite_draw ( i, 240, 160);
	}
	UpdateSpriteMemory();
}

void sprite_move ( int i )
//I:
//O:
//R:
{
	int x = mysprites[i].x/8;
	int y = mysprites[i].y/8;
	
	if ( Pressed(BUTTON_UP ) && y > 0 && myBg.select[( y - 2 ) * myBg.mtw + x ] == showmovesMap[2] )
	{
		mysprites[i].y -= 16;
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_UP ));
	}
	else if ( Pressed(BUTTON_DOWN ) && y < myBg.mth && myBg.select[( y + 2 ) * myBg.mtw + x ] == showmovesMap[2] )
	{
		mysprites[i].y += 16;
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_DOWN ));
	}
	else if ( Pressed(BUTTON_LEFT ) && x > 0 && myBg.select[ y * myBg.mtw + x - 2 ] == showmovesMap[2] )
	{
		mysprites[i].x -= 16;
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_LEFT ));
	}
	else if ( Pressed(BUTTON_RIGHT ) && x < myBg.mtw && myBg.select[ y * myBg.mtw + x + 2 ] == showmovesMap[2] )
	{
		mysprites[i].x += 16;
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_RIGHT ));
	}
}
