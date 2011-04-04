#ifndef _DAVE_H
#define _DAVE_H

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "font.h"

#define u16 unsigned short

#define REG_DISPCNT *(unsigned long*) 0x4000000
#define MODE_0 0x0
#define MODE_3 0x3
#define MODE_4 0x4
#define BG3_ENABLE 0x800
#define BG2_ENABLE 0x400
#define BG1_ENABLE 0x200
#define BG0_ENABLE 0x100
#define SetMode(mode) REG_DISPCNT = (mode)
#define RGB(r,g,b) (unsigned short)(r+(g<<5)+(b<<10))

unsigned short* videoBuffer = (unsigned short*)0x6000000;
inline void DrawPixel3(int x, int y, unsigned short c)
{
    videoBuffer[y*240+x] = c;
}

//buttons
volatile unsigned int *BUTTONS = ( volatile unsigned int*) 0x04000130;
#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_SELECT 4
#define BUTTON_START 8
#define BUTTON_RIGHT 16
#define BUTTON_LEFT 32
#define BUTTON_UP 64
#define BUTTON_DOWN 128
#define BUTTON_R 256
#define BUTTON_L 512
#define bool int
#define true 1
#define false 0
bool buttons[10];
//button polling


void CheckButtons()
{
    buttons[0] = !((*BUTTONS) & BUTTON_A);
    buttons[1] = !((*BUTTONS) & BUTTON_B);
    buttons[2] = !((*BUTTONS) & BUTTON_LEFT);
    buttons[3] = !((*BUTTONS) & BUTTON_RIGHT);
    buttons[4] = !((*BUTTONS) & BUTTON_UP);
    buttons[5] = !((*BUTTONS) & BUTTON_DOWN);
    buttons[6] = !((*BUTTONS) & BUTTON_START);
    buttons[7] = !((*BUTTONS) & BUTTON_SELECT);
    buttons[8] = !((*BUTTONS) & BUTTON_R);
    buttons[9] = !((*BUTTONS) & BUTTON_L);
}

bool Pressed(int button)
{
    switch(button)
    {
        case BUTTON_A: return buttons[0];
        case BUTTON_B: return buttons[1];
        case BUTTON_LEFT: return buttons[2];
        case BUTTON_RIGHT: return buttons[3];
        case BUTTON_UP: return buttons[4];
        case BUTTON_DOWN: return buttons[5];
        case BUTTON_START: return buttons[6];
        case BUTTON_SELECT: return buttons[7];
        case BUTTON_L: return buttons[8];
        case BUTTON_R: return buttons[9];
    }
}
void DrawLine3(int x1, int y1, int x2, int y2, unsigned short color)
{
    int i, deltax, deltay, numpixels;
    int d, dinc1, dinc2;
    int x, xinc1, xinc2;
    int y, yinc1, yinc2;
    //calculate deltaX and deltaY
    deltax = abs(x2 - x1);
    deltay = abs(y2 - y1);
    //initialize
    if(deltax >= deltay)
    {
        //If x is independent variable
        numpixels = deltax + 1;
        d = (2 * deltay) - deltax;
        dinc1 = deltay << 1;
        dinc2 = (deltay - deltax) << 1;
        xinc1 = 1;
        xinc2 = 1;
        yinc1 = 0;
        yinc2 = 1;
    }
    else
    {
        //if y is independant variable
        numpixels = deltay + 1;
        d = (2 * deltax) - deltay;
        dinc1 = deltax << 1;
        dinc2 = (deltax - deltay) << 1;
        xinc1 = 0;
        xinc2 = 1;
        yinc1 = 1;
        yinc2 = 1;
    }
    //move the right direction
    if(x1 > x2)
    {
        xinc1 = -xinc1;
        xinc2 = -xinc2;
    }
    if(y1 > y2)
    {
        yinc1 = -yinc1;
        yinc2 = -yinc2;
    }
    x = x1;
    y = y1;
    //draw the pixels
    for(i = 1; i < numpixels; i++)
    {
        DrawPixel3(x, y, color);
        if(d < 0)
        {
            d = d + dinc1;
            x = x + xinc1;
            y = y + yinc1;
        }
        else
        {
            d = d + dinc2;
            x = x + xinc2;
            y = y + yinc2;
        }
    }
}
void DrawCircle3(int xCenter, int yCenter, int radius, unsigned short color)
{
    int x = 0;
    int y = radius;
    int p = 3 - 2 * radius;
    while (x <= y)
    {
        if ( xCenter + x < 240 && yCenter + y < 160 )
            DrawPixel3(xCenter + x, yCenter + y, color);

        if ( xCenter - x >= 0 && yCenter + y < 160 )
            DrawPixel3(xCenter - x, yCenter + y, color);

        if ( xCenter + x < 240 && yCenter - y >= 0 )
            DrawPixel3(xCenter + x, yCenter - y, color);

        if ( xCenter - x >= 0 && yCenter - y >= 0 )
            DrawPixel3(xCenter - x, yCenter - y, color);

        if ( xCenter + y < 240 && yCenter + x < 160 )
            DrawPixel3(xCenter + y, yCenter + x, color);

        if ( xCenter - y >= 0 && yCenter + x < 160 )
            DrawPixel3(xCenter - y, yCenter + x, color);

        if ( xCenter + y < 240 && yCenter - x >=0 )
            DrawPixel3(xCenter + y, yCenter - x, color);

        if ( xCenter - y >= 0 && yCenter - x  >=0 )
            DrawPixel3(xCenter - y, yCenter - x, color);

        if (p < 0)
            p += 4 * x++ + 6;
        else
            p += 4 * (x++ - y--) + 10;
    }
}
void DrawCircleFull3(int xCenter, int yCenter, int radius, unsigned short color)
{
    int x = 0;
    int y = radius;
    int p = 3 - 2 * radius;
    while (x <= y)
    {
        int tmp[8] = { xCenter+x,yCenter+y, xCenter-x, yCenter-y,
                        xCenter+y, xCenter-y, yCenter+x, yCenter-x};
        int i;
        for ( i = 0; i < 8; ++i )
        {
            if (tmp[i] < -1 )
                tmp[i] = -1;
            if (tmp[i] > 239)
                tmp[i] = 239;
        }
        //DrawBox3(xCenter+x, yCenter+y, xCenter-x, yCenter+y, color);
        //DrawBox3(xCenter+x, yCenter-y, xCenter-x, yCenter-y, color);

        //DrawPixel3(xCenter + x, yCenter + y, color);
        //DrawPixel3(xCenter - x, yCenter + y, color);
        DrawLine3(tmp[0], tmp[1], tmp[2], tmp[1], color );

        //DrawPixel3(xCenter + x, yCenter - y, color);
        //DrawPixel3(xCenter - x, yCenter - y, color);
        DrawLine3(tmp[0], tmp[3], tmp[2], tmp[3], color );

        //DrawPixel3(xCenter + y, yCenter + x, color);
        //DrawPixel3(xCenter - y, yCenter + x, color);
        DrawLine3(tmp[4], tmp[6], tmp[5], tmp[6], color );

        //DrawPixel3(xCenter + y, yCenter - x, color);
        //DrawPixel3(xCenter - y, yCenter - x, color);*/
        DrawLine3(tmp[4], tmp[7] , tmp[5], tmp[7], color );

        

        if (p < 0)
            p += 4 * x++ + 6;
        else
            p += 4 * (x++ - y--) + 10;
    }
}

void DrawBox3(int left, int top, int right, int bottom, unsigned short color)
{
    int tmp;
    if ( left > right )
    {
        tmp = right;
        right = left;
        left = tmp;
        right++;
    }
    if ( bottom < top )
    {
        tmp = top;
        top = bottom;
        bottom = tmp;
        bottom++;
    }
        
    int x, y;
    for(y = top; y < bottom; y++)
        for(x = left; x < right; x++)
            DrawPixel3(x, y, color);
}
void DrawBoxEmpty3(int left, int top, int right, int bottom, unsigned short color)
{
    int tmp;
    if ( left > right )
    {
        tmp = right;
        right = left;
        left = tmp;
        //right++;
    }
    if ( bottom < top )
    {
        tmp = top;
        top = bottom;
        bottom = tmp;
        //bottom++;
    }
    DrawLine3(left, top, right, top, color);
    DrawLine3(left, bottom, right+1, bottom, color);
    DrawLine3(left, top, left, bottom, color);
    DrawLine3(right, top, right, bottom, color );
}


unsigned short* paletteMem = (unsigned short*)0x5000000;
inline void DrawPixel4(int x, int y, unsigned char color)
{
    unsigned short pixel;
    unsigned short offset = (y * 240 + x) >> 1;
    pixel = videoBuffer[offset];
    if (x & 1)
        videoBuffer[offset] = (color << 8) + (pixel & 0x00FF);
    else
        videoBuffer[offset] = (pixel & 0xFF00) + color;
}

volatile unsigned short* ScanlineCounter = (volatile unsigned short*)0x4000006;
void WaitVBlank(void)
{
    while(*ScanlineCounter < 160)
    {
    }
}

unsigned short* FrontBuffer = (unsigned short*)0x6000000;
unsigned short* BackBuffer = (unsigned short*)0x600A000;
#define BACKBUFFER 0x10
void FlipPage(void)
{
    if(REG_DISPCNT & BACKBUFFER)
    {
        REG_DISPCNT &= ~BACKBUFFER;
        videoBuffer = BackBuffer;
    }
    else
    {
        REG_DISPCNT |= BACKBUFFER;
        videoBuffer = FrontBuffer;
    }
}

void DrawBmp4 ( int x, int y, const unsigned char * bmp, int width, int height )
//I:    a bitmap to draw, the position on screen to draw it, the dimensions of the bmp
//O:    The bitmap is drawn to current videobuffer
//R:    None
{
    int i, j, k = 0;
    int ymax = y + height;
    int xmax = x + width;
    for ( i = y; i < ymax ; ++i)
    {
        for ( j = x; j < xmax; ++j )
        {
            //if this is a valid pixel
            if ( j >= 0 && j < 240 && i >= 0 && i < 160 )
                //draw it
                DrawPixel4(j,i, bmp[k]);
            //go to next pixel to draw
            k++;
        }
    }
}


void DrawBmp3 ( int x, int y, const unsigned short * bmp, int width, int height )
//I:    a bitmap to draw, the position on screen to draw it, the dimensions of the bmp
//O:    The bitmap is drawn to videobuffer
//R:    None
{
    int i, j, k = 0;
    int ymax = y + height;
    int xmax = x + width;
    for ( i = y; i < ymax ; ++i)
    {
        for ( j = x; j < xmax; ++j )
        {
            //if this is a valid pixel
            if ( j >= 0 && j < 240 && i >= 0 && i < 160 )
                //draw it
                DrawPixel3(j,i, bmp[k]);
            //go to next pixel to draw
            k++;
        }
    }
}

//STUFF FOR PROJECT 4

void DrawChar(int left, int top, char letter, unsigned short color, int size)
//I:    a position, a character to draw, a color, and a size
//O:    char of given size and color is drawn to videoBuffer
//R:    none
{
    int x, y;
    int draw;
    //number of pixels to display char = max * max
    int max = 8 * size;
    //for each pixel
    for(y = 0; y < max; y++)
        for (x = 0; x < max; x++)
        {
            // grab a pixel from the font char, take size into account
            draw = font[(letter-32) * 64 + y/size * 8 + x/size];
            // if pixel = 1, then draw it
            if (draw == 1)
                DrawPixel3(left + x, top + y, color);
        }
}

int findOffset ( char ch )
//I:    a character
//O:    offset for printing next character is found
//R:    the offset
{
    int i, offset = 8;
    //find the first 2 (denotes end of needed pixels to display char)
    for ( i = 0; i < 8; i++)
        if (  font[(ch-32) * 64 + i ] == 2 )
            offset = i;
    return offset;
}

void Print(int left, int top, char *str, unsigned short color, int size)
//I:    a position, a string, a color, and a size
//O:    string of given size and color is printed at postion
//R:    none
{
    int pos = 0;
    while (*str)
    {
        DrawChar(left + pos, top, *str, color, size);
        //next pos depends on offset and size
        pos += findOffset( *str++ ) * size;
    }
}

//STUFF FOR PROJECT 5
#define REG_DMA3SAD *(volatile unsigned int*)0x40000D4
#define REG_DMA3DAD *(volatile unsigned int*)0x40000D8
#define REG_DMA3CNT *(volatile unsigned int*)0x40000DC
#define DMA_ENABLE 0x80000000
#define DMA_TIMING_IMMEDIATE 0x00000000
#define DMA_16 0x00000000
#define DMA_32 0x04000000
#define DMA_32NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_32)
#define DMA_16NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_16)

void DMAFastCopy(void* source, void* dest, unsigned int count, unsigned int mode)
{
    if (mode == DMA_16NOW || mode == DMA_32NOW)
    {
        REG_DMA3SAD = (unsigned int)source;
        REG_DMA3DAD = (unsigned int)dest;
        REG_DMA3CNT = count | mode;
    }
}

#define REG_BG0HOFS *(volatile unsigned short*)0x4000010
#define REG_BG0VOFS *(volatile unsigned short*)0x4000012
#define REG_BG1HOFS *(volatile unsigned short*)0x4000014
#define REG_BG1VOFS *(volatile unsigned short*)0x4000016
#define REG_BG2HOFS *(volatile unsigned short*)0x4000018
#define REG_BG2VOFS *(volatile unsigned short*)0x400001A
#define REG_BG3HOFS *(volatile unsigned short*)0x400001C
#define REG_BG3VOFS *(volatile unsigned short*)0x400001E

//background setup registers and data
#define REG_BG0CNT *(volatile unsigned short*)0x4000008
#define REG_BG1CNT *(volatile unsigned short*)0x400000A
#define REG_BG2CNT *(volatile unsigned short*)0x400000C
#define REG_BG3CNT *(volatile unsigned short*)0x400000E
#define BG_COLOR256 0x80
#define CHAR_SHIFT 2
#define SCREEN_SHIFT 8
#define WRAPAROUND 0x1

//Palette for Backgrounds
#define BGPaletteMem ((unsigned short*)0x5000000)
//background tile bitmap sizes
#define TEXTBG_SIZE_256x256 0x0
#define TEXTBG_SIZE_256x512 0x8000
#define TEXTBG_SIZE_512x256 0x4000
#define TEXTBG_SIZE_512x512 0xC000
//background memory offset macros
#define CharBaseBlock(n) (((n)*0x4000)+0x6000000)
#define ScreenBaseBlock(n) (((n)*0x800)+0x6000000)

//SPRITES!
typedef struct tagSprite
{
    unsigned short attribute0;
    unsigned short attribute1;
    unsigned short attribute2;
    unsigned short attribute3;
}Sprite,*pSprite;
Sprite sprites[128];

typedef struct tagSpriteHanlder
{
    int x, y;
    int size;
    int dirx, diry;
    int alive;
    int flipped;

}SpriteHanlder;
SpriteHanlder mysprites[128];

//OAM
#define SpriteMem ((unsigned short*)0x7000000)
//Sprite Image data
#define SpriteData ((unsigned short*) 0x6010000)
//Sprite Palette
#define SpritePal ((unsigned short*) 0x5000200)
//other constants
#define OBJ_MAP_2D 0x0
#define OBJ_MAP_1D 0x40
#define OBJ_ENABLE 0x1000
//attribute0 stuff
#define ROTATION_FLAG 0x100
#define SIZE_DOUBLE 0x200
#define MODE_NORMAL 0x0
#define MODE_TRANSPERANT 0x400
#define MODE_WINDOWED 0x800
#define MOSAIC 0x1000
#define COLOR_16 0x0000
#define COLOR_256 0x2000
#define SQUARE 0x0
#define TALL 0x8000
#define WIDE 0x4000
//attribute1 stuff
#define ROTDATA(n) ((n) << 9)
#define HORIZONTAL_FLIP 0x1000
#define VERTICAL_FLIP 0x2000
#define SIZE_8 0x0
#define SIZE_16 0x4000
#define SIZE_32 0x8000
#define SIZE_64 0xC000
//attribute2 stuff
#define PRIORITY(n) ((n) << 10)
#define PALETTE(n) ((n) << 12)

void UpdateSpriteMemory(void)
{
    DMAFastCopy((void*)sprites, (void*)SpriteMem, 512, DMA_16NOW);
}
#endif
