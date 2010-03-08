#include "ht1632.h"
#include <pins_arduino.h>

HT1632::HT1632( 
  volatile uint8_t *cs_dir,   volatile uint8_t *cs_output_reg,   volatile uint8_t *cs_input_reg,   uint8_t cs_pin, 
  volatile uint8_t *wclk_dir, volatile uint8_t *wclk_output_reg, volatile uint8_t *wclk_input_reg, uint8_t wclk_pin, 
  volatile uint8_t *rclk_dir, volatile uint8_t *rclk_output_reg, volatile uint8_t *rclk_input_reg, uint8_t rclk_pin, 
  volatile uint8_t *data_dir, volatile uint8_t *data_output_reg, volatile uint8_t *data_input_reg, uint8_t data_pin,
  Command commons, 
  boolean master 
) {
  this->cs_dir = cs_dir;
  this->cs_output_reg = cs_output_reg;
  this->cs_input_reg = cs_input_reg;
  this->cs_pin = cs_pin;
  
  this->wclk_dir = wclk_dir;
  this->wclk_output_reg = wclk_output_reg;
  this->wclk_input_reg = wclk_input_reg;
  this->wclk_pin = wclk_pin;
  
  this->rclk_dir = rclk_dir;
  this->rclk_output_reg = rclk_output_reg;
  this->rclk_input_reg = rclk_input_reg;
  this->rclk_pin = rclk_pin;
  
  this->data_dir = data_dir;
  this->data_output_reg = data_output_reg;
  this->data_input_reg = data_input_reg;
  this->data_pin = data_pin;
  
  this->commons = commons;
  this->mode = no_mode;
  
  mhv_setOutput(cs_dir,cs_output_reg,cs_intput_reg,cs_pin);
  mhv_setOutput(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
  mhv_setOutput(rclk_dir,rclk_output_reg,rclk_intput_reg,rclk_pin);
  mhv_setOutput(data_dir,data_output_reg,data_intput_reg,data_pin);
  
  mhv_pinOn(cs_dir,cs_output_reg,cs_intput_reg,cs_pin);
  mhv_pinOn(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
  mhv_pinOn(rclk_dir,rclk_output_reg,rclk_intput_reg,rclk_pin);
  
  set_mode( command_mode );
  send_command(disable_system_oscillator);
  send_command((Command)commons);
  
  if( commons == nmos_8commons || commons == pmos_8commons ) {
    maxx = 31;
    maxy = 7;
  } else {
    maxx = 23;
    maxy = 15;
  }
  
  if( master ) {
    send_command(master_mode);
  } else {
    send_command(slave_mode);
  }
  
  send_command(enable_system_oscillator);
  send_command(leds_on);
  send_command(blink_off);
  send_command(pwm_16);
  
  set_mode( HT1632::write_mode );
  send_address( 0 );
  for( byte i =0; i < 0x5E; ++i ) {
    send_data( 0x00 );
  }
  deselect();
}

void HT1632::set_mode(Mode mode) {
  deselect();
  select();
  write_bits_msb( _BV(2), (byte)mode );
  this->mode = mode;
}

void HT1632::write_bits_msb( byte startingbit, byte value ) { 
  mhv_setOutput(data_dir,data_output_reg,data_intput_reg,data_pin);
  while (startingbit) {
    mhv_pinOff(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
    if( value & startingbit ) {
      mhv_pinOn(data_dir,data_output_reg,data_intput_reg,data_pin);
    } else {
      mhv_pinOff(data_dir,data_output_reg,data_intput_reg,data_pin);
    }
    mhv_pinOn(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
    startingbit >>= 1;
  }
}  

void HT1632::write_bits_lsb( byte finishingbit, byte value ) {    
  finishingbit <<=1;
  mhv_setOutput(data_dir,data_output_reg,data_intput_reg,data_pin);
  byte i = _BV(0);
  while (finishingbit != i) {
    mhv_pinOff(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
    if( value & i ) {
      mhv_pinOn(data_dir,data_output_reg,data_intput_reg,data_pin);
    } else {
      mhv_pinOff(data_dir,data_output_reg,data_intput_reg,data_pin);
    }
    mhv_pinOn(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
    i <<= 1;
  }
}

void HT1632::send_address( byte addr ) {
  write_bits_msb( _BV(6), addr );
}

void HT1632::send_data( byte data ) { // only sends bottom nibble
  write_bits_lsb( _BV(3), data );
}

void HT1632::send_command( Command command ) {
  write_bits_msb( _BV(7), (byte)command );
  write_bits_msb( _BV(0), 0 );
}

byte HT1632::read_bits_lsb() {
  byte buffer(0);
  mhv_setInput(data_dir,data_output_reg,data_intput_reg,data_pin);
  for( int i = 1; i < _BV(4); i<<=1 ) {
    mhv_pinOff(rclk_dir,rclk_output_reg,rclk_input_reg,rclk_pin);
    if( mhv_pinRead(data_dir,data_output_reg,data_input_reg,data_pin) ) {
      buffer |= i;
    }
    mhv_pinOn(rclk_dir,rclk_output_reg,rclk_input_reg,rclk_pin);
  }
  return buffer;
}

byte HT1632::read_nibble() {
  return read_bits_lsb();
}

void HT1632::deselect() {
  mhv_pinOn(cs_dir,cs_output_reg,cs_intput_reg,cs_pin);
}

void HT1632::select() {
  mhv_pinOff(cs_dir,cs_output_reg,cs_intput_reg,cs_pin);
}

boolean HT1632::map_coordinate( byte x, byte y, byte &addr, byte &bitmask ) {
  if( x < 0 || y < 0 || x > maxx, y > maxy )
    return false;
  
  if( commons == nmos_8commons || commons == pmos_8commons ) {
    addr = (x<<1) + (y>>2);
    bitmask = _BV( y );
    if( y  > 3 )
      bitmask >>= 4;
  } else {
    addr = (x<<2) + (y>>2);
    bitmask = 8>>(y&3);
  }
  return true;  
}

void HT1632::set_max_coordinates( byte x, byte y ) {
  maxx = x;
  maxy = y;
}

