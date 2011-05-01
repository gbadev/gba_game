//#include "graphics/font.h"
//#include "graphics/font.c"

//const unsigned short fontTiles[2016];
//const unsigned short fontPal[256];

//#include "graphics/font.h"
//#include "graphics/font.c"

//const unsigned short fontTiles[2016];
//const unsigned short fontPal[256];

void tty_printChar(int, int, char);
int getIndex(char);


void tty_printChar(int x, int y, char c)
{
	bg1map[(y * 32) + (x % 32)] = fontMap[getIndex(c)];
}

void tty_printString (  int x, int y, char * str ,int len)
{
	int i;
	for ( i = 0; i < len; i++)
		tty_printChar(x+i, y, str[i]);
}

int getIndex(char c)
{
	return ( c - ' ' + 1 );
	
}