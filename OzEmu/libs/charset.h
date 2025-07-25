const uint8_t charset[][5] = {
  { 0x00, 0x00, 0x00, 0x00, 0x00 },  // 00 zero
  { 0x1c, 0x2a, 0x32, 0x2a, 0x1c },  // 01 smile
  { 0xe3, 0xd5, 0xcd, 0xd5, 0xe3 },  // 02 negsmile
  { 0x1c, 0x3e, 0x7c, 0x3e, 0x1c },  // 03 heart
  { 0x18, 0x3c, 0x7e, 0x3c, 0x18 },  // 04 бубен
  { 0x38, 0x94, 0xfe, 0x94, 0x38 },  // 05 крест
  { 0x38, 0x9c, 0xfe, 0x9c, 0x38 },  // 06 пик
  { 0x00, 0x1f, 0x1f, 0x00, 0x00 },  // 07 mid point
  { 0xff, 0xe0, 0xe0, 0xff, 0xff },  // 08 rew point
  { 0x1f, 0x24, 0x42, 0x24, 0x1f },  // 09 circle
  { 0xe0, 0xdb, 0xbd, 0xdb, 0xe0 },  // 0a rew circle
  { 0x60, 0x90, 0x9a, 0x66, 0x0e },  // 0b man
  { 0x0c, 0x52, 0xf2, 0x52, 0x0c },  // 0c wom
  { 0xe0, 0xe0, 0x7f, 0x03, 0x06 },  // 0d note
  { 0xc0, 0xfe, 0x03, 0x61, 0x7f },  // 0e dnote
  { 0x2a, 0x1c, 0x36, 0x1c, 0x2a },  // 0f sun
  { 0xfe, 0x7c, 0x38, 0x10, 0x00 },  // 10 rtriangle
  { 0x00, 0x10, 0x38, 0x7c, 0xfe },  // 11 ltriangle
  { 0x00, 0x44, 0xfe, 0x44, 0x00 },  // 12 varrow
  { 0x00, 0xbe, 0x00, 0xbe, 0x00 },  // 13 !!
  { 0x18, 0xfe, 0x02, 0xfe, 0x02 },  // 14 enter sign
  { 0x00, 0xb8, 0xee, 0x3a, 0x00 },  // 15 paragraph
  { 0x18, 0x18, 0x18, 0x18, 0x18 },  // 16 fat stick
  { 0x00, 0x44, 0xfe, 0x44, 0x00 },  // 17 varrow
  { 0x00, 0x04, 0xfe, 0x04, 0x00 },  // 18 uarrow
  { 0x00, 0x40, 0xfe, 0x40, 0x00 },  // 19 darrow
  { 0x10, 0x10, 0x10, 0x38, 0x10 },  // 1a rarrow
  { 0x10, 0x38, 0x10, 0x10, 0x10 },  // 1b larrow
  { 0x7c, 0x40, 0x40, 0x40, 0x40 },  // 1c angle
  { 0x10, 0x38, 0x10, 0x38, 0x10 },  // 1d harrow
  { 0x20, 0x30, 0x38, 0x30, 0x20 },  // utriangle
  { 0x04, 0x0c, 0x1c, 0x0c, 0x04 },  // dtriangle
  { 0x00, 0x00, 0x00, 0x00, 0x00 },  // 20 space
  { 0x00, 0x00, 0x5f, 0x00, 0x00 },  // 21 !
  { 0x00, 0x07, 0x00, 0x07, 0x00 },  // 22 "
  { 0x14, 0x7f, 0x14, 0x7f, 0x14 },  // 23 #
  { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },  // 24 $
  { 0x23, 0x13, 0x08, 0x64, 0x62 },  // 25 %
  { 0x36, 0x49, 0x55, 0x22, 0x50 },  // 26 &
  { 0x00, 0x05, 0x03, 0x00, 0x00 },  // 27 '
  { 0x00, 0x1c, 0x22, 0x41, 0x00 },  // 28 (
  { 0x00, 0x41, 0x22, 0x1c, 0x00 },  // 29 )
  { 0x14, 0x08, 0x3e, 0x08, 0x14 },  // 2a *
  { 0x08, 0x08, 0x3e, 0x08, 0x08 },  // 2b +
  { 0x00, 0x50, 0x30, 0x00, 0x00 },  // 2c ,
  { 0x08, 0x08, 0x08, 0x08, 0x08 },  // 2d -
  { 0x00, 0x60, 0x60, 0x00, 0x00 },  // 2e .
  { 0x20, 0x10, 0x08, 0x04, 0x02 },  // 2f /
  { 0x3e, 0x51, 0x49, 0x45, 0x3e },  // 30 0
  { 0x00, 0x42, 0x7f, 0x40, 0x00 },  // 31 1
  { 0x42, 0x61, 0x51, 0x49, 0x46 },  // 32 2
  { 0x21, 0x41, 0x45, 0x4b, 0x31 },  // 33 3
  { 0x18, 0x14, 0x12, 0x7f, 0x10 },  // 34 4
  { 0x27, 0x45, 0x45, 0x45, 0x39 },  // 35 5
  { 0x3c, 0x4a, 0x49, 0x49, 0x30 },  // 36 6
  { 0x01, 0x71, 0x09, 0x05, 0x03 },  // 37 7
  { 0x36, 0x49, 0x49, 0x49, 0x36 },  // 38 8
  { 0x06, 0x49, 0x49, 0x29, 0x1e },  // 39 9
  { 0x00, 0x36, 0x36, 0x00, 0x00 },  // 3a :
  { 0x00, 0x56, 0x36, 0x00, 0x00 },  // 3b ;
  { 0x08, 0x14, 0x22, 0x41, 0x00 },  // 3c <
  { 0x14, 0x14, 0x14, 0x14, 0x14 },  // 3d =
  { 0x00, 0x41, 0x22, 0x14, 0x08 },  // 3e >
  { 0x02, 0x01, 0x51, 0x09, 0x06 },  // 3f ?
  { 0x32, 0x49, 0x79, 0x41, 0x3e },  // 40 @
  { 0x7e, 0x11, 0x11, 0x11, 0x7e },  // 41 A
  { 0x7f, 0x49, 0x49, 0x49, 0x36 },  // 42 B
  { 0x3e, 0x41, 0x41, 0x41, 0x22 },  // 43 C
  { 0x7f, 0x41, 0x41, 0x22, 0x1c },  // 44 D
  { 0x7f, 0x49, 0x49, 0x49, 0x41 },  // 45 E
  { 0x7f, 0x09, 0x09, 0x09, 0x01 },  // 46 F
  { 0x3e, 0x41, 0x49, 0x49, 0x7a },  // 47 G
  { 0x7f, 0x08, 0x08, 0x08, 0x7f },  // 48 H
  { 0x00, 0x41, 0x7f, 0x41, 0x00 },  // 49 I
  { 0x20, 0x40, 0x41, 0x3f, 0x01 },  // 4a J
  { 0x7f, 0x08, 0x14, 0x22, 0x41 },  // 4b K
  { 0x7f, 0x40, 0x40, 0x40, 0x40 },  // 4c L
  { 0x7f, 0x02, 0x0c, 0x02, 0x7f },  // 4d M
  { 0x7f, 0x04, 0x08, 0x10, 0x7f },  // 4e N
  { 0x3e, 0x41, 0x41, 0x41, 0x3e },  // 4f O
  { 0x7f, 0x09, 0x09, 0x09, 0x06 },  // 50 P
  { 0x3e, 0x41, 0x51, 0x21, 0x5e },  // 51 Q
  { 0x7f, 0x09, 0x19, 0x29, 0x46 },  // 52 R
  { 0x46, 0x49, 0x49, 0x49, 0x31 },  // 53 S
  { 0x01, 0x01, 0x7f, 0x01, 0x01 },  // 54 T
  { 0x3f, 0x40, 0x40, 0x40, 0x3f },  // 55 U
  { 0x1f, 0x20, 0x40, 0x20, 0x1f },  // 56 V
  { 0x3f, 0x40, 0x38, 0x40, 0x3f },  // 57 W
  { 0x63, 0x14, 0x08, 0x14, 0x63 },  // 58 X
  { 0x07, 0x08, 0x70, 0x08, 0x07 },  // 59 Y
  { 0x61, 0x51, 0x49, 0x45, 0x43 },  // 5a Z
  { 0x00, 0x7f, 0x41, 0x41, 0x00 },  // 5b [
  { 0x02, 0x04, 0x08, 0x10, 0x20 },  // 5c backslash
  { 0x00, 0x41, 0x41, 0x7f, 0x00 },  // 5d ]
  { 0x04, 0x02, 0x01, 0x02, 0x04 },  // 5e ^
  { 0x40, 0x40, 0x40, 0x40, 0x40 },  // 5f _
  { 0x00, 0x01, 0x02, 0x04, 0x00 },  // 60 `
  { 0x20, 0x54, 0x54, 0x54, 0x78 },  // 61 a
  { 0x7f, 0x48, 0x44, 0x44, 0x38 },  // 62 b
  { 0x38, 0x44, 0x44, 0x44, 0x20 },  // 63 c
  { 0x38, 0x44, 0x44, 0x48, 0x7f },  // 64 d
  { 0x38, 0x54, 0x54, 0x54, 0x18 },  // 65 e
  { 0x08, 0x7e, 0x09, 0x01, 0x02 },  // 66 f
  { 0x0c, 0x52, 0x52, 0x52, 0x3e },  // 67 g
  { 0x7f, 0x08, 0x04, 0x04, 0x78 },  // 68 h
  { 0x00, 0x44, 0x7d, 0x40, 0x00 },  // 69 i
  { 0x20, 0x40, 0x44, 0x3d, 0x00 },  // 6a j
  { 0x7f, 0x10, 0x28, 0x44, 0x00 },  // 6b k
  { 0x00, 0x41, 0x7f, 0x40, 0x00 },  // 6c l
  { 0x7c, 0x04, 0x18, 0x04, 0x78 },  // 6d m
  { 0x7c, 0x08, 0x04, 0x04, 0x78 },  // 6e n
  { 0x38, 0x44, 0x44, 0x44, 0x38 },  // 6f o
  { 0x7c, 0x14, 0x14, 0x14, 0x08 },  // 70 p
  { 0x08, 0x14, 0x14, 0x18, 0x7c },  // 71 q
  { 0x7c, 0x08, 0x04, 0x04, 0x08 },  // 72 r
  { 0x48, 0x54, 0x54, 0x54, 0x20 },  // 73 s
  { 0x04, 0x3f, 0x44, 0x40, 0x20 },  // 74 t
  { 0x3c, 0x40, 0x40, 0x20, 0x7c },  // 75 u
  { 0x1c, 0x20, 0x40, 0x20, 0x1c },  // 76 v
  { 0x3c, 0x40, 0x30, 0x40, 0x3c },  // 77 w
  { 0x44, 0x28, 0x10, 0x28, 0x44 },  // 78 x
  { 0x0c, 0x50, 0x50, 0x50, 0x3c },  // 79 y
  { 0x44, 0x64, 0x54, 0x4c, 0x44 },  // 7a z
  { 0x00, 0x08, 0x36, 0x41, 0x00 },  // 7b {
  { 0x00, 0x00, 0x7f, 0x00, 0x00 },  // 7c |
  { 0x00, 0x41, 0x36, 0x08, 0x00 },  // 7d }
  { 0x10, 0x08, 0x08, 0x10, 0x08 },  // 7e ~
  { 0x00, 0x00, 0x00, 0x00, 0x00 },  // 7f
  { 0x7e, 0x11, 0x11, 0x11, 0x7e },  //A	0x80	
  { 0x7f, 0x49, 0x49, 0x49, 0x33 },  //Б	0x81	
  { 0x7f, 0x49, 0x49, 0x49, 0x36 },  //В	0x82	
  { 0x7f, 0x01, 0x01, 0x01, 0x03 },  //Г	0x83	
  { 0xe0, 0x51, 0x4f, 0x41, 0xff },  //Д	0x84	
  { 0x7f, 0x49, 0x49, 0x49, 0x41 },  //E	0x85	
  { 0x77, 0x08, 0x7f, 0x08, 0x77 },  //Ж	0x86	
  { 0x41, 0x49, 0x49, 0x49, 0x36 },  //З	0x87	
  { 0x7f, 0x10, 0x08, 0x04, 0x7f },  //И	0x88	
  { 0x7c, 0x21, 0x12, 0x09, 0x7c },  //Й	0x89	
  { 0x7f, 0x08, 0x14, 0x22, 0x41 },  //K	0x8A	
  { 0x20, 0x41, 0x3f, 0x01, 0x7f },  //Л	0x8B	
  { 0x7f, 0x02, 0x0c, 0x02, 0x7f },  //M	0x8C	
  { 0x7f, 0x08, 0x08, 0x08, 0x7f },  //H	0x8D	
  { 0x3e, 0x41, 0x41, 0x41, 0x3e },  //O	0x8E	
  { 0x7f, 0x01, 0x01, 0x01, 0x7f },  //П	0x8F	
  { 0x7f, 0x09, 0x09, 0x09, 0x06 },  //P	0x90	
  { 0x3e, 0x41, 0x41, 0x41, 0x22 },  //C	0x91
  { 0x01, 0x01, 0x7f, 0x01, 0x01 },  //T	0x92
  { 0x47, 0x28, 0x10, 0x08, 0x07 },  //У	0x93
  { 0x1c, 0x22, 0x7f, 0x22, 0x1c },  //Ф	0x94
  { 0x63, 0x14, 0x08, 0x14, 0x63 },  //X	0x95
  { 0x7f, 0x40, 0x40, 0x40, 0xff },  //Ц	0x96
  { 0x07, 0x08, 0x08, 0x08, 0x7f },  //Ч	0x97
  { 0x7f, 0x40, 0x7f, 0x40, 0x7f },  //Ш	0x98
  { 0x7f, 0x40, 0x7f, 0x40, 0xff },  //Щ	0x99
  { 0x01, 0x7f, 0x48, 0x48, 0x30 },  //Ъ	0x9A
  { 0x7f, 0x48, 0x30, 0x00, 0x7f },  //Ы	0x9B
  { 0x00, 0x7f, 0x48, 0x48, 0x30 },  //Э	0x9C
  { 0x22, 0x41, 0x49, 0x49, 0x3e },  //Ь	0x9D
  { 0x7f, 0x08, 0x3e, 0x41, 0x3e },  //Ю	0x9E
  { 0x46, 0x29, 0x19, 0x09, 0x7f },  //Я	0x9F
  { 0x20, 0x54, 0x54, 0x54, 0x78 },  //a	0xA0
  { 0x3c, 0x4a, 0x4a, 0x49, 0x31 },  //б	0xA1
  { 0x7c, 0x54, 0x54, 0x28, 0x00 },  //в	0xA2
  { 0x7c, 0x04, 0x04, 0x04, 0x0c },  //г	0xA3
  { 0xe0, 0x54, 0x4c, 0x44, 0xfc },  //д	0xA4
  { 0x38, 0x54, 0x54, 0x54, 0x18 },  //e	0xA5
  { 0x6c, 0x10, 0x7c, 0x10, 0x6c },  //ж	0xA6
  { 0x44, 0x44, 0x54, 0x54, 0x28 },  //з	0xA7
  { 0x7c, 0x20, 0x10, 0x08, 0x7c },  //и	0xA8
  { 0x7c, 0x41, 0x22, 0x11, 0x7c },  //й	0xA9
  { 0x7c, 0x10, 0x28, 0x44, 0x00 },  //к	0xAA
  { 0x20, 0x44, 0x3c, 0x04, 0x7c },  //л	0xAB
  { 0x7c, 0x08, 0x10, 0x08, 0x7c },  //м	0xAC
  { 0x7c, 0x10, 0x10, 0x10, 0x7c },  //н	0xAD
  { 0x38, 0x44, 0x44, 0x44, 0x38 },  //o	0xAE
  { 0x7c, 0x04, 0x04, 0x04, 0x7c },  //п	0xAF
////////////////////////////////////////////////
  { 0x7C, 0x14, 0x14, 0x14, 0x08 },  //p 	0xE0
  { 0x38, 0x44, 0x44, 0x44, 0x20 },  //c	0xE1
  { 0x04, 0x04, 0x7c, 0x04, 0x04 },  //т	0xE2
  { 0x0C, 0x50, 0x50, 0x50, 0x3C },  //у	0xE3
  { 0x30, 0x48, 0xfc, 0x48, 0x30 },  //ф	0xE4
  { 0x44, 0x28, 0x10, 0x28, 0x44 },  //x	0xE5
  { 0x7c, 0x40, 0x40, 0x40, 0xfc },  //ц	0xE6
  { 0x0c, 0x10, 0x10, 0x10, 0x7c },  //ч	0xE7
  { 0x7c, 0x40, 0x7c, 0x40, 0x7c },  //ш	0xE8
  { 0x7c, 0x40, 0x7c, 0x40, 0xfc },  //щ	0xE9
  { 0x04, 0x7c, 0x50, 0x50, 0x20 },  //ъ	0xEA
  { 0x7c, 0x50, 0x50, 0x20, 0x7c },  //ы	0xEB
  { 0x7c, 0x50, 0x50, 0x20, 0x00 },  //ь	0xEC
  { 0x28, 0x44, 0x54, 0x54, 0x38 },  //э	0xED
  { 0x7c, 0x10, 0x38, 0x44, 0x38 },  //ю	0xEE
  { 0x08, 0x54, 0x34, 0x14, 0x7c }   //я	0xEF
};
