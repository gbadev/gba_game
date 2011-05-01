#include "master_pal_bgs.h"
#include "park.h"
#include "park_shadow.h"
#include "pizza.h"
#include "pizza_shadow.h"
#include "park.h"
#include "park_shadow.h"
#include "finale.h"
#include "finale_shadow.h"

typedef struct tagLvlHandler
{
	const u16 * pal;
	const u16 * tiles ;
	const u16 * map; 	//curr map that is being played
	const u16 * shadow; 	//curr map's shadow/hit map for passable terrain

	int width;	//width in pixels
	int height;	//height in pixels
	
	int numStartZombies;
	int maxZombies;
	
	int len;
	

}LvlHandler;
LvlHandler myLvl[4];
void maps_init()
{
	//thoms lvl
	//bg_load(&x, &y, master_pal_bgsPal, parkMap, parkTiles , park_shadowMap, 256, 256);
	myLvl[0].pal = master_pal_bgsPal;
	myLvl[0].tiles = parkTiles;
	myLvl[0].map = parkMap;
	myLvl[0].shadow = park_shadowMap;
	myLvl[0].width = 256;
	myLvl[0].height =256;
	myLvl[0].numStartZombies = 10;
	myLvl[0].maxZombies =10;
	myLvl[0].len = 1920;
	
	//scotts lvl
	myLvl[1].pal =master_pal_bgsPal;
	myLvl[1].tiles = pizzaTiles;
	myLvl[1].map = pizzaMap;
	myLvl[1].shadow = pizza_shadowMap;
	myLvl[1].width = 256;
	myLvl[1].height = 256;
	myLvl[1].numStartZombies = 10;
	myLvl[1].maxZombies = 10;
	myLvl[1].len = 32800;

	//daves lvl
	/*
	lvlHandler myLvl[2].pal = master_pal_bgsPal;
	lvlHandler myLvl[2].tiles = gravedemoSha;
	lvlHandler myLvl[2].map =;
	lvlHandler myLvl[2].shadow =;
	lvlHandler myLvl[2].width =;
	lvlHandler myLvl[2].height =;
	lvlHandler myLvl[2].numStartZombies =;
	lvlHandler myLvl[2].maxZombies =;*/

	//seans lvl
	myLvl[3].pal =master_pal_bgsPal;
	myLvl[3].tiles =finaleTiles;
	myLvl[3].map =finaleMap;
	myLvl[3].shadow =finale_shadowMap;
	myLvl[3].width = 512;
	myLvl[3].height = 256;
	myLvl[3].numStartZombies = 20;
	myLvl[3].maxZombies =20;
	myLvl[3].len = 32800;
}