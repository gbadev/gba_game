
//{{BLOCK(bluetile)

//======================================================================
//
//	bluetile, 16x16@8, 
//	+ palette 256 entries, not compressed
//	+ 2 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 2x2 
//	Total size: 512 + 128 + 8 = 648
//
//	Time-stamp: 2011-04-02, 04:11:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BLUETILE_H
#define GRIT_BLUETILE_H

#define bluetileTilesLen 128
extern const unsigned short bluetileTiles[64];

#define bluetileMapLen 8
extern const unsigned short bluetileMap[4];

#define bluetilePalLen 512
extern const unsigned short bluetilePal[256];

#endif // GRIT_BLUETILE_H

//}}BLOCK(bluetile)
