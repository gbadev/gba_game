//bg.h
#define u16 unsigned short
typedef struct tagBgHandler
{
	u16 * pal;
	u16 * tiles ;
	u16 * map; 	//curr map that is being played
	u16 * shadow; 	//curr map's shadow/hit map for passable terrain
	u16 * select; 	//used for movement selection, will also be used for attack/special selection
	int width;	//width in pixels
	int height;	//height in pixels
	int mtw;	//map tile width
	int mth;	//map tile height
	int numtiles;
	int bgtiles;

}BgHandler;
BgHandler myBg;

//globals
const int screen_width = 240;
const int screen_height = 160;
int xi, yi;
int delta_x = 0, delta_y = 0;

//create a pointer to background 3 and 2 tilemap buffers
unsigned short* bg3map =(unsigned short*) ScreenBaseBlock (31);
unsigned short* bg2map =(unsigned short*) ScreenBaseBlock (30);

//bg functions
//init and load
void bg_init();
void bg_load(int *x, int *y, const u16 * currPal, const u16 * currMap, const u16 * currTiles ,const u16 * currShadow, 
	int width, int height);
//scrolling
void bg_scroll( int *, int *);
void bg_scrollLeft();
void bg_scrollRight();
void bg_scrollUp();
void bg_scrollDown();
//moveableArea
void bg_drawMoveableArea ( int, int * x, int * y, int moves);
void bg_drawMoveableSquares ( int, int, int );
void bg_drawMoveableSquare ( int, int);
void bg_clearMoveable();
void bg_updateMoveable(int x, int y);

//helpers
int isValidMapPosition ( int x, int y);

void bg_init()
//I:	none
//O:	backgrounds are initialized.
//		bg3 - 256 x 256 pixels, 32x32 8x8 tiles, located in ScreenBaseBlock 31
//		bg2 - 256 x 256 pixels, 32x32 8x8 tiles, located in ScreenBaseBlock 30
//		Mode is set to 0, currently 2 backgrounds enabled.
//R:	none
{
    //set up background 0
    //We are using a 256x256 Map which is placed in ScreenBaseBlock 31
    REG_BG3CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (31 << SCREEN_SHIFT);
	REG_BG2CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (30 << SCREEN_SHIFT) | ( 1 << CHAR_SHIFT);
    //set video mode 0 with background 0
    SetMode(0|BG3_ENABLE|BG2_ENABLE|OBJ_ENABLE|OBJ_MAP_1D);
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
	myBg.pal = currPal;
	myBg.map = currMap;
	myBg.tiles = currTiles;
	myBg.shadow = currShadow;
	myBg.width = width;
	myBg.height = height;
	myBg.mtw = width / 8;
	myBg.mth = height / 8;
	myBg.numtiles = myBg.mtw * myBg.mth;
	myBg.bgtiles = 32 *32;
	
	myBg.select = malloc ( sizeof ( u16 ) * myBg.numtiles );
	
    //copy the palette into the background palette memory
    DMAFastCopy((void*)myBg.pal, (void*)BGPaletteMem, 256, DMA_16NOW);
    //copy the tile images into the tile memory
    DMAFastCopy((void*)myBg.tiles, (void*)CharBaseBlock(0), 6976/4, DMA_32NOW);		//need to change to be dynamic
	DMAFastCopy((void*)showmovesTiles, (void*)CharBaseBlock(1),128/4, DMA_32NOW);
    //4992 = #Tiles * 64

    //copy the tile map into background 0
    int i, j, k = 0;
    for ( j = 0; j < 32; j++ )
        for ( i = 0; i < 32; i++ )
		{
            bg3map[k] = myBg.map[j * myBg.mtw + i];
			//bg2map[k] = bluetileMap[0];
			k++;
		}
    
	*x = 0;
	REG_BG3HOFS = 0;
	REG_BG2HOFS = 0 ;
    *y = 0;
	REG_BG3VOFS = 0;
	REG_BG2VOFS = 0 ;	
	
	bg_clearMoveable();
}

void bg_scroll( int * x, int * y )
//I:	scrolling registers
//O:	if direction buttons are pressed, scrolls in appropriate direction.
//R:	none
{
	//process y - axis movement first
    //move up
    if( Pressed ( BUTTON_UP ))
        if ( *y > 0)
        {
            (*y)--;
            delta_y--;
        }

    //move down
    if( Pressed ( BUTTON_DOWN))
		if ( (*y + screen_height) <  myBg.height )
        {
            (*y)++;
            delta_y++;
        }

 

    //process x movement
    //move left
    if( Pressed ( BUTTON_LEFT ))
        if ( *x > 0)
        {
            (*x)--;
            delta_x--;
        }
		
    //move right
    if( Pressed ( BUTTON_RIGHT ))
		if ( ( *x + screen_width ) < myBg.width)
        {
			(*x)++;
            delta_x++;
        }
        
    //caculate new xi, yi
	yi = *y / 8; //bg y coordinate
    xi = *x / 8; //bg x coordinate

	 //if you have moved 8 pixels down
    if (delta_y == 8  )
		bg_scrollDown();		
    else if (delta_y == -8 )
		bg_scrollUp();
	
    if (delta_x == 8 )
		bg_scrollRight();
    else if (delta_x == -8 )
    	bg_scrollLeft();
    
    //update scrolling registers
    REG_BG3VOFS = *y ;
	REG_BG2VOFS = *y ;
    REG_BG3HOFS = *x ;
	REG_BG2HOFS = *x ;
		
}

void bg_scrollLeft()
//I:	none
//O:	bg scrolls one 8 pixel wide ( 1 tile ) column to the left
//R:	none
{
	int bgstart, bgoffset, mstart, moffset, bgindex, mindex, i;
	//moving left
    //find start indexes
    bgstart = ( ((yi+31)%32 ) * 32 ) + ( ( xi + 31 ) %32 );
    mstart = (( yi-1) * myBg.mtw ) + ( xi - 1 );
    
	for ( i = 0; i < 32; i++ )
    {
		//find offsets
		bgoffset = i * 32;
        moffset = i * myBg.mtw;
                
        //calcuate indexes
        bgindex = ( bgstart + bgoffset ) % myBg.bgtiles;
        mindex = mstart + moffset;
                
        //if index is valid
        if ( isValidMapPosition (  xi-1, yi - 1 + i ) == 1)
        {
			//write
			WaitVBlank();
            bg3map[ bgindex ] = myBg.map[ mindex ];
			bg2map[ bgindex ] = myBg.select[ mindex ];
        }
        //reset delta x
        delta_x = 0;
    }
}

void bg_scrollRight()
//I:	none
//O:	bg scrolls one 8 pixel wide ( 1 tile ) column to the right
//R:	none
{
	int bgstart, bgoffset, mstart, moffset, bgindex, mindex, i;
	//moving right
    //find start indexes
    bgstart = ( ((yi + 31)%32) * 32 ) + ( (xi + 30) % 32 );
    mstart = ( (yi - 1) * myBg.mtw )+ ( xi + 30 );

    for ( i = 0; i < 32; i++ )
    {
		//find offset
        bgoffset = i * 32;
        moffset =  i * myBg.mtw;
		//calculate indexes
        bgindex = ( bgstart + bgoffset ) % myBg.bgtiles;
        mindex = mstart + moffset;
                
        //if index is valid
        if ( isValidMapPosition (  xi+30, yi - 1 + i ) == 1 )
        {
			//write
			WaitVBlank();
            bg3map[ bgindex ] = myBg.map[ mindex ];
			bg2map[ bgindex ] = myBg.select[ mindex ];
        }
        //reset delta x to 0
        delta_x = 0;
    }
}

void bg_scrollUp()
//I:	none
//O:	bg scrolls one 8 pixel high ( 1 tile ) row up
//R:	none
{
	int bgstart, bgoffset, mstart, moffset, bgindex, mindex,i;
	//moving up
    //find start indexes
    bgstart = ( ( (yi + 31 ) % 32) * 32 + ((xi +31)% 32) );
    mstart = ( yi - 1 ) * myBg.mtw + xi - 1;
 
    for ( i = 0; i < 32; i++ )
    {
		//find offsets
        bgoffset = i;
        moffset = i;
        //calculate indexes
        bgindex = bgstart + bgoffset;
        mindex = mstart + moffset;

        //if index is valid
        if ( isValidMapPosition (  (xi - 1) + moffset, yi - 1 ) )
        {
			//find end of current row
            int nextrow = bgstart - ( bgstart % 32 ) + 32;
            //stay in current row
            if ( bgindex >= nextrow)
            {
				bgindex -= 32;
            }
            //write!
			WaitVBlank();
            bg3map[ bgindex ] = myBg.map[ mindex ];
			bg2map[ bgindex ] = myBg.select[ mindex ];
        }
        //reset delta y
        delta_y = 0;
    }
}

void bg_scrollDown()
//I:	none
//O:	bg scrolls one 8 pixel high ( 1 tile ) row down
//R:	none
{
	int bgstart, bgoffset, mstart, moffset, bgindex, mindex,i;
	//moving down
    //bg start
    bgstart = ( ( ( yi + 30 )% 32 ) * 32 + ((xi +31 ) % 32) );
    //map start
    mstart = ( yi + 30 ) * myBg.mtw + xi - 1;
    for ( i = 0; i < 32; i++ )
    {
        //find offsets
        bgoffset = i;
        moffset = i;
        //calculate index
        bgindex = bgstart + bgoffset;
        mindex = mstart + moffset;

        //check if index is valid
        if ( isValidMapPosition ( xi - 1 + moffset, yi+30))
        {
            //find end of current row
            int nextrow = bgstart - ( bgstart % 32 ) + 32;
            //stay in current row
            if ( bgindex >= nextrow )
                bgindex -=32;
                    
            //write!
			WaitVBlank();
            bg3map[ bgindex ] = myBg.map [ mindex ];
			bg2map[ bgindex ] = myBg.select[ mindex ];
        }
        //reset delta y
        delta_y = 0;
    }
}

int isValidMapPosition ( int x, int y)
//I:    a position, given by x and y coords
//O:    none
//R:    true if index is valid map index, false
{
    int result = 1;
    if ( x < 0 || y < 0 || x >= myBg.mtw || y >= myBg.mth )
        result = 0;
    return result;
}

void bg_drawMoveableArea ( int i, int * x, int * y, int moves)
//I:	a character's SpriteHandler index, scrolling registers, number of tiles character can move
//O:	map for characters moveable area is created, bg showing moveable area is displayed over base map
//R:	none
{							 //x tile index    y tile index
	bg_drawMoveableSquares ( mysprites[i].x/8, mysprites[i].y/8, moves);
	bg_updateMoveable( *x, *y);
	
	
}
void bg_drawMoveableSquares ( int x, int y, int moves )
//I:	a tile's x and y indexes, the number of moves left
//O:	moveable map is generated ( really shitty algoritm currently )
//R:	none
{
	//if the current x,y tile indexes are valid and the tile at x,y is passable
	if ( x >= 0 && y >= 0 && x < myBg.mtw && y < myBg.mth && myBg.shadow[y * myBg.mtw + x ] != 0x1001)
	{
		//that square is passable, thus update movement map to include it
		bg_drawMoveableSquare ( x, y );
		//if there are more moves to make
		if ( moves )
		{
			//check to see if tile is passable before recursively calling in all directions
			if ( myBg.shadow[y * myBg.mtw + x - 2 ] != 0x1001 && x-2 >= 0 && y >= 0 && x-2 < myBg.mtw && y < myBg.mth )
				bg_drawMoveableSquares ( x - 2, y , moves - 1 );
			if ( myBg.shadow[y * myBg.mtw + x + 2 ] != 0x1001 && x +2>= 0 && y >= 0 && x +2< myBg.mtw && y < myBg.mth)
				bg_drawMoveableSquares ( x + 2, y , moves - 1 );
			if ( myBg.shadow[( y - 2 ) * myBg.mtw + x ] != 0x1001 && x >= 0 && y -2 >= 0 && x < myBg.mtw && y-2 < myBg.mth)
				bg_drawMoveableSquares ( x, y - 2 , moves - 1 );
			if ( myBg.shadow[( y + 2 ) * myBg.mtw + x ] != 0x1001 && x >= 0 && y +2>= 0 && x < myBg.mtw && y +2< myBg.mth)
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
	myBg.select[y*myBg.mtw + x] = showmovesMap[2];
	myBg.select[y*myBg.mtw + x + 1] = showmovesMap[3];
	myBg.select[(y+1)*myBg.mtw+ x] = showmovesMap[6];
	myBg.select[(y+1)*myBg.mtw+ x+1] = showmovesMap[7];
}

void bg_clearMoveable()
//I:	none
//O:	the move selection background and map are reset
//R:	none
{
	int i, max = 32 * 32;
	//clear moveable bg
	for ( i = 0; i < max; ++i )
		bg2map[i] = showmovesMap[0];
	//clear moveable map
	for ( i = 0; i < myBg.numtiles; ++i )
		myBg.select[i] =  showmovesMap[0];
}

void bg_updateMoveable(int x, int y)
//I:	the values of the scrolling registers
//O:	the map content for moveselection is properly copied into bg2 ( accounting for scrolling registers )
//R:	none
{
	//find tile indexes
	x /= 8;
	y /= 8;
	
	int i, j;
	for ( i = 0; i < 32 ; ++i )
		for ( j = 0; j < 32; j++ )
		{
			//modulus is magic!
			bg2map[((i+y)%32) * 32 + (j+x)%32 ] = myBg.select[( y+i ) * myBg.mtw + x + j];
		}
}

