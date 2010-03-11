
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

int max_cols = 127;
int starting_col = max_cols; 
HT1632 *cur_mat = 0;
char *string = "thank you for visiting makehackvoid. have a safe and productive day.";
  
void setup ()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
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
  char *c = string;
  uint8_t char_col = 0;
  
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
      if( 
        (col - starting_col) < 0 || 
        *c == '\0'
      ) {
        cur_mat->send_data(0);
        cur_mat->send_data(0);
      } else {
        if( char_col == 5 ) {
          c++;
          char_col = 0;
          cur_mat->send_data(0);
          cur_mat->send_data(0);
        } else {
          
          if (*c >= 'A' && *c <= 'Z' ||
            (*c >= 'a' && *c <= 'z') ) {
            *c &= 0x1F;   // A-Z maps to 1-26
          } 
          else if (*c >= '0' && *c <= '9') {
            *c = (*c - '0') + 27;
          } 
          else if (*c == ' ') {
            *c = 0; // space
          }
    
          byte dots = revbits(pgm_read_byte_near(&myfont[*c][char_col]));
          cur_mat->send_data(dots);
          cur_mat->send_data(dots>>4);
          
          char_col++;
          
        }
      }
    }
  }
  --starting_col;
  if( starting_col <-(signed int)(strlen(string)*6) ){
    starting_col = max_cols;
  }
}
