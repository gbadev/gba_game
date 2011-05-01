
//{{BLOCK(font)

//======================================================================
//
//	font, 8x544@8, 
//	Transparent color : 00,00,00
//	+ palette 256 entries, not compressed
//	+ 65 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 1x68 
//	Total size: 512 + 4160 + 136 = 4808
//
//	Time-stamp: 2011-04-30, 19:45:02
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FONT_H
#define GRIT_FONT_H

#define fontTilesLen 4160
extern const unsigned short fontTiles[2080];

#define fontMapLen 136
extern const unsigned short fontMap[68];

#define fontPalLen 512
extern const unsigned short fontPal[256];

#endif // GRIT_FONT_H

//}}BLOCK(font)
