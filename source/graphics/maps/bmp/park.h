
//{{BLOCK(park)

//======================================================================
//
//	park, 256x256@8, 
//	Transparent color : 00,00,00
//	+ palette 256 entries, not compressed
//	+ 60 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 3840 + 2048 = 6400
//
//	Time-stamp: 2011-04-30, 19:45:02
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PARK_H
#define GRIT_PARK_H

#define parkTilesLen 3840
extern const unsigned short parkTiles[1920];

#define parkMapLen 2048
extern const unsigned short parkMap[1024];

#define parkPalLen 512
extern const unsigned short parkPal[256];

#endif // GRIT_PARK_H

//}}BLOCK(park)
