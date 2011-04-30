
@{{BLOCK(showmoves)

@=======================================================================
@
@	showmoves, 32x16@8, 
@	+ palette 256 entries, not compressed
@	+ 8 tiles not compressed
@	Total size: 512 + 512 = 1024
@
@	Time-stamp: 2011-04-30, 15:14:46
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global showmovesTiles		@ 512 bytes
showmovesTiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x41414100,0x41414141,0x00004100,0x00000000,0x00004100,0x00000000
	.word 0x00004100,0x00000000,0x00004100,0x00000000,0x00004100,0x00000000,0x00004100,0x00000000
	.word 0x00000000,0x00000000,0x41414141,0x00414141,0x00000000,0x00410000,0x00000000,0x00410000
	.word 0x00000000,0x00410000,0x00000000,0x00410000,0x00000000,0x00410000,0x00000000,0x00410000

	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x00004100,0x00000000,0x00004100,0x00000000,0x00004100,0x00000000,0x00004100,0x00000000
	.word 0x00004100,0x00000000,0x00004100,0x00000000,0x41414100,0x41414141,0x00000000,0x00000000
	.word 0x00000000,0x00410000,0x00000000,0x00410000,0x00000000,0x00410000,0x00000000,0x00410000
	.word 0x00000000,0x00410000,0x00000000,0x00410000,0x41414141,0x00414141,0x00000000,0x00000000
	.size	showmovesTiles, .-showmovesTiles

	.section .rodata
	.align	2
	.global showmovesPal		@ 512 bytes
showmovesPal:
	.hword 0x0000,0x0002,0x043E,0x0C7A,0x18D6,0x1CEC,0x0853,0x042F
	.hword 0x042A,0x211E,0x2D7D,0x463D,0x5ADD,0x7426,0x5CE9,0x3D09
	.hword 0x20A6,0x5024,0x2802,0x750B,0x75D0,0x7654,0x76F8,0x0BA6
	.hword 0x16E8,0x1DE8,0x1926,0x06A5,0x0562,0x1FAB,0x3FB1,0x57B6
	.hword 0x0BBD,0x1339,0x1E10,0x194A,0x06B5,0x01EF,0x0129,0x1BBD
	.hword 0x2FBD,0x47BD,0x5BBD,0x09FC,0x15D6,0x1D70,0x190A,0x0575
	.hword 0x0510,0x00CB,0x161C,0x2A7C,0x3ABD,0x5B5E,0x7458,0x5CD4
	.hword 0x40EF,0x28EA,0x5432,0x402D,0x2808,0x74F9,0x755A,0x763B

	.hword 0x76DC,0x6B83,0x52C6,0x3E08,0x2946,0x4EA2,0x3E01,0x2D81
	.hword 0x6FA9,0x6FAE,0x73B3,0x77B8,0x294A,0x4A52,0x6B5A,0x7FFF
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000
	.size	showmovesPal, .-showmovesPal

@}}BLOCK(showmoves)
