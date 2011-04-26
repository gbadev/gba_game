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

int getIndex(char c)
{
	int retval = 0;
	switch(c)
	{
		case ' ':
		{
			retval = 1;
			break;
		}
		case '!':
		{
			retval = 2;
			break;
		}
		case '"':
		{
			retval = 3;
			break;
		}
		case '#':
		{
			retval = 4;
			break;
		}
		case '$':
		{
			retval = 5;
			break;
		}
		case '%':
		{
			retval = 6;
			break;
		}
		case '&':
		{
			retval = 7;
			break;
		}
		case '\'':
		{
			retval = 8;
			break;
		}
		case '(':
		{
			retval = 9;
			break;
		}
		case ')':
		{
			retval = 10;
			break;
		}
		case '*':
		{
			retval = 11;
			break;
		}
		case '+':
		{
			retval = 12;
			break;
		}
		case ',':
		{
			retval = 13;
			break;
		}
		case '-':
		{
			retval = 14;
			break;
		}
		case '.':
		{
			retval = 15;
			break;
		}
		case '/':
		{
			retval = 16;
			break;
		}
		case '0':
		{
			retval = 17;
			break;
		}
		case '1':
		{
			retval = 18;
			break;
		}
		case '2':
		{
			retval = 19;
			break;
		}
		case '3':
		{
			retval = 20;
			break;
		}
		case '4':
		{
			retval = 21;
			break;
		}
		case '5':
		{
			retval = 22;
			break;
		}
		case '6':
		{
			retval = 23;
			break;
		}
		case '7':
		{
			retval = 24;
			break;
		}
		case '8':
		{
			retval = 25;
			break;
		}
		case '9':
		{
			retval = 26;
			break;
		}
		case ':':
		{
			retval = 27;
			break;
		}
		case ';':
		{
			retval = 28;
			break;
		}
		case '<':
		{
			retval = 29;
			break;
		}
		case '=':
		{
			retval = 30;
			break;
		}
		case '>':
		{
			retval = 31;
			break;
		}
		case '?':
		{
			retval = 32;
			break;
		}
		case '@':
		{
			retval = 33;
			break;
		}
		case 'A':
		{
			retval = 34;
			break;
		}
		case 'B':
		{
			retval = 35;
			break;
		}
		case 'C':
		{
			retval = 36;
			break;
		}
		case 'D':
		{
			retval = 37;
			break;
		}
		case 'E':
		{
			retval = 38;
			break;
		}
		case 'F':
		{
			retval = 39;
			break;
		}
		case 'G':
		{
			retval = 40;
			break;
		}
		case 'H':
		{
			retval = 41;
			break;
		}
		case 'I':
		{
			retval = 42;
			break;
		}
		case 'J':
		{
			retval = 43;
			break;
		}
		case 'K':
		{
			retval = 44;
			break;
		}
		case 'L':
		{
			retval = 45;
			break;
		}
		case 'M':
		{
			retval = 46;
			break;
		}
		case 'N':
		{
			retval = 47;
			break;
		}
		case 'O':
		{
			retval = 48;
			break;
		}
		case 'P':
		{
			retval = 49;
			break;
		}
		case 'Q':
		{
			retval = 50;
			break;
		}
		case 'R':
		{
			retval = 51;
			break;
		}
		case 'S':
		{
			retval = 52; 
			break;
		}
		case 'T':
		{
			retval = 53;
			break;
		}
		case 'U':
		{
			retval = 54;
			break;
		}
		case 'V':
		{
			retval = 55;
			break;
		}
		case 'W':
		{
			retval = 56;
			break;
		}
		case 'X':
		{
			retval = 57;
			break;
		}
		case 'Y':
		{
			retval = 58;
			break;
		}
		case 'Z':
		{
			retval = 59;
			break;
		}
		case '[':
		{
			retval = 60;
			break;
		}
		case '\\':
		{
			retval = 61;
			break;
		}
		case ']':
		{
			retval = 62;
			break;
		}
		case '_':
		{
			retval = 63;
			break;
		}
		default:
		{
			retval = 0;
			break;
		}
	}
	return retval;
}