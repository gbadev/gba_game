//bg.h
#define u16 unsigned short

extern void sprite_updateAll();
extern void ui_draw();

typedef struct tagBgHandler
{
	int x;	//srolling x registers
	int y;	//scrolling y registers
	u16 * pal;
	u16 * tiles ;
	u16 * map; 	//curr map that is being played
	u16 * shadow; 	//curr map's shadow/hit map for passable terrain
	u16 * select; 	//used for movement selection, will also be used for attack/special selection
	short * movesleft;
	int width;	//width in pixels
	int height;	//height in pixels
	int mtw;	//map tile width
	int mth;	//map tile height
	int numtiles;
	int bgtiles;
	
	int numStartZombies;

}BgHandler;
BgHandler myBg;

//globals
const int screen_width = 240;
const int screen_height = 160;

int delta_x = 0, delta_y = 0;

//create a pointer to background 3 and 2 tilemap buffers
//unsigned short* bg3map =(unsigned short*) ScreenBaseBlock (31);
//unsigned short* bg2map =(unsigned short*) ScreenBaseBlock (30);
unsigned short* bg3map =(unsigned short*) ScreenBaseBlock (28);
unsigned short* bg2map =(unsigned short*) ScreenBaseBlock (24);
unsigned short* bg1map =(unsigned short*) ScreenBaseBlock (23);

//bg functions
//init and load
void bg_init();
void bg_load(int *x, int *y, const u16 * currPal, const u16 * currMap, const u16 * currTiles ,const u16 * currShadow, 
	int width, int height);
void bg_loadTile(int , int, u16 *, u16 *);

//scrolling
void bg_scroll();
void bg_centerOver( int curr );

//moveableArea
void bg_drawMoveableArea ( int, int moves);
void bg_drawMoveableSquares ( int, int, int );
void bg_drawMoveableSquare ( int, int);

//selectable area
void bg_drawSelectableRange(int index );
void bg_drawRangeSquares ( int x, int y, int moves );


void bg_clearMoveable();
void bg_updateMoveable();

//tile occupied
int bg_tileOccupied ( int x, int y );
int bg_tileOccupiedByPlayer ( int x, int y );

//helpers
int isValidMapPosition ( int x, int y);

void bg_init()
//I:	none
//O:	backgrounds are initialized.
//		bg3 - 512 x 512 pixels, 64x64 8x8 tiles, located in ScreenBaseBlocks 28-31
//		bg2 - 512 x 512 pixels, 64x64 8x8 tiles, located in ScreenBaseBlocks 24-27
//		Mode is set to 0, currently 2 backgrounds enabled.
//R:	none
{
    //set up background 0
    //We are using a 256x256 Map which is placed in ScreenBaseBlock 31
    //REG_BG3CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (31 << SCREEN_SHIFT);
	//REG_BG2CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (30 << SCREEN_SHIFT) | ( 1 << CHAR_SHIFT);
	 //set video mode 0 with background 0
	SetMode(0|BG3_ENABLE|BG2_ENABLE|BG1_ENABLE|OBJ_ENABLE|OBJ_MAP_1D);
	
	REG_BG3CNT = BG_COLOR256 | TEXTBG_SIZE_512x512 | (28 << SCREEN_SHIFT)|3;
	REG_BG2CNT = BG_COLOR256 | TEXTBG_SIZE_512x512 | (24 << SCREEN_SHIFT) | ( 1 << CHAR_SHIFT )|2;
	REG_BG1CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (23 << SCREEN_SHIFT) | ( 1 << CHAR_SHIFT )|0 ;
   
    
	
	myBg.x = 0;
	myBg.y = 0;
	myBg.pal = NULL;
	myBg.map = NULL;
	myBg.tiles = NULL;
	myBg.shadow = NULL;
	myBg.width = 0;
	myBg.height = 0;
	myBg.mtw = 0;
	myBg.mth = 0;
	myBg.numtiles = 0;
	myBg.bgtiles = 0;
	myBg.numStartZombies = 0;
	
	myBg.select = NULL;
	myBg.movesleft = NULL;
	

}

//should probably create a level struct to pass
void bg_load(int *x, int *y, const u16 * currPal, const u16 * currMap, const u16 * currTiles , const u16 * currShadow, int width, int height)
//I:	scrolling registers, a map pallete, a map, a map tileset, a shadowmap, the map's width and height
//O:	the specified map is loaded into bgHandler, scrolling registers are set to upper left corner of map,
//		initial 32x32 area loaded into bg3.
//R:
{ 
	if ( myBg.select )
		free ( myBg.select );
	if ( myBg.movesleft )
		free ( myBg.movesleft );
	myBg.pal = currPal;
	myBg.map = currMap;
	myBg.tiles = currTiles;
	myBg.shadow = currShadow;
	myBg.width = width;
	myBg.height = height;
	myBg.mtw = width / 8;
	myBg.mth = height / 8;
	myBg.numtiles = myBg.mtw * myBg.mth;
	myBg.bgtiles = 64 *64;
	
	myBg.select = malloc ( sizeof ( u16 ) * myBg.numtiles );
	myBg.movesleft = malloc ( sizeof ( short ) * (myBg.numtiles/4));
	
    //copy the palette into the background palette memory
    DMAFastCopy((void*)myBg.pal, (void*)BGPaletteMem, 256, DMA_16NOW);
    //copy the tile images into the tile memory
    DMAFastCopy((void*)myBg.tiles, (void*)CharBaseBlock(0), 6976/4, DMA_32NOW);		//need to change to be dynamic
	DMAFastCopy((void*)fontTiles, (void*)CharBaseBlock(1),4416/4, DMA_32NOW);
    //4992 = #Tiles * 64

    //copy the tile map into background 0
    int i, j;
    /*for ( j = 0; j < 32; j++ )
        for ( i = 0; i < 32; i++ )
		{
            bg3map[k] = myBg.map[j * myBg.mtw + i];
			//bg2map[k] = bluetileMap[0];
			k++;
		}
    */
	for ( j = 0; j < myBg.mth; j++ )
        for ( i = 0; i < myBg.mtw; i++ )
		{
			bg_loadTile(j,i, bg3map, myBg.map);
		}
	
	//TODO make this passable!
	myBg.numStartZombies = 10;
	
	myBg.x = -16;
	REG_BG3HOFS = -16;
	REG_BG2HOFS = -16 ;
    myBg.y = -16;
	REG_BG3VOFS = -16;
	REG_BG2VOFS = -16 ;	
	
	ui_draw();
	
	bg_clearMoveable();
}

void bg_loadTile(int x,int y, u16 * thisBg ,u16 * thisMap)
//I:	x and y tile coordinates, the bg to load into, the map to load from
//O:	the tile in thisMap is loaded into the correct place is thisBg (1 of 4 different 32*32 bgs)
//R:	none
{
	if ( y < 32 )
		if ( x < 32 )
			{
				thisBg[y*32+x] = thisMap[y * myBg.mtw + x];
			}
		else//x>=32
			{
				thisBg[32*32+y*32+x-32] = thisMap[y*myBg.mtw+x];
			}
	else//y>=32
		if ( x < 32 )
			{
				thisBg[32*32*2+(y-32)*32+x] = thisMap[y * myBg.mtw +x];
			}
		else//x>=32
			{
				thisBg[32*32*3+(y-32)*32+(x-32)] = thisMap[y*myBg.mtw+x];
			}
}
	

void bg_scroll()
//I:	scrolling registers
//O:	if direction buttons are pressed, scrolls in appropriate direction.
//R:	none
{
	//process y - axis movement first
    //move up
    if( keyHeld ( BUTTON_UP ))
        if ( myBg.y > -16)
        {
            (myBg.y)--;
            delta_y--;
        }

    //move down
    if( keyHeld ( BUTTON_DOWN))
		if ( (myBg.y + screen_height) <  myBg.height )
        {
            (myBg.y)++;
            delta_y++;
        }

 

    //process x movement
    //move left
    if( keyHeld ( BUTTON_LEFT ))
        if ( myBg.x > -16)
        {
            (myBg.x)--;
            delta_x--;
        }
		
    //move right
    if( keyHeld ( BUTTON_RIGHT ))
		if ( ( myBg.x + screen_width ) < myBg.width)
        {
			(myBg.x)++;
            delta_x++;
        }
    
    //update scrolling registers
    REG_BG3VOFS = myBg.y ;
	REG_BG2VOFS = myBg.y ;
    REG_BG3HOFS = myBg.x ;
	REG_BG2HOFS = myBg.x ;
	
	ui_draw();
}
	
void bg_centerOver( int curr )
{
	int start_x = myBg.x;
	int start_y = myBg.y;
	
	int end_x = mysprites[curr].x - 120;
	int end_y = mysprites[curr].y - 80;
	
	if ( end_x < -16)
		end_x = -16;
	if ( end_y < -16)
		end_y = -16;
	
	
	
	while (( start_x != end_x) || (start_y != end_y) )
	{
		int dx = end_x-start_x;
		int dy = end_y-start_y;
		if ( dx > 0 )
		{
			myBg.x++;
			start_x++;
		}
		else if ( dx < 0 )
		{
			myBg.x--;
			start_x--;
		}
		if ( dy > 0 )
		{
			myBg.y++;
			start_y++;
		}
		else if ( dy < 0 )
		{
			myBg.y--;
			start_y--;
		}
	
		REG_BG3VOFS = myBg.y ;
		REG_BG2VOFS = myBg.y ;
		REG_BG3HOFS = myBg.x ;
		REG_BG2HOFS = myBg.x ;
		sprite_updateAll();
		volatile int n;
		for ( n = 0; n < 3000; n++);
	}

		
}

int isValidMapPosition ( int x, int y)
//I:    a position, given  by x and y tile coords
//O:    none
//R:    true if index is valid map index, false
{
    int result = 1;
    if ( x < 0 || y < 0 || x >= myBg.mtw || y >= myBg.mth )
        result = 0;
    return result;
}

void bg_drawMoveableArea ( int i, int moves)
//I:	a character's SpriteHandler index, scrolling registers, number of tiles character can move
//O:	map for characters moveable area is created, bg showing moveable area is displayed over base map
//R:	none
{							 //x tile index    y tile index
	bg_centerOver(i);
	int j, max = myBg.numtiles/4;
	for ( j = 0; j  < max; ++j )
	{
		myBg.movesleft[j] = -1;
	}
	
	bg_drawMoveableSquares ( mysprites[i].x/8, mysprites[i].y/8, moves);
	//bg_updateMoveable();
	
	
}
void bg_drawMoveableSquares ( int x, int y, int moves )
//I:	a tile's x and y indexes, the number of moves left
//O:	moveable map is generated ( really shitty algoritm currently )
//R:	none
{ 
	//if the current x,y tile indexes are valid and the tile at x,y is passable
	if (isValidMapPosition ( x, y ) && myBg.shadow[y * myBg.mtw + x ] != 0x1001 && moves > myBg.movesleft[y/2 *(myBg.mtw/2)+x/2])
	{
		//that square is passable, thus update movement map to include it
		myBg.movesleft[y/2*(myBg.mtw/2) + x/2] = moves;
		bg_drawMoveableSquare ( x, y );
		
		//if there are more moves to make
		if ( moves )
		{
			//check to see if tile is passable before recursively calling in all directions
			if ( isValidMapPosition ( x-2, y ) &&myBg.shadow[y * myBg.mtw + x - 2 ] != 0x1001
				&& !bg_tileOccupied ( (x-2)*8, y*8 ))
				bg_drawMoveableSquares ( x - 2, y , moves - 1 );
			if ( isValidMapPosition ( x+2, y ) &&myBg.shadow[y * myBg.mtw + x + 2 ] != 0x1001
				&& !bg_tileOccupied ( (x+2)*8, y*8 ))
				bg_drawMoveableSquares ( x + 2, y , moves - 1 );
			if ( isValidMapPosition ( x, y-2 ) &&myBg.shadow[( y - 2 ) * myBg.mtw + x ] != 0x1001
				&& !bg_tileOccupied ( x*8, (y-2)*8 ))
				bg_drawMoveableSquares ( x, y - 2 , moves - 1 );
			if ( isValidMapPosition ( x, y+2 ) &&myBg.shadow[( y + 2 ) * myBg.mtw + x ] != 0x1001
				&& !bg_tileOccupied ( x*8, (y+2)*8 ))
				bg_drawMoveableSquares ( x, y  + 2, moves - 1 );
		}
	}
}
	
void bg_drawMoveableSquare ( int x, int y )
//I:	the x and y tile indexes for the upper left 8x8 quarter of the 16x16 tile that is passable
//O:	the 16x16 tile with upper left corner at tile index x,y is changed to a blue square
//R:	none
{
	//hardcoded....
	//copy into the select array that is actually arranged logically.
	myBg.select[y*myBg.mtw + x] = fontMap[64];
	myBg.select[y*myBg.mtw + x + 1] = fontMap[65];
	myBg.select[(y+1)*myBg.mtw+ x] = fontMap[66];
	myBg.select[(y+1)*myBg.mtw+ x+1] = fontMap[67];
	//load this square ( 4 tiles ) into bg2
	bg_loadTile(x,y,bg2map, myBg.select);
	bg_loadTile(x+1,y,bg2map, myBg.select);
	bg_loadTile(x,y+1,bg2map, myBg.select);
	bg_loadTile(x+1,y+1,bg2map, myBg.select);
}


void bg_clearMoveable()
//I:	none
//O:	the move selection background and map are reset
//R:	none
{
	int i, max = 64 * 64;
	//clear moveable bg
	for ( i = 0; i < max; ++i )
		bg2map[i] = fontMap[0];
		
	//clear moveable map
	for ( i = 0; i < myBg.numtiles; ++i )
		myBg.select[i] =  fontMap[0];
		//myBg.select[i] = 0;
		
	//clear movesleft map
	max = myBg.numtiles/4;
	for ( i = 0 ; i < max; ++i )
		myBg.movesleft[i] = -1;
}

void bg_updateMoveable()
//I:	the values of the scrolling registers
//O:	the map content for moveselection is properly copied into bg2 ( accounting for scrolling registers )
//R:	none
{
	
	//find tile indexes
	
	int i, j;
	for ( i = 0; i < 64 ; ++i )
		for ( j = 0; j < 64; j++ )
		{
			//modulus is magic!
			bg_loadTile(i, j, bg2map, myBg.select);
		}
}

int bg_tileOccupied ( int x, int y )
//I:	the x and y coordinates of the upper left corner of a tile
//O:	none		
//R:	true if the tile is currently occupied by a player or zombie, false otherwise
{
	int i;
	int result = 0;
	for ( i = 0; i < 100 && !result; i++ )
	{//only check possible character sprites
		if ( x == mysprites[i].x && y == mysprites[i].y )
			result = 1;
	}
	return result;
}

int bg_tileOccupiedByPlayer ( int x, int y )
//I:	the x and y coordinates of the upper left corner of a tile
//O:	none		
//R:	true if the tile is currently occupied by a player or zombie, false otherwise
{
	int i;
	int result = 0; 
	for ( i = 0; i < 4 && !result; i++ )
	{//only check possible character sprites
		if ( x == mysprites[i].x && y == mysprites[i].y )
			result = 1;
	}
	return result;
}

int bg_drawAttackableSquares(int index)
{

	int rval = 0;
	bg_clearMoveable();
	Stack x,y;
	stack_init ( &x );
	stack_init ( &y );
	int xi = mysprites[index].x/8;
	int yi = mysprites[index].y/8;
	if ( isValidMapPosition ( xi+2, yi) && bg_tileOccupied(mysprites[index].x+16, mysprites[index].y ))
	{
		stack_push(&x, (mysprites[index].x+16));
		stack_push(&y, (mysprites[index].y));
		rval = 1;
	}
	if ( isValidMapPosition ( xi, yi+2) && bg_tileOccupied(mysprites[index].x, mysprites[index].y+16 ))
	{
		stack_push(&x, (mysprites[index].x));
		stack_push(&y, (mysprites[index].y+16));
		rval = 1;
	}
	if ( isValidMapPosition ( xi-2, yi) && bg_tileOccupied(mysprites[index].x-16, mysprites[index].y ))
	{
		stack_push(&x, (mysprites[index].x-16));
		stack_push(&y, (mysprites[index].y));
		rval = 1;
	}
	if ( isValidMapPosition ( xi, yi-2) && bg_tileOccupied(mysprites[index].x, mysprites[index].y-16 ))
	{
		stack_push(&x, (mysprites[index].x));
		stack_push(&y, (mysprites[index].y-16));
		rval = 1;
		
	}
	
	if (!stack_empty(&x))
	{
		bg_drawMoveableSquare (xi, yi);
		mysprites[127].x = -160;
		mysprites[127].y = -160;
	}
		
	while (!stack_empty(&x))
	{
		bg_drawMoveableSquare (stack_pop(&x)/8, stack_pop(&y)/8);
	}	
	stack_free ( &x );
	stack_free ( &y );
	
	
	return rval;
}

void bg_drawSelectableRange(int index )
{
	bg_centerOver(index);
	bg_drawRangeSquares ( mysprites[index].x/8, mysprites[index].y/8, getSpecialRange(index));

}

void bg_drawRangeSquares ( int x, int y, int moves )
//I:	a tile's x and y indexes, the number of moves left
//O:	moveable map is generated ( really shitty algoritm currently )
//R:	none
{ 
	//if the current x,y tile indexes are valid and the tile at x,y is passable
	if (isValidMapPosition ( x, y ) && moves > myBg.movesleft[y/2 *(myBg.mtw/2)+x/2])
	{
		//that square is passable, thus update movement map to include it
		myBg.movesleft[y/2*(myBg.mtw/2) + x/2] = moves;
		bg_drawMoveableSquare ( x, y );
		
		//if there are more moves to make
		if ( moves )
		{
			//check to see if tile is passable before recursively calling in all directions
			if ( isValidMapPosition ( x-2, y ))
				bg_drawRangeSquares ( x - 2, y , moves - 1 );
			if ( isValidMapPosition ( x+2, y ))
				bg_drawRangeSquares ( x + 2, y , moves - 1 );
			if ( isValidMapPosition ( x, y-2 ) )
				bg_drawRangeSquares ( x, y - 2 , moves - 1 );
			if ( isValidMapPosition ( x, y+2 ))
				bg_drawRangeSquares ( x, y  + 2, moves - 1 );
		}
	}
}

void bg_drawSniperRange ( int curr )
{	
	bg_centerOver(curr );
	int xi = mysprites[curr].x / 8;
	int yi = mysprites[curr].y / 8;
	
	bg_drawMoveableSquare ( xi, yi );
	
	int thisx, thisy;
	
	thisx = xi + 2;
	thisy = yi;
	
	while ( isValidMapPosition ( thisx, thisy ) && myBg.shadow[thisy*myBg.mtw+thisx] != 0x1001 )
	{
		bg_drawMoveableSquare ( thisx, thisy );
		thisx+=2;
	}
	
	thisx = xi - 2;
	thisy = yi;
	
	while ( isValidMapPosition ( thisx, thisy ) && myBg.shadow[thisy*myBg.mtw+thisx] != 0x1001 )
	{
		bg_drawMoveableSquare ( thisx, thisy );
		thisx-=2;
	}
	
	thisx = xi;
	thisy = yi + 2;
	
	while ( isValidMapPosition ( thisx, thisy ) && myBg.shadow[thisy*myBg.mtw+thisx] != 0x1001)
	{
		bg_drawMoveableSquare ( thisx, thisy );
		thisy+=2;
	}
	
	thisx = xi;
	thisy = yi - 2;
	
	while ( isValidMapPosition ( thisx, thisy ) && myBg.shadow[thisy*myBg.mtw+thisx] != 0x1001 )
	{
		bg_drawMoveableSquare ( thisx, thisy );
		thisy-=2;
	}
}

