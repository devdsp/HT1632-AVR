/*
Writen 2010 by Adam Thomas
This example is public domain.
*/
#include <ht1632.h>
#include <MHV_io_ArduinoDuemilanove328p.h>

void * operator new(size_t size); 
void operator delete(void * ptr); 

void * operator new(size_t size) { 
  return malloc(size); 
} 

void operator delete(void * ptr) { 
  free(ptr); 
} 


HT1632 *matrix;

void setup ()
{  
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  
  matrix = new HT1632( MHV_ARDUINO_PIN_4,MHV_ARDUINO_PIN_8,MHV_ARDUINO_PIN_10,MHV_ARDUINO_PIN_9, HT1632::pmos_8commons );
}

void loop ()
{
  // fill the display using consecutive writes
  matrix->set_mode( HT1632::write_mode );
  matrix->send_address( 0 );
  for( int i = 0; i < matrix->memory_limit(); ++i ) {
    matrix->send_data(0xF);
  }
    
  delay(1000);
  
  //blank the display using consecurtive writes
  matrix->set_mode( HT1632::write_mode );
  matrix->send_address( 0 );
  for( int i = 0; i < matrix->memory_limit(); ++i ) {
    matrix->send_data(0x0);
  }
    
  delay(1000);

  // chase a single LED by overwriting memory pages
  for( int i = 0; i < matrix->memory_limit(); ++i ) {
    for( int j = _BV(0); j < _BV(4); j<<=1 ) {
      matrix->write_to_address( i, j );
      delay(25);
    }
    matrix->write_to_address( i, 0 );
  }
  
  delay(1000);
 
  // randomly toggle bits
  for( int i = 0; i < matrix->memory_limit(); ++i ) {
    int addr = (int)random(matrix->memory_limit());
    matrix->set_mode( HT1632::write_mode );
    matrix->send_address( addr );
    uint8_t bits = matrix->read_nibble();
    bits ^= 1 << (int) random(4);
    matrix->send_data( bits );
    delay(100);
  }
}
