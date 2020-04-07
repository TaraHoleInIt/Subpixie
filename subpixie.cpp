// Copyright (c) 2020 Tara Keeling
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include <Arduino.h>
#include "subpixie.h"

Subpixie::Subpixie( int DisplayWidth, int DisplayHeight, const Subpixie_Fontdef* Font, bool Wide, SetAddressWindowProc SetAddressWindow, WritePixelsProc WritePixels ) {
    _DisplayWidth = DisplayWidth;
    _DisplayHeight = DisplayHeight;

    _SetAddressWindow = SetAddressWindow;
    _WritePixels = WritePixels;

    _Font = Font;
    _Wide = Wide;

    CachedGlyph = 0;
    PrintX = 0;
    PrintY = 0;

    SetFont( Font, Wide );
    DecodeGlyphSubpixel( 0 );
}

void Subpixie::SetFont( const Subpixie_Fontdef* Font, bool Wide ) {
    _Wide = Wide;
    _Font = Font;

    GlyphWidth = ( Wide == true ) ? ( ( Font->Width + 2 ) / 3 ) * 2 : ( Font->Width + 2 ) / 3;
    GlyphHeight = Font->Height;
    GlyphSize = GlyphWidth * GlyphHeight * 2;
}

const uint8_t* Subpixie::GetGlyphPtr( const uint8_t Char ) {
    const uint8_t* Ptr = _Font->Fontdata;
    uint8_t RoundedWidth = 0;
    char c = 0;

    // Make sure the glyph exists in the font
    // if not then it will just return a pointer to the first glyph
    c = ( Char >= _Font->FirstChar && Char <= _Font->LastChar ) ? Char - _Font->FirstChar : 0;

    // Font widths must be rounded up to a multiple of 8
    RoundedWidth = ( _Font->Width + 7 ) / 8;

    // Return a pointer to glyph data
    return Ptr + ( ( c * RoundedWidth * _Font->Height ) );
}

void Subpixie::DecodeGlyphSubpixel( const uint8_t Char ) {
    if ( Char != CachedGlyph ) {
        _Font->Decode( GetGlyphPtr( Char ), GlyphCache, _Wide );
        CachedGlyph = Char;
    }
}

void Subpixie::DrawGlyph( int x, int y ) {
    _SetAddressWindow( x, y, x + GlyphWidth, y + GlyphHeight );
    _WritePixels( GlyphCache, GlyphSize / 2 );
}

size_t Subpixie::write( const uint8_t Data ) {
    if ( ( PrintX + GlyphWidth ) >= _DisplayWidth ) {
        PrintY+= GlyphHeight;
        PrintX = 0;
    }

    if ( PrintY >= _DisplayHeight ) {
        PrintY = 0;
        PrintX = 0;
    }

    switch ( Data ) {
        case '\r': {
            PrintX = 0;
            break;
        }
        case '\n': {
            PrintY+= GlyphHeight;
            PrintX = 0;

            break;
        }
        default: {
            DecodeGlyphSubpixel( Data );
            DrawGlyph( PrintX, PrintY );

            PrintX+= GlyphWidth;
            break;
        }
    }

    return 1;
}

void Subpixie::GetPrintCoords( int& x, int& y ) {
    x = PrintX;
    y = PrintY;
}

void Subpixie::SetPrintCoords( int x, int y ) {
    PrintX = x;
    PrintY = y;
}

const uint16_t Subpixie::SubpxDecodeTable_Wide[ 8 ][ 2 ] = {
	{ RGB565( 0, 0, 0 ), RGB565( 0, 0, 0 ) }, // 000
	{ RGB565( 0, 0, 0 ), RGB565( 0, 63, 31 ) }, // 001
	{ RGB565( 0, 0, 31 ), RGB565( 31, 0, 0 ) }, // 010
	{ RGB565( 0, 0, 31 ), RGB565( 31, 63, 31 ) }, // 011
	{ RGB565( 31, 63, 0 ), RGB565( 0, 0, 0 ) }, // 100
	{ RGB565( 31, 63, 0 ), RGB565( 0, 63, 31 ) }, // 101
	{ RGB565( 31, 63, 31 ), RGB565( 31, 0, 0 ) }, // 110
	{ RGB565( 31, 63, 31 ), RGB565( 31, 63, 31 ) } // 111
};

const uint16_t Subpixie::SubpxDecodeTable[ 8 ] = {
	RGB565( 0, 0, 0 ),	// 000
	RGB565( 0, 0, 31 ),	// 001
	RGB565( 0, 63, 0 ),	// 010
	RGB565( 0, 63, 31 ),	// 011
	RGB565( 31, 0, 0 ),	// 100
	RGB565( 31, 0, 31 ),	// 101
	RGB565( 31, 63, 0 ),	// 110
	RGB565( 31, 63, 31 )	// 111
};
