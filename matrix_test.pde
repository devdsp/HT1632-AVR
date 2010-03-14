
#include "ht1632.h"
#include "font.h"
#include <stdlib.h> 

#include "MHV_io_ArduinoDuemilanove328p.h"

void * operator new(size_t size); 
void operator delete(void * ptr); 

void * operator new(size_t size) { 
  return malloc(size); 
} 

void operator delete(void * ptr) { 
  free(ptr); 
} 

uint8_t revbits(uint8_t arg) { 
  uint8_t result=0;
  if (arg & (_BV(0))) result |= _BV(7); 
  if (arg & (_BV(1))) result |= _BV(6); 
  if (arg & (_BV(2))) result |= _BV(5); 
  if (arg & (_BV(3))) result |= _BV(4); 
  if (arg & (_BV(4))) result |= _BV(3); 
  if (arg & (_BV(5))) result |= _BV(2); 
  if (arg & (_BV(6))) result |= _BV(1); 
  if (arg & (_BV(7))) result |= _BV(0); 
  return result;
}

HT1632 *matrix[4];

uint8_t max_cols = 127;
int16_t col_offset = max_cols;

HT1632 *cur_mat = 0;
char *string = "Welcome to Make, Hack, Void. Have a safe and productive day.";
char *first_char = string;
uint8_t first_char_col = 0;
char *cptr = first_char;
uint8_t char_col = first_char_col;

void setup ()
{  
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  
  matrix[0] = new HT1632( MHV_ARDUINO_PIN_4,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons );
  matrix[1] = new HT1632( MHV_ARDUINO_PIN_5,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons, false );
  matrix[2] = new HT1632( MHV_ARDUINO_PIN_6,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons, false );
  matrix[3] = new HT1632( MHV_ARDUINO_PIN_7,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons, false );
}

void loop ()
{
  uint8_t char_index, char_width;      
  uint16_t char_offset;
  uint8_t first_char_width;

  if( *first_char > DEDP105_FONT_FIRST_CHAR + DEDP105_FONT_CHAR_COUNT || *first_char < DEDP105_FONT_FIRST_CHAR ) {
    char_index = DEDP105_FONT_CHAR_UNKNOWN;
  } else {
    char_index = *first_char - DEDP105_FONT_FIRST_CHAR;
  }
         
  first_char_width = pgm_read_byte( &dedp105_font_widths[char_index] );
  
  if( col_offset < 0 ) {
    if( ++first_char_col > first_char_width ) {
      first_char_col = 0;
    
      if( *(++first_char) == '\0' ) {
        first_char = string;
        col_offset = max_cols;
      }
      if( *first_char > DEDP105_FONT_FIRST_CHAR + DEDP105_FONT_CHAR_COUNT || *first_char < DEDP105_FONT_FIRST_CHAR ) {
        char_index = DEDP105_FONT_CHAR_UNKNOWN;
      } else {
        char_index = *first_char - DEDP105_FONT_FIRST_CHAR;
      }
         
      first_char_width = pgm_read_byte( &dedp105_font_widths[char_index] );
    }
  }
  cptr = first_char;
  char_col = first_char_col;
  char_width = first_char_width;
  char_offset = pgm_read_word(&dedp105_font_offsets[char_index]);
  
  
  uint8_t pixels;
  
  for( byte col = 0; col < max_cols+1; ++col ) {
    switch(col) {
      case 0:
      case 32:
      case 64:
      case 96:
        if( cur_mat )
          cur_mat->deselect();
        cur_mat = matrix[(int)(col/32)];
        cur_mat->set_mode( HT1632::write_mode );
        cur_mat->send_address( 0 );
      break;
    }
    if( cur_mat ) {
      pixels = 0;
      if( col_offset < col && *cptr != '\0' ) {
        
        if( char_col < char_width ) {
          pixels = pgm_read_byte(&dedp105_font[(int)(char_offset + char_col)]);
        }
  
        ++char_col;
  
        if( char_col > char_width ) {
          char_col = 0;
          cptr++;
          if( *cptr > DEDP105_FONT_FIRST_CHAR + DEDP105_FONT_CHAR_COUNT || *cptr < DEDP105_FONT_FIRST_CHAR ) {
            char_index = DEDP105_FONT_CHAR_UNKNOWN;
          } else {
            char_index = *cptr - DEDP105_FONT_FIRST_CHAR;
          }
  
          char_width = pgm_read_byte(&dedp105_font_widths[char_index]);
          char_offset = pgm_read_word(&dedp105_font_offsets[char_index]);
        }
      }
      cur_mat->send_data(pixels);
      cur_mat->send_data(pixels>>4);
    }
  }
  col_offset--;
}
