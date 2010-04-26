/*
Copyright (C) 2010 Adam Thomas

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/
#include "ht1632.h"
#include <pins_arduino.h>

HT1632::HT1632( 
  volatile uint8_t *cs_dir,   volatile uint8_t *cs_output_reg,   volatile uint8_t *cs_input_reg,   uint8_t cs_pin, 
  volatile uint8_t *wclk_dir, volatile uint8_t *wclk_output_reg, volatile uint8_t *wclk_input_reg, uint8_t wclk_pin, 
  volatile uint8_t *rclk_dir, volatile uint8_t *rclk_output_reg, volatile uint8_t *rclk_input_reg, uint8_t rclk_pin, 
  volatile uint8_t *data_dir, volatile uint8_t *data_output_reg, volatile uint8_t *data_input_reg, uint8_t data_pin,
  Command commons, 
  bool master 
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
  
  _mhv_setOutput(cs_dir,cs_output_reg,cs_intput_reg,cs_pin);
  _mhv_setOutput(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
  _mhv_setOutput(rclk_dir,rclk_output_reg,rclk_intput_reg,rclk_pin);
  _mhv_setOutput(data_dir,data_output_reg,data_intput_reg,data_pin);
  
  _mhv_pinOn(cs_dir,cs_output_reg,cs_intput_reg,cs_pin);
  _mhv_pinOn(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
  _mhv_pinOn(rclk_dir,rclk_output_reg,rclk_intput_reg,rclk_pin);
  
  set_mode( command_mode );
  send_command(disable_system_oscillator);
  send_command((Command)commons);
  
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
  for( uint8_t i =0; i < 0x5E; ++i ) {
    send_data( 0x00 );
  }
  deselect();
}

void HT1632::set_mode(Mode mode) {
  deselect();
  select();
  write_bits_msb( _BV(2), (uint8_t)mode );
  this->mode = mode;
}

void HT1632::write_bits_msb( uint8_t startingbit, uint8_t value ) { 
  _mhv_setOutput(data_dir,data_output_reg,data_intput_reg,data_pin);
  while (startingbit) {
    _mhv_pinOff(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
    if( value & startingbit ) {
      _mhv_pinOn(data_dir,data_output_reg,data_intput_reg,data_pin);
    } else {
      _mhv_pinOff(data_dir,data_output_reg,data_intput_reg,data_pin);
    }
    _mhv_pinOn(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
    startingbit >>= 1;
  }
}  

void HT1632::write_bits_lsb( uint8_t finishingbit, uint8_t value ) {    
  finishingbit <<=1;
  _mhv_setOutput(data_dir,data_output_reg,data_intput_reg,data_pin);
  uint8_t i = _BV(0);
  while (finishingbit != i) {
    _mhv_pinOff(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
    if( value & i ) {
      _mhv_pinOn(data_dir,data_output_reg,data_intput_reg,data_pin);
    } else {
      _mhv_pinOff(data_dir,data_output_reg,data_intput_reg,data_pin);
    }
    _mhv_pinOn(wclk_dir,wclk_output_reg,wclk_intput_reg,wclk_pin);
    i <<= 1;
  }
}

uint8_t HT1632::memory_limit() {
  return ( this->commons == nmos_8commons || this->commons == pmos_8commons ) ?
    0x40 : 0x5E;
}

bool HT1632::write_to_address( uint8_t addr, uint8_t value ) {
  if( addr >= memory_limit() ) 
    return false;

  set_mode(write_mode);
  send_address( addr );
  send_data( value );
  deselect();
  return true;
}

bool HT1632::read_from_address( uint8_t addr, uint8_t *value ) {
  if( addr >= memory_limit() ) 
    return false;

  set_mode(read_mode);
  send_address( addr );
  *value = read_nibble();
  deselect();
  return true;
}

bool HT1632::write_buffer( uint8_t *buffer, uint8_t length, uint8_t offset ) {
  if( (length + offset)*2 > memory_limit() )
    return false;
  
  uint8_t *pointer = buffer;  

  set_mode(write_mode);
  send_address( offset*2 );
  
  while( length-- ) {
    uint8_t temp = *pointer;
    send_data( temp );
    swap( temp );
    send_data( temp );
    ++pointer; 
  }
}

bool HT1632::read_buffer( uint8_t *buffer, uint8_t length, uint8_t offset ) {
  if( (length + offset)*2 > memory_limit() )
     return false;
  
  uint8_t *pointer = buffer;  

  set_mode(write_mode);
  send_address( offset*2 );
  
  while( length-- ) {
    uint8_t temp = read_nibble();
    swap( temp );
    temp |= read_nibble();
    swap( temp );
    *pointer = temp;
    ++pointer; 
  }
}

void HT1632::send_address( uint8_t addr ) {
  write_bits_msb( _BV(6), addr );
}

void HT1632::send_data( uint8_t data ) { // only sends bottom nibble
  write_bits_lsb( _BV(3), data );
}

void HT1632::send_command( Command command ) {
  write_bits_msb( _BV(7), (uint8_t)command );
  write_bits_msb( _BV(0), 0 );
}

uint8_t HT1632::read_bits_lsb() {
  uint8_t buffer(0);
  _mhv_setInput(data_dir,data_output_reg,data_intput_reg,data_pin);
  for( int i = 1; i < _BV(4); i<<=1 ) {
    _mhv_pinOff(rclk_dir,rclk_output_reg,rclk_input_reg,rclk_pin);
    if( _mhv_pinRead(data_dir,data_output_reg,data_input_reg,data_pin) ) {
      buffer |= i;
    }
    _mhv_pinOn(rclk_dir,rclk_output_reg,rclk_input_reg,rclk_pin);
  }
  return buffer;
}

uint8_t HT1632::read_nibble() {
  return read_bits_lsb();
}

void HT1632::deselect() {
  _mhv_pinOn(cs_dir,cs_output_reg,cs_intput_reg,cs_pin);
}

void HT1632::select() {
  _mhv_pinOff(cs_dir,cs_output_reg,cs_intput_reg,cs_pin);
}

