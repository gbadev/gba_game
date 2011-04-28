//gamestate.h

void gs_init();

typedef struct tagGSHandler
{
	int mode;
	int hasMoved;
	int hasAttacked;
	int curr;

}GSHandler;
GSHandler myGame;

void gs_init()
{
}

