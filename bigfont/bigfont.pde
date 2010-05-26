/*
Writen 2010 Adam Thomas
This example is public domain
*/
#include <ht1632.h>
#include <MHV_io_ArduinoDuemilanove328p.h>
#include <avr/pgmspace.h>
#include "c64d_font.h"

HT1632 matrix[4] = {
  HT1632( MHV_ARDUINO_PIN_4,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons ),
  HT1632( MHV_ARDUINO_PIN_5,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons ),
  HT1632( MHV_ARDUINO_PIN_6,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons ),
  HT1632( MHV_ARDUINO_PIN_7,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons )
};

void setup ()
{ 
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
}

const char *string = "MakeHackVoid";
uint8_t offset = 0;

void loop ()
{
  uint8_t data[32], bitmap[32];
  
  for(int i = 0; i < 4; ++i ) {
    memcpy_P( data, &typeface[32*(string[i+(offset*4)]-32)], 32 );
    for(int i=0;i < 16; i++) {
     bitmap[i     ] = data[ i*2     ];
     bitmap[i + 16] = data[ i*2 + 1 ];
    }
    matrix[ i & 2 ? 2 : 0 ].write_buffer( bitmap   , 16, i & 1 ? 16 : 0 );
    matrix[ i & 2 ? 3 : 1 ].write_buffer( bitmap+16, 16, i & 1 ? 16 : 0 );
  }
  
  delay(1000);
  if( ++offset >= 3 ) {
    offset = 0;
  }
}
