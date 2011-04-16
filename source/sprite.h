//sprite.h

void sprite_init();

void sprite_setPos( int, int , int  );
void sprite_draw( int , int , int  );
void sprite_updateAll();
void spriteMoveUp(int);
void spriteMoveDown(int);
void spriteMoveLeft(int);
void spriteMoveRight(int);

//needs to be moved to sprite.h
void sprite_move ( int i );

void sprite_init()
//I:	none
//O:	sprite stuff is initialized
//R:	none
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
    int max = 16 * 16 * 48 / 2;
    
	//read in sprite data
    for(n = 0; n < max; n++)
        SpriteData[n] = robotData[n];

    
    UpdateSpriteMemory();
    
	//TODO : remove this crap
    sprite_setTilePos ( 0, 0, 0 );
	sprite_setTilePos ( 1, 9, 9 );

	
    sprites[0].attribute2 = 0;
	sprites[1].attribute2 = 8;
    UpdateSpriteMemory();
}

void sprite_setPos( int index, int x, int y )
//I:	a sprite index, a position given in x and y coordinates
//O:	the ith sprites x and y positions are set to x and y
//R:	none
{
	mysprites[index].x = x;
	mysprites[index].y = y;
}

void sprite_setTilePos( int index, int x, int y )
//I:	a sprite index, a position given in x and y TILE coordinates
//O:	the ith sprites x and y positions are set to x and y TILE coords
//R:	none
{
	mysprites[index].x = x * 16;
	mysprites[index].y = y * 16;
}

void sprite_draw( int index, int x, int y )
//I:	a sprite index, and an x and y position
//O:	OAM mem is updated so that sprite i is dipslayed with upper left corner at x,y
//R:	none
{
	sprites[index].attribute1 = SIZE_16 | x;
	sprites[index].attribute0 = COLOR_256 | SQUARE | y;
}


void sprite_updateAll()
//I:	none
//O:	all 128 sprites are updated on the screen.
//		if sprites are not visible on the screen they are moved to offscreen
//R:	none
{
	int i;
	for ( i = 0; i < 128; i++ )
	{
		if ( mysprites[i].x - myBg.x >= 0 && mysprites[i].y - myBg.y >= 0 &&
			mysprites[i].x - myBg.x < 240 && mysprites[i].y - myBg.y < 240 )
			sprite_draw ( i, mysprites[i].x - myBg.x, mysprites[i].y - myBg.y );
		else
			sprite_draw ( i, 240, 160);
	}
	UpdateSpriteMemory();
}

void sprite_move ( int i )
//I:	a sprite index
//O:	currently sprite is moved in direction of key presses...
//TODO:	make function work via single cursor input...need to implement findpath function
//R:
{
	int x = mysprites[i].x/8;
	int y = mysprites[i].y/8;
	
	if ( Pressed(BUTTON_UP ) && y > 0 && myBg.select[( y - 2 ) * myBg.mtw + x ] == showmovesMap[2] )
	{
		spriteMoveUp(i);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_UP ));
	}
	else if ( Pressed(BUTTON_DOWN ) && y < myBg.mth && myBg.select[( y + 2 ) * myBg.mtw + x ] == showmovesMap[2] )
	{
		spriteMoveDown(i);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_DOWN ));
	}
	else if ( Pressed(BUTTON_LEFT ) && x > 0 && myBg.select[ y * myBg.mtw + x - 2 ] == showmovesMap[2] )
	{
		spriteMoveLeft(i);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_LEFT ));
	}
	else if ( Pressed(BUTTON_RIGHT ) && x < myBg.mtw && myBg.select[ y * myBg.mtw + x + 2 ] == showmovesMap[2] )
	{
		spriteMoveRight(i);
		do
		{
			CheckButtons();
		}while ( Pressed(BUTTON_RIGHT ));
	}
}

//whoami(sprite) + direction + slide number
void spriteMoveDown(int i)
{
	volatile int n;
	int j;
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].y++;
		sprites[i].attribute2 = (i * 128) + (0*8 + (j%4)*8);
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}

void spriteMoveUp(int i)
{
	volatile int n;
	int j;
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].y--;
		sprites[i].attribute2 = (i * 128) + (0*8 + (j%4)*8);
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}

void spriteMoveRight(int i)
{
	volatile int n;
	int j;
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].x++;
		sprites[i].attribute2 = (i * 128) + (0*8 + (j%4)*8);
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}
void spriteMoveLeft(int i)
{
	volatile int n;
	int j;
	for ( j = 0; j < 16; j++)
	{
		mysprites[i].x--;
		sprites[i].attribute2 = (i * 128) + (0*8 + (j%4)*8);
		sprite_updateAll();
		for ( n = 0; n < 10000; n++);
	}
}
