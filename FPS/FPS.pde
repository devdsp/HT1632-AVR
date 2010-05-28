/*
Writen 2010 Adam Thomas
This example is public domain
*/
#include <ht1632.h>
#include <MHV_io_ArduinoDuemilanove328p.h>
#include <avr/pgmspace.h>
#include "font.h"

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

uint32_t next_time(0);
uint16_t frame_count(0);
uint16_t fps(0);


void loop ()
{
  if( millis() > next_time ) {
    next_time += 1000;
    fps = frame_count;
    frame_count = 0;
  }

  uint16_t copy = fps;
  
  uint8_t buffer[32] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
  };
    
  for( int i = 32; copy != 0 && i > 0; --i ) {
    uint8_t digit = copy%10;
    copy /= 10;
    uint8_t char_index = 48 + digit - DEDP105_FONT_FIRST_CHAR;
    uint8_t char_width = pgm_read_byte( &dedp105_font_widths[char_index] );
    uint16_t char_offset = pgm_read_byte( &dedp105_font_offsets[char_index] );
    
    i -= char_width;
    memcpy_P( buffer+i, &dedp105_font[char_offset], char_width );
  }
  
  matrix[0].write_buffer( buffer, 32 );
  matrix[1].write_buffer( buffer, 32 );
  matrix[2].write_buffer( buffer, 32 );
  matrix[3].write_buffer( buffer, 32 );
   
  ++frame_count;
}
