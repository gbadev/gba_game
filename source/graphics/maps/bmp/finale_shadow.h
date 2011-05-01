
//{{BLOCK(finale_shadow)

//======================================================================
//
//	finale_shadow, 512x256@8, 
//	+ palette 256 entries, not compressed
//	+ 2 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 64x32 
//	Total size: 512 + 128 + 4096 = 4736
//
//	Time-stamp: 2011-05-01, 00:41:51
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FINALE_SHADOW_H
#define GRIT_FINALE_SHADOW_H

#define finale_shadowTilesLen 128
extern const unsigned short finale_shadowTiles[64];

#define finale_shadowMapLen 4096
extern const unsigned short finale_shadowMap[2048];

#define finale_shadowPalLen 512
extern const unsigned short finale_shadowPal[256];

#endif // GRIT_FINALE_SHADOW_H

//}}BLOCK(finale_shadow)
