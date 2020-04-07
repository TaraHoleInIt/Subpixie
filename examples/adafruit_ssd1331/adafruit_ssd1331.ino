// Copyright (c) 2020 Tara Keeling
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <subpixie.h>

static const int Pin_CS = -1;
static const int Pin_DC = PD3;
static const int Pin_RST = PD2;

Adafruit_SSD1331 LCD = Adafruit_SSD1331( &SPI, Pin_CS, Pin_DC, Pin_RST );

void Adafruit_SetAddressWindow( int x0, int y0, int x1, int y1 ) {
  LCD.setAddrWindow( x0, y0, ( x1 - x0 ), ( y1 - y0 ) );
}

void Adafruit_WritePixels( uint16_t* Buffer, size_t Count ) {
  LCD.writePixels( Buffer, Count, true, false );
}

Subpixie Sub = Subpixie( 96, 64, &Font_8x8, true, Adafruit_SetAddressWindow, Adafruit_WritePixels );

void setup( void ) {
  SPI.begin( );
  LCD.begin( );
}

void loop( void ) {
  LCD.fillScreen( 0 );
  LCD.setTextColor( 0xFFFF );

  LCD.println( "Adafruit 6x8" );
  Sub.println( );
  Sub.println( "Subpixie 8x8(W)" );

  Sub.SetFont( &Font_8x8, false );
  Sub.println( "Subpixie 8x8" );

  Sub.SetFont( &Font_6x8, true );
  Sub.println( "Subpixie 6x8(W)" );

  Sub.SetFont( &Font_6x8, false );
  Sub.println( "Subpixie 6x8" );

  while ( true ) {
  }
}
