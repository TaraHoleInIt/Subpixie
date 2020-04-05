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
    uint8_t c = ( Char >= _Font->FirstChar && Char <= _Font->LastChar ) ? Char - _Font->FirstChar : 0;
    return &_Font->Fontdata[ c * ( ( _Font->Width * _Font->Height ) / 8 ) ];
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
