//#include "master_pal_bgs.h"
#include "park.h"
#include "park_shadow.h"
#include "pizza.h"
#include "pizza_shadow.h"
#include "park.h"
#include "park_shadow.h"
#include "finale.h"
#include "finale_shadow.h"
////////////////////remove
#include "gravedemo.h"
#include "gravedemoshadow.h"

extern int gs_getCurrLvlIndex ( );
//init chars
extern void sprite_init_tank( int x, int y );
extern void sprite_init_gren( int x, int y );
extern void sprite_init_snip( int x, int y );
extern void sprite_init_heal( int x, int y );
//init zombies
extern void sprite_init_zomb(int index, int x, int y );
extern void sprite_init_spit( int index, int x, int y );


typedef struct tagLvlHandler
{
	//map data
	const u16 * pal;
	const u16 * tiles ;
	const u16 * map; 	//curr map that is being played
	const u16 * shadow; 	//curr map's shadow/hit map for passable terrain

	int width;	//width in pixels
	int height;	//height in pixels
	
	//zombie counters
	int numStartZombies;
	int maxZombies;
	int numZombies;

	
	//non passable index ( used for shadow map )
	int npi ;
	
	//length of associated tiles array.
	int len;
	
	//start positions of player chars
	int tank_x, tank_y, gren_x, gren_y, snip_x, snip_y, heal_x, heal_y;

}LvlHandler;
LvlHandler myLvl[4];
void maps_init()
{
	//thoms lvl
	//bg_load(&x, &y, master_pal_bgsPal, parkMap, parkTiles , park_shadowMap, 256, 256);
	myLvl[0].pal = parkPal;
	myLvl[0].tiles = parkTiles;
	myLvl[0].map = parkMap;
	myLvl[0].shadow = park_shadowMap;
	myLvl[0].width = 256;
	myLvl[0].height =256;
	myLvl[0].numStartZombies = 10;
	myLvl[0].maxZombies =10;
	myLvl[0].numZombies = 10;
	myLvl[0].len = 1920;
	myLvl[0].npi = 0x0001;
	myLvl[0].tank_x = 0;
	myLvl[0].tank_y = 0;
	myLvl[0].gren_x = 1;
	myLvl[0].gren_y = 0;
	myLvl[0].snip_x = 2;
	myLvl[0].snip_y = 0;
	myLvl[0].heal_x = 3;
	myLvl[0].heal_y = 0;
	
	
	//scotts lvl
	myLvl[1].pal =pizzaPal;
	myLvl[1].tiles = pizzaTiles;
	myLvl[1].map = pizzaMap;
	myLvl[1].shadow = pizza_shadowMap;
	myLvl[1].width = 256;
	myLvl[1].height = 256;
	myLvl[1].numStartZombies = 10;
	myLvl[1].maxZombies = 10;
	myLvl[1].numZombies = 10;
	myLvl[1].len = 1280;
	myLvl[1].npi = 0x0001;
	myLvl[1].tank_x = 0;
	myLvl[1].tank_y = 0;
	myLvl[1].gren_x = 1;
	myLvl[1].gren_y = 0;
	myLvl[1].snip_x = 2;
	myLvl[1].snip_y = 0;
	myLvl[1].heal_x = 3;
	myLvl[1].heal_y = 0;

	//daves lvl
	myLvl[2].pal = gravedemoPal;
	myLvl[2].tiles = gravedemoTiles;
	myLvl[2].map = gravedemoMap;
	myLvl[2].shadow =gravedemoshadowMap;
	myLvl[2].width = 512;
	myLvl[2].height =512;
	myLvl[2].numStartZombies =96;
	myLvl[2].maxZombies =96;
	myLvl[2].numZombies = 96;
	myLvl[2].len = 3488;
	myLvl[2].npi = 0x1001;
	myLvl[2].tank_x = 0;
	myLvl[2].tank_y = 0;
	myLvl[2].gren_x = 1;
	myLvl[2].gren_y = 0;
	myLvl[2].snip_x = 2;
	myLvl[2].snip_y = 0;
	myLvl[2].heal_x = 3;
	myLvl[2].heal_y = 0;

	//seans lvl
	myLvl[3].pal = finalePal;
	myLvl[3].tiles =finaleTiles;
	myLvl[3].map =finaleMap;
	myLvl[3].shadow =finale_shadowMap;
	myLvl[3].width = 512;
	myLvl[3].height = 256;
	myLvl[3].numStartZombies = 20;
	myLvl[3].maxZombies =20;
	myLvl[3].numZombies = 20;
	myLvl[3].len = 1312;
	myLvl[3].npi = 0x0001;
	myLvl[3].tank_x = 0;
	myLvl[3].tank_y = 0;
	myLvl[3].gren_x = 1;
	myLvl[3].gren_y = 0;
	myLvl[3].snip_x = 2;
	myLvl[3].snip_y = 0;
	myLvl[3].heal_x = 3;
	myLvl[3].heal_y = 0;
}

void map_initCharacters()
{
	//chars init
	sprite_init_tank( myLvl[gs_getCurrLvlIndex ( )].tank_x, myLvl[gs_getCurrLvlIndex ( )].tank_y );
	sprite_init_gren( myLvl[gs_getCurrLvlIndex ( )].gren_x, myLvl[gs_getCurrLvlIndex ( )].gren_y );
	sprite_init_snip( myLvl[gs_getCurrLvlIndex ( )].snip_x, myLvl[gs_getCurrLvlIndex ( )].snip_y );
	sprite_init_heal( myLvl[gs_getCurrLvlIndex ( )].heal_x, myLvl[gs_getCurrLvlIndex ( )].heal_y );
}

void map_initZombies ()
{
	//zombs
	int i = 0;
	for ( i = 0; i < myLvl[gs_getCurrLvlIndex()].numStartZombies; i++ )
	{
		int x, y;
		do
		{
			x = rand() % ( myLvl[gs_getCurrLvlIndex()].width / 16 - 8 ) + 8;
			y = rand() % ( myLvl[gs_getCurrLvlIndex()].height / 16 - 8) + 8;
		} while (!bg_tileIsPlaceable ( x * 16, y * 16 ));
		if ( i % 2 )
			sprite_init_zomb ( i+ 4, x, y);
		else
			sprite_init_spit ( i+ 4, x, y);
	}
}

void removeZombie()
{
	myLvl[gs_getCurrLvlIndex()].numZombies--;
}