#ifndef _SUBPIXIE_H_
#define _SUBPIXIE_H_

#include <Arduino.h>

#define RGB565( r, g, b ) ( uint16_t ) ( ( r << 11 ) | ( g << 5 ) | ( b ) )

#if defined __AVR__
#include <avr/pgmspace.h>
#define FONT_DATA_IN_FLASH PROGMEM

#define Subpixie_Font_Get_Byte( Ptr ) pgm_read_byte( Ptr )
#else
#define FONT_DATA_IN_FLASH

#define Subpixie_Font_Get_Byte( Ptr ) *Ptr
#endif

/**
 * @brief Procedure to set the render window of the target LCD
 * 
 * @param x0 Starting x coordinate
 * @param y0 Starting y coordinate
 * @param x1 Ending x coordinate
 * @param y1 Ending y coordinate
 */
typedef void ( *SetAddressWindowProc ) ( int x0, int y0, int x1, int y1 );

/**
 * @brief Procedure to write pixels to the target LCD
 * 
 * @param Pixels Pointer to 16bit pixels to draw
 * @param Count Number of 16bit words to write to the LCD
 */
typedef void ( *WritePixelsProc ) ( uint16_t* Pixels, size_t Count );

/**
 * @brief Decodes the given 1bpp linear font data into subpixels
 * 
 * @param GlyphPtr Pointer to font data
 * @param Buffer Pointer to buffer which will receive decoded data
 * @param Wide Render twice as wide
 */
typedef void ( *SubpixelDecodeProc ) ( const uint8_t* GlyphPtr, uint16_t* Buffer, bool Wide );

typedef struct {
    const uint8_t* Fontdata;
    int Width;
    int Height;
    int FirstChar;
    int LastChar;
    SubpixelDecodeProc Decode;
} Subpixie_Fontdef;

class Subpixie : public Print {
private:
    uint16_t GlyphCache[ 6 * 8 ]; /**< Space for decoded glyph data */

    SetAddressWindowProc _SetAddressWindow; /**< Callback to function to set the render window of the target LCD */
    WritePixelsProc _WritePixels; /**< Callback to function to write pixels to the target LCD */

    const Subpixie_Fontdef* _Font; /**< Currently selected font */

    uint8_t CachedGlyph; /**< Value of last decoded character */

    int _DisplayWidth = 0; /**< Width of target display */
    int _DisplayHeight = 0; /**< Height of target display */

    int GlyphSize = 0; /**< Size in bytes of decoded character glyphs */
    int GlyphWidth = 0; /**< Width in pixels of decoded subpixel font */
    int GlyphHeight = 0; /**< Height of characters in font */

    bool _Wide; /**< If true, render fonts twice as wide */

    int PrintX; /**< Current x coordinate for drawing characters */
    int PrintY; /**< Current y coordinate for drawing characters */

    /**
     * @brief Returns a pointer to the 1bpp font data for the given character
     * 
     * @param Char Character to get the data for
     * @return const uint8_t* Pointer to font data
     */
    const uint8_t* GetGlyphPtr( const uint8_t Char );

    /**
     * @brief Decodes 1bpp linear font data into subpixels
     * 
     * @param Char Character to decode
     */
    void DecodeGlyphSubpixel( const uint8_t Char );

    /**
     * @brief Draws the currently cached/decoded glyph at the given coordinates
     * 
     * @param x x
     * @param y y
     */
    void DrawGlyph( int x, int y );

public:
    static const uint16_t SubpxDecodeTable_Wide[ 8 ][ 2 ];
    static const uint16_t SubpxDecodeTable[ 8 ];

    /** 
     * Creates a new Subpixie renderer with the given parameters.
     * @param DisplayWidth Width of LCD screen
     * @param DisplayHeight Height of LCD screen
     * @param Font Pointer to Subpixie font to use
     * @param Wide Render subpixel fonts twice as wide (increases readability, size)
     * @param DisplaySetAddressWindow Pointer to a function to set the drawing window of the LCD you're rendering to
     * @param WritePixels Pointer to a function which copies data to the LCD you're rendering to
     */
    Subpixie( int DisplayWidth, int DisplayHeight, const Subpixie_Fontdef* Font, bool Wide, SetAddressWindowProc DisplaySetAddressWindow, WritePixelsProc WritePixels );

    /**
     * @brief Select which Subpixie font to draw with
     * 
     * @param Font Subpixie font to use
     * @param Wide Render characters with twice the width (increases readibility, size)
     */
    void SetFont( const Subpixie_Fontdef* Font, bool Wide );

    /**
     * @brief Writes a character at the current print coordinates
     * 
     * @param Data Character to write
     * @return size_t Number of characters written
     */
    virtual size_t write( const uint8_t Data );

    /**
     * @brief Gives the caller the current x and y coordinates of the print cursor
     * 
     * @param x X coordinate
     * @param y Y coordinate
     */
    void GetPrintCoords( int& x, int& y );

    /**
     * @brief Set the coordinates of the print cursor
     * 
     * @param x New x coordinate
     * @param y New y coordinate
     */
    void SetPrintCoords( int x, int y );
};

extern const Subpixie_Fontdef Font_8x8;
extern const Subpixie_Fontdef Font_6x8;

#endif
