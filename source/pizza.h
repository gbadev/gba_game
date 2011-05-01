
//{{BLOCK(pizza)

//======================================================================
//
//	pizza, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 40 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 2560 + 2048 = 5120
//
//	Time-stamp: 2011-05-01, 00:41:51
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PIZZA_H
#define GRIT_PIZZA_H

#define pizzaTilesLen 2560
extern const unsigned short pizzaTiles[1280];

#define pizzaMapLen 2048
extern const unsigned short pizzaMap[1024];

#define pizzaPalLen 512
extern const unsigned short pizzaPal[256];

#endif // GRIT_PIZZA_H

//}}BLOCK(pizza)
