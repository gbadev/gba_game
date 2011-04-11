//bg.h
#define u16 unsigned short
typedef struct tagBgHandler
{
	u16 * Pal;
	u16 * Tiles;
	u16 * Map; 	//curr map that is being played
	u16 * Shadow; 	//curr map's shadow/hit map for passable terrain
	u16 * Select; 	//used for movement selection, will also be used for attack/special selection
	int width;	//width in pixels
	int height;	//height in pixels
	int mtw;	//map tile width
	int mth;	//map tile height
	int numtiles;
	int bgtiles;

}BgHandler;
BgHandler myBg;

//TODO rewrite everything that uses this shit below, has been replaced by BgHanlder
//globals
const int screen_width = 240;
const int screen_height = 160;

int xi, yi;

//boolean function
int isValidMapPosition ( int, int );


//create a pointer to background 0 tilemap buffer
unsigned short* bg3map =(unsigned short*) ScreenBaseBlock (31);
unsigned short* bg2map =(unsigned short*) ScreenBaseBlock (30);

unsigned short moveableMap[4096];

int delta_x = 0, delta_y = 0;

void bg_init();
void bg_load(int *x, int *y, const u16 * currPal, const u16 * currMap, const u16 * currTiles ,const u16 * currShadow, 
	int width, int height);
void bg_scroll( int *, int *);
void bg_scrollLeft();
void bg_scrollRight();
void bg_scrollUp();
void bg_scrollDown();


//bg functions, need to renamed
void bg_drawMoveableArea ( int, int * x, int * y, int moves);
void bg_drawMoveableSquares ( int, int, int );
void bg_drawMoveableSquare ( int, int);

void bg_clearMoveable();
void bg_updateMoveable(int x, int y);

void bg_init()
{
    //set up background 0
    //We are using a 256x256 Map which is placed in ScreenBaseBlock 31
    REG_BG3CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (31 << SCREEN_SHIFT);
	REG_BG2CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (30 << SCREEN_SHIFT) | ( 1 << CHAR_SHIFT);
    //set video mode 0 with background 0
    SetMode(0|BG3_ENABLE|BG2_ENABLE|OBJ_ENABLE|OBJ_MAP_1D);
}

void bg_load(int *x, int *y, const u16 * currPal, const u16 * currMap, const u16 * currTiles , const u16 * currShadow, int width, int height)
{ 
	myBg.Pal = currPal;
	myBg.Map = currMap;
	myBg.Tiles = currTiles;
	myBg.Shadow = currShadow;
	myBg.width = width;
	myBg.height = height;
	myBg.mtw = width / 8;
	myBg.mth = height / 8;
	myBg.numtiles = myBg.mtw * myBg.mth;
	myBg.bgtiles = 32 *32;
	
	myBg.Select = moveableMap;
    //copy the palette into the background palette memory
    DMAFastCopy((void*)myBg.Pal, (void*)BGPaletteMem, 256, DMA_16NOW);
    //copy the tile images into the tile memory
    DMAFastCopy((void*)myBg.Tiles, (void*)CharBaseBlock(0), 6976/4, DMA_32NOW);		//need to change to be dynamic
	DMAFastCopy((void*)showmovesTiles, (void*)CharBaseBlock(1),128/4, DMA_32NOW);
    //4992 = #Tiles * 64

    //copy the tile map into background 0
    int i, j, k = 0;
    for ( j = 0; j < 32; j++ )
        for ( i = 0; i < 32; i++ )
		{
            bg3map[k] = myBg.Map[j * myBg.mtw + i];
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

    //find map indexes for current position
    yi = *y / 8; //bg y coordinate
    xi = *x / 8; //bg x coordinate

    //if you have moved 8 pixels down
    if (delta_y == 8  )
		bg_scrollDown();		
    else if (delta_y == -8 )
		bg_scrollUp();

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
        
    //caculate new xi
    xi = *x / 8; //bg x coordinate

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
            bg3map[ bgindex ] = myBg.Map[ mindex ];
			bg2map[ bgindex ] = myBg.Select[ mindex ];
        }
        //reset delta x
        delta_x = 0;
    }
}

void bg_scrollRight()
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
            bg3map[ bgindex ] = myBg.Map[ mindex ];
			bg2map[ bgindex ] = myBg.Select[ mindex ];
        }
        //reset delta x to 0
        delta_x = 0;
    }
}
void bg_scrollUp()
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
            bg3map[ bgindex ] = myBg.Map[ mindex ];
			bg2map[ bgindex ] = myBg.Select[ mindex ];
        }
        //reset delta y
        delta_y = 0;
    }
}

void bg_scrollDown()
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
            bg3map[ bgindex ] = myBg.Map [ mindex ];
			bg2map[ bgindex ] = myBg.Select[ mindex ];
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
{
	bg_drawMoveableSquares ( mysprites[i].x/8, mysprites[i].y/8, moves);
	bg_updateMoveable( *x, *y);
	
	
}
void bg_drawMoveableSquares ( int x, int y, int moves )
{

	if ( myBg.Shadow[y * myBg.mtw + x ] != 0x1001 && x >= 0 && y >= 0 && x < myBg.mtw && y < myBg.mth)
	{
		bg_drawMoveableSquare ( x, y );
		if ( moves )
		{
			if ( myBg.Shadow[y * myBg.mtw + x - 2 ] != 0x1001 )
				bg_drawMoveableSquares ( x - 2, y , moves - 1 );
			if ( myBg.Shadow[y * myBg.mtw + x + 2 ] != 0x1001 )
				bg_drawMoveableSquares ( x + 2, y , moves - 1 );
			if ( myBg.Shadow[( y - 2 ) * myBg.mtw + x ] != 0x1001 )
				bg_drawMoveableSquares ( x, y - 2 , moves - 1 );
			if ( myBg.Shadow[( y + 2 ) * myBg.mtw + x ] != 0x1001 )
				bg_drawMoveableSquares ( x, y  + 2, moves - 1 );
		}
	}
}
	
void bg_drawMoveableSquare ( int x, int y )
{
	myBg.Select[y*myBg.mtw + x] = showmovesMap[2];
	myBg.Select[y*myBg.mtw + x + 1] = showmovesMap[3];
	myBg.Select[(y+1)*myBg.mtw+ x] = showmovesMap[6];
	myBg.Select[(y+1)*myBg.mtw+ x+1] = showmovesMap[7];
}

void bg_clearMoveable()
{
	int i, max = 32 * 32;
	for ( i = 0; i < max; ++i )
		bg2map[i] = showmovesMap[0];
	for ( i = 0; i < myBg.numtiles; ++i )
		myBg.Select[i] =  showmovesMap[0];
}

void bg_updateMoveable(int x, int y)
{
	x /= 8;
	y /= 8;
	int i, j;
	for ( i = 0; i < 32 ; ++i )
		for ( j = 0; j < 32; j++ )
		{
			bg2map[((i+y)%32) * 32 + (j+x)%32 ] = myBg.Select[( y+i ) * myBg.mtw + x + j];
		}
}

