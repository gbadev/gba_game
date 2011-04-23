
//{{BLOCK(ui)

//======================================================================
//
//	ui, 32x16@8, 
//	+ palette 256 entries, not compressed
//	+ 4 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 4x2 
//	Total size: 512 + 256 + 16 = 784
//
//	Time-stamp: 2011-04-23, 04:52:31
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_UI_H
#define GRIT_UI_H

#define uiTilesLen 256
extern const unsigned short uiTiles[128];

#define uiMapLen 16
extern const unsigned short uiMap[8];

#define uiPalLen 512
extern const unsigned short uiPal[256];

#endif // GRIT_UI_H

//}}BLOCK(ui)
