#include  "dave.h"

#include "graphics/gravedemo.h"
#include "graphics/gravedemoshadow.h"
#include "graphics/bluetile.h"
#include "graphics/robot.h"

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

void SetSpritePos( int index, int x, int y );
void DrawSprite( int index, int x, int y );

void DrawMoveableArea ( int);
void DrawMoveableSquares ( int, int, int );
void DrawMoveableSquare ( int, int);

int main ( void )
{
    //calculate map tile width, height
    mtw = map_width / 8; 
    mth = map_height /8;
    volatile int n;
	int y, x;
    x = y = 0;

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
    
    ///init sprite in upper left hand corner

    //attribute0: color mode, shape and y pos
	
	for ( i =0 ; i < 4; i++)
	{
		SetSpritePos( i, i*128, i*128);
		DrawMoveableArea ( i );
	}
    
	//attribute2: Image location
    sprites[0].attribute2 = 0;
    UpdateSpriteMemory();
	
	
    //counters
    int delta_x = 0, delta_y = 0;

    //set scrolling regitsters to upper right corner of bg
    x = REG_BG3HOFS = 0;
	REG_BG2HOFS = 0 ;
    y = REG_BG3VOFS = 0;
	REG_BG2VOFS = 0 ;
    
    //vars for game loop
    int bgstart, bgoffset, mstart, moffset, bgindex, mindex;
    
	


	
	
    //game loop
    while ( 1 )
    {
        //WaitVBlank();
        
        //button polling
        CheckButtons();

        //process y - axis movement first
        //move up
        if( Pressed ( BUTTON_UP ))
        {
            if ( y > 0)
            {
                y--;
                delta_y--;
            }
        }
        //move down
        if( Pressed ( BUTTON_DOWN))
        {
            if ( (y + screen_height) <  map_height )
            {
                y++;
                delta_y++;
            }
        }

        //find map indexes for current position
        yi = y / 8; //bg y coordinate
        xi = x / 8; //bg x coordinate

        //if you have moved 8 pixels down
        if (delta_y == 8  )
        {
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
                    {
                        bgindex -=32;
                    }
                    //write!
					WaitVBlank();
                    bg3map[ bgindex ] = gravedemoMap [ mindex ];
                }
                //reset delta y
                delta_y = 0;
            }

        }

        else if (delta_y == -8 )
        {
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

        //process x movement
        //move left
        if( Pressed ( BUTTON_LEFT ))
        {
            if ( x > 0)
            {
                x--;
                delta_x--;
            }
        }
        //move right
        if( Pressed ( BUTTON_RIGHT ))
        {
            if ( ( x + screen_width ) < map_width)
            {
                x++;
                delta_x++;
            }
        }
        
        //caculate new xi
        xi = x / 8; //bg x coordinate


        if (delta_x == 8 )
        {
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

        else if (delta_x == -8 )
        {
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

        //update scrolling registers
        REG_BG3VOFS = y ;
		REG_BG2VOFS = y ;
        REG_BG3HOFS = x ;
		REG_BG2HOFS = x ;
		
		for ( i = 0; i < 128; i++ )
		{
			if ( mysprites[i].x - x >= 0 && mysprites[i].y - y >= 0 &&
				mysprites[i].x - x < 240 && mysprites[i].y - y < 240 )
				DrawSprite ( i, mysprites[i].x -x, mysprites[i].y - y );
			else
				DrawSprite ( i, 240, 160);
		}
		UpdateSpriteMemory();
        WaitVBlank();
        for(n = 0; n < 1000; n++);
    }

    return 0;
}

int isValidMapPosition ( int x, int y)
//I:    a position, given by x and y coords
//O:    none
//R:    true if index is valid map index, false
{
    int result = 1;
    if ( x < 0 || y < 0 || x >= mtw || y >= mth )
        result = 0;
    return result;
}

void SetSpritePos( int index, int x, int y )
{
	mysprites[index].x = x;
	mysprites[index].y = y;
}

void DrawSprite( int index, int x, int y )
{
	sprites[index].attribute1 = SIZE_16 | x;
	sprites[index].attribute0 = COLOR_256 | SQUARE | y;
}

void DrawMoveableArea ( int i)
{
	int x = mysprites[i].x, y = mysprites[i].y;
	x/=8;
	y/=8;
	DrawMoveableSquares ( x, y, 3 );
	
}
void DrawMoveableSquares ( int x, int y, int moves )
{

	if ( gravedemoshadowMap[y * mtw + x ] != 0x1001 && x >= 0 && y >= 0 && x < 32 && y < 32)
	{
		DrawMoveableSquare ( x, y );
		if ( moves )
		{
			DrawMoveableSquares ( x - 2, y , moves - 1 );
			DrawMoveableSquares ( x + 2, y , moves - 1 );
			DrawMoveableSquares ( x, y - 2 , moves - 1 );
			DrawMoveableSquares ( x, y  + 2, moves - 1 );
		}
	}
}
void DrawMoveableSquare ( int x, int y )
{
	bg2map[y*32 + x] = bluetileMap[0];
	bg2map[y*32 + x + 1] = bluetileMap[0];
	bg2map[(y+1)*32+ x] = bluetileMap[0];
	bg2map[(y+1)*32+ x+1] = bluetileMap[0];
}

/* END OF FILE */
