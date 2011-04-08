
//{{BLOCK(showmoves)

//======================================================================
//
//	showmoves, 32x16@8, 
//	+ palette 256 entries, not compressed
//	+ 2 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 4x2 
//	Total size: 512 + 128 + 16 = 656
//
//	Time-stamp: 2011-04-08, 01:01:16
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SHOWMOVES_H
#define GRIT_SHOWMOVES_H

#define showmovesTilesLen 128
extern const unsigned short showmovesTiles[64];

#define showmovesMapLen 16
extern const unsigned short showmovesMap[8];

#define showmovesPalLen 512
extern const unsigned short showmovesPal[256];

#endif // GRIT_SHOWMOVES_H

//}}BLOCK(showmoves)
