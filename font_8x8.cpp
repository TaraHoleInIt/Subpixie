// Copyright (c) 2020 Tara Keeling
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include "subpixie.h"

static void Decode8x8_Wide( const uint8_t* GlyphPtr, uint16_t* Buffer, bool Inverse );
static void Decode8x8( const uint8_t* GlyphPtr, uint16_t* Buffer, bool Wide, bool Inverse );

static const uint8_t FontData_8x8[ ] FONT_DATA_IN_FLASH = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x00, 0x30, 0x00, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00, 0x10, 0x7c, 0xd2, 0x7c,
	0x86, 0x7c, 0x10, 0x00, 0xf0, 0x96, 0xfc, 0x18, 0x3e, 0x72, 0xde, 0x00,
	0x30, 0x48, 0x30, 0x78, 0xce, 0xcc, 0x78, 0x00, 0x0c, 0x0c, 0x18, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x60, 0xc0, 0xc0, 0xc0, 0x60, 0x10, 0x00,
	0x10, 0x0c, 0x06, 0x06, 0x06, 0x0c, 0x10, 0x00, 0x00, 0x54, 0x38, 0xfe,
	0x38, 0x54, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x70, 0x00, 0x00, 0x00, 0x7e,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
	0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x00, 0x7c, 0xce, 0xde, 0xf6,
	0xe6, 0xe6, 0x7c, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x3c, 0x00,
	0x7c, 0xc6, 0x06, 0x0c, 0x30, 0x60, 0xfe, 0x00, 0x7c, 0xc6, 0x06, 0x3c,
	0x06, 0xc6, 0x7c, 0x00, 0x0e, 0x1e, 0x36, 0x66, 0xfe, 0x06, 0x06, 0x00,
	0xfe, 0xc0, 0xc0, 0xfc, 0x06, 0x06, 0xfc, 0x00, 0x7c, 0xc6, 0xc0, 0xfc,
	0xc6, 0xc6, 0x7c, 0x00, 0xfe, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x60, 0x00,
	0x7c, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0x7c, 0x00, 0x7c, 0xc6, 0xc6, 0x7e,
	0x06, 0xc6, 0x7c, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00,
	0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x20, 0x00, 0x00, 0x1c, 0x30, 0x60,
	0x30, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00,
	0x00, 0x70, 0x18, 0x0c, 0x18, 0x70, 0x00, 0x00, 0x7c, 0xc6, 0x0c, 0x18,
	0x30, 0x00, 0x30, 0x00, 0x7c, 0x82, 0x9a, 0xaa, 0xaa, 0x9e, 0x7c, 0x00,
	0x7c, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00, 0xfc, 0x66, 0x66, 0x7c,
	0x66, 0x66, 0xfc, 0x00, 0x7c, 0xc6, 0xc0, 0xc0, 0xc0, 0xc6, 0x7c, 0x00,
	0xfc, 0x66, 0x66, 0x66, 0x66, 0x66, 0xfc, 0x00, 0xfe, 0x62, 0x68, 0x78,
	0x68, 0x62, 0xfe, 0x00, 0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00,
	0x7c, 0xc6, 0xc6, 0xc0, 0xde, 0xc6, 0x7c, 0x00, 0xc6, 0xc6, 0xc6, 0xfe,
	0xc6, 0xc6, 0xc6, 0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00,
	0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0xcc, 0x78, 0x00, 0xc6, 0xcc, 0xd8, 0xf0,
	0xd8, 0xcc, 0xc6, 0x00, 0xf0, 0x60, 0x60, 0x60, 0x60, 0x62, 0xfe, 0x00,
	0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0xc6, 0x00, 0xc6, 0xe6, 0xf6, 0xde,
	0xce, 0xc6, 0xc6, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00,
	0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00, 0x7c, 0xc6, 0xc6, 0xc6,
	0xc6, 0xc6, 0x7c, 0x0c, 0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xe6, 0x00,
	0x7c, 0xc6, 0xc0, 0x7c, 0x06, 0xc6, 0x7c, 0x00, 0x7e, 0x5a, 0x18, 0x18,
	0x18, 0x18, 0x3c, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00,
	0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00, 0xc6, 0xc6, 0xc6, 0xc6,
	0xd6, 0xee, 0xc6, 0x00, 0xc6, 0x6c, 0x38, 0x38, 0x38, 0x6c, 0xc6, 0x00,
	0x66, 0x66, 0x66, 0x3c, 0x18, 0x18, 0x3c, 0x00, 0xfe, 0xc6, 0x0c, 0x18,
	0x30, 0x66, 0xfe, 0x00, 0x1c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1c, 0x00,
	0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x02, 0x00, 0x70, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x70, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x30, 0x30, 0x18, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x06, 0x7e, 0xc6, 0x7e, 0x00,
	0xc0, 0xc0, 0xfc, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x7c, 0xc6,
	0xc0, 0xc6, 0x7c, 0x00, 0x06, 0x06, 0x7e, 0xc6, 0xc6, 0xc6, 0x7e, 0x00,
	0x00, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00, 0x3c, 0x66, 0x60, 0xf0,
	0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x7e, 0xc6, 0xc6, 0x7e, 0x06, 0x7c,
	0xc0, 0xc0, 0xfc, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x18, 0x00, 0x38, 0x18,
	0x18, 0x18, 0x3c, 0x00, 0x00, 0x0c, 0x00, 0x1c, 0x0c, 0x0c, 0xcc, 0x78,
	0xc0, 0xc0, 0xc6, 0xd8, 0xf0, 0xd8, 0xc6, 0x00, 0x38, 0x18, 0x18, 0x18,
	0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0x00,
	0x00, 0x00, 0xfc, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x7c, 0xc6,
	0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0xfc, 0xc6, 0xc6, 0xfc, 0xc0, 0xc0,
	0x00, 0x00, 0x7e, 0xc6, 0xc6, 0x7e, 0x06, 0x06, 0x00, 0x00, 0xde, 0x76,
	0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x7c, 0xc0, 0x7c, 0x06, 0x7c, 0x00,
	0x18, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x1e, 0x00, 0x00, 0x00, 0xc6, 0xc6,
	0xc6, 0xc6, 0x7e, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00,
	0x00, 0x00, 0xc6, 0xc6, 0xd6, 0xfe, 0xc6, 0x00, 0x00, 0x00, 0xc6, 0x6c,
	0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x7c,
	0x00, 0x00, 0xfe, 0x0c, 0x18, 0x60, 0xfe, 0x00, 0x0e, 0x18, 0x18, 0x70,
	0x18, 0x18, 0x0e, 0x00, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,
	0xe0, 0x30, 0x30, 0x1c, 0x30, 0x30, 0xe0, 0x00, 0x00, 0x00, 0x70, 0x9a,
	0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x66, 0xff, 0x00, 0x00,
	0x7c, 0xc6, 0xc0, 0xc0, 0xc6, 0x7c, 0x18, 0x70, 0x66, 0x00, 0xc6, 0xc6,
	0xc6, 0xc6, 0x7e, 0x00, 0x0e, 0x18, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00,
	0x18, 0x24, 0x7c, 0x06, 0x7e, 0xc6, 0x7e, 0x00, 0x66, 0x00, 0x7c, 0x06,
	0x7e, 0xc6, 0x7e, 0x00, 0x38, 0x0c, 0x7c, 0x06, 0x7e, 0xc6, 0x7e, 0x00,
	0x18, 0x00, 0x7c, 0x06, 0x7e, 0xc6, 0x7e, 0x00, 0x00, 0x00, 0x7c, 0xc0,
	0xc0, 0x7c, 0x18, 0x70, 0x18, 0x24, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00,
	0x66, 0x00, 0x7c, 0xc6, 0xfe, 0xc0, 0x7c, 0x00, 0x70, 0x18, 0x7c, 0xc6,
	0xfe, 0xc0, 0x7c, 0x00, 0x66, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3c, 0x00,
	0x18, 0x24, 0x38, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x38, 0x0c, 0x38, 0x18,
	0x18, 0x18, 0x3c, 0x00, 0x66, 0x00, 0x7c, 0xc6, 0xfe, 0xc6, 0xc6, 0x00,
	0x18, 0x00, 0x7c, 0xc6, 0xfe, 0xc6, 0xc6, 0x00, 0x0e, 0x18, 0xfe, 0x60,
	0x78, 0x60, 0xfe, 0x00, 0x00, 0x00, 0x7c, 0x1a, 0x7e, 0xd8, 0x7e, 0x00,
	0x7e, 0xd8, 0xd8, 0xde, 0xf8, 0xd8, 0xde, 0x00, 0x18, 0x24, 0x7c, 0xc6,
	0xc6, 0xc6, 0x7c, 0x00, 0x66, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00,
	0x38, 0x0c, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x18, 0x24, 0xc6, 0xc6,
	0xc6, 0xc6, 0x7e, 0x00, 0x38, 0x0c, 0xc6, 0xc6, 0xc6, 0xc6, 0x7e, 0x00,
	0x66, 0x00, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x7c, 0x66, 0x7c, 0xc6, 0xc6,
	0xc6, 0xc6, 0x7c, 0x00, 0xc6, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00,
	0x18, 0x7c, 0xc6, 0xc0, 0xc6, 0x7c, 0x18, 0x00, 0x1e, 0x32, 0x30, 0x78,
	0x30, 0x70, 0xfe, 0x00, 0x66, 0x3c, 0x18, 0x7e, 0x18, 0x3c, 0x18, 0x00,
	0xfc, 0xc6, 0xfc, 0xc0, 0xcc, 0xde, 0xcc, 0x0e, 0x00, 0x1c, 0x32, 0x30,
	0xfc, 0x30, 0xf0, 0x00
};

const Subpixie_Fontdef Font_8x8 = {
	FontData_8x8,
	8,
	8,
	0x20,
	0x7F,
	Decode8x8
};

static void Decode8x8_Wide( const uint8_t* GlyphPtr, uint16_t* Buffer, bool Inverse ) {
	uint8_t Data = 0;
	int y = 0;

	for ( y = 0; y < Font_8x8.Height; y++ ) {
		Data = Subpixie_Font_Get_Byte( GlyphPtr++ );
		Data = ( Inverse == true ) ? ~Data : Data;
		
		*Buffer++ = SubpxDecodeTable_Wide[ ( Data >> 5 ) & 0x07 ][ 0 ];
		*Buffer++ = SubpxDecodeTable_Wide[ ( Data >> 5 ) & 0x07 ][ 1 ];

		*Buffer++ = SubpxDecodeTable_Wide[ ( Data >> 2 ) & 0x07 ][ 0 ];
		*Buffer++ = SubpxDecodeTable_Wide[ ( Data >> 2 ) & 0x07 ][ 1 ];

		*Buffer++ = SubpxDecodeTable_Wide[ ( ( ( Data & 0x03 ) ) << 1 ) | Inverse ][ 0 ];
		*Buffer++ = SubpxDecodeTable_Wide[ ( ( ( Data & 0x03 ) ) << 1 ) | Inverse ][ 1 ];
	}
}

static void Decode8x8( const uint8_t* GlyphPtr, uint16_t* Buffer, bool Wide, bool Inverse ) {
	uint8_t Data = 0;
	int y = 0;

	if ( Wide == false ) {
		for ( y = 0; y < Font_8x8.Height; y++ ) {
			Data = Subpixie_Font_Get_Byte( GlyphPtr++ );
			Data = ( Inverse == true ) ? ~Data : Data;

			// ABCDEFGH
			// RGBRGBRGB

			*Buffer++ = SubpxDecodeTable[ ( Data >> 5 ) & 0x07 ];
			*Buffer++ = SubpxDecodeTable[ ( Data >> 2 ) & 0x07 ];
			*Buffer++ = SubpxDecodeTable[ ( ( Data & 0x03 ) << 1 ) | Inverse ];
		}

		return;
	}

	Decode8x8_Wide( GlyphPtr, Buffer, Inverse );
}
