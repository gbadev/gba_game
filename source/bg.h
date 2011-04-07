//bg.h

//globals
const int screen_width = 240;
const int screen_height = 160;
const int map_width = 512;
const int map_height = 512;
int mtw; //map tile width
int mth; //height
const int bgtiles = 32*32;

//map x,y indexes
int xi, yi;

int isValidMapPosition ( int, int );


//create a pointer to background 0 tilemap buffer
unsigned short* bg3map =(unsigned short*) ScreenBaseBlock (31);
unsigned short* bg2map =(unsigned short*) ScreenBaseBlock (30);

int delta_x = 0, delta_y = 0;

void bg_load();

void bg_scroll_left();
void bg_scroll_right();
void bg_scroll_up();
void bg_scroll_down();

void bg_load()
{
    //set up background 0
    //We are using a 256x256 Map which is placed in ScreenBaseBlock 31
    REG_BG3CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (31 << SCREEN_SHIFT);
	REG_BG2CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (30 << SCREEN_SHIFT) | ( 1 << CHAR_SHIFT);
    //set video mode 0 with background 0
    SetMode(0|BG3_ENABLE|BG2_ENABLE|OBJ_ENABLE|OBJ_MAP_1D);

    //copy the palette into the background palette memory
    DMAFastCopy((void*)gravedemoPal, (void*)BGPaletteMem, 256, DMA_16NOW);
    //copy the tile images into the tile memory
    DMAFastCopy((void*)gravedemoTiles, (void*)CharBaseBlock(0), 6976/4, DMA_32NOW);
	DMAFastCopy((void*)bluetileTiles, (void*)CharBaseBlock(1),128/4, DMA_32NOW);
    //4992 = #Tiles * 64

    //copy the tile map into background 0
    int i, j, k = 0;
    for ( j = 0; j < 32; j++ )
        for ( i = 0; i < 32; i++ )
		{
            bg3map[k] = gravedemoMap[j * mtw + i];
			//bg2map[k] = bluetileMap[0];
			k++;
		}
}

void bg_scroll_left()
{
	int bgstart, bgoffset, mstart, moffset, bgindex, mindex, i;
	//moving left
    //find start indexes
    bgstart = ( ((yi+31)%32 ) * 32 ) + ( ( xi + 31 ) %32 );
    mstart = (( yi-1) * mtw ) + ( xi - 1 );
    
	for ( i = 0; i < 32; i++ )
    {
		//find offsets
		bgoffset = i * 32;
        moffset = i * mtw;
                
        //calcuate indexes
        bgindex = ( bgstart + bgoffset ) % bgtiles;
        mindex = mstart + moffset;
                
        //if index is valid
        if ( isValidMapPosition (  xi-1, yi - 1 + i ) == 1)
        {
			//write
			WaitVBlank();
            bg3map[ bgindex ] = gravedemoMap[ mindex ];
        }
        //reset delta x
        delta_x = 0;
    }
}
void bg_scroll_right()
{
	int bgstart, bgoffset, mstart, moffset, bgindex, mindex, i;
	//moving right
    //find start indexes
    bgstart = ( ((yi + 31)%32) * 32 ) + ( (xi + 30) % 32 );
    mstart = ( (yi - 1) * mtw )+ ( xi + 30 );

    for ( i = 0; i < 32; i++ )
    {
		//find offset
        bgoffset = i * 32;
        moffset =  i * mtw;
		//calculate indexes
        bgindex = ( bgstart + bgoffset ) % bgtiles;
        mindex = mstart + moffset;
                
        //if index is valid
        if ( isValidMapPosition (  xi+30, yi - 1 + i ) == 1 )
        {
			//write
			WaitVBlank();
            bg3map[ bgindex ] = gravedemoMap[ mindex ];
        }
        //reset delta x to 0
        delta_x = 0;
    }
}
void bg_scroll_up()
{
	int bgstart, bgoffset, mstart, moffset, bgindex, mindex,i;
	//moving up
    //find start indexes
    bgstart = ( ( (yi + 31 ) % 32) * 32 + ((xi +31)% 32) );
    mstart = ( yi - 1 ) * mtw + xi - 1;
 
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
            bg3map[ bgindex ] = gravedemoMap[ mindex ];
        }
        //reset delta y
        delta_y = 0;
    }
}
void bg_scroll_down()
{
	int bgstart, bgoffset, mstart, moffset, bgindex, mindex,i;
	//moving down
    //bg start
    bgstart = ( ( ( yi + 30 )% 32 ) * 32 + ((xi +31 ) % 32) );
    //map start
    mstart = ( yi + 30 ) * mtw + xi - 1;
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
            bg3map[ bgindex ] = gravedemoMap [ mindex ];
        }
        //reset delta y
        delta_y = 0;
    }
}