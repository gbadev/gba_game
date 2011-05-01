
//{{BLOCK(gravedemo)

//======================================================================
//
//	gravedemo, 512x512@8, 
//	+ palette 256 entries, not compressed
//	+ 109 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 64x64 
//	Total size: 512 + 6976 + 8192 = 15680
//
//	Time-stamp: 2011-04-02, 04:11:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GRAVEDEMO_H
#define GRIT_GRAVEDEMO_H

#define gravedemoTilesLen 6976
extern const unsigned short gravedemoTiles[3488];

#define gravedemoMapLen 8192
extern const unsigned short gravedemoMap[4096];

#define gravedemoPalLen 512
extern const unsigned short gravedemoPal[256];

#endif // GRIT_GRAVEDEMO_H

//}}BLOCK(gravedemo)
