// Copyright (c) 2020 Tara Keeling
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include <Arduino.h>
#include <M5Stack.h>
#include <subpixie.h>

void M5SetAddressWindow( int x0, int y0, int x1, int y1 ) {
	M5.Lcd.setAddrWindow( x0, y0, x1 - x0, y1 - y0 );
}

void M5WritePixels( uint16_t* Pixels, size_t Count ) {
	M5.Lcd.pushColors( ( uint16_t* ) Pixels, ( uint32_t ) Count, false );
}

Subpixie Sub = Subpixie( 320, 240, &Font_8x8, true, M5SetAddressWindow, M5WritePixels );

void setup( void ) {
	M5.begin( );
	M5.Power.begin( );
}

void loop( void ) {
    M5.lcd.fillScreen( TFT_BLACK );
    
    Sub.println( "Hello world!" );
    Sub.println( "Hi!" );
    Sub.print( 0xC0, 16 );
    Sub.println( 0xDE, 16 );

    while ( true ) {
    }
}