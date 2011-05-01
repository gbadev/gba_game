
//{{BLOCK(park_shadow)

//======================================================================
//
//	park_shadow, 256x256@8, 
//	Transparent color : 00,00,00
//	+ palette 256 entries, not compressed
//	+ 2 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 128 + 2048 = 2688
//
//	Time-stamp: 2011-04-30, 19:45:02
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PARK_SHADOW_H
#define GRIT_PARK_SHADOW_H

#define park_shadowTilesLen 128
extern const unsigned short park_shadowTiles[64];

#define park_shadowMapLen 2048
extern const unsigned short park_shadowMap[1024];

#define park_shadowPalLen 512
extern const unsigned short park_shadowPal[256];

#endif // GRIT_PARK_SHADOW_H

//}}BLOCK(park_shadow)
