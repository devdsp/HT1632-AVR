/*
 * ht1632.h
 * defintions for Holtek ht1632 LED driver.

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

#ifndef __HT1632_H_
#define __HT1632_H_

#include <stdint.h> 
#include "MHV_io.h"

class HT1632 {
  
public:
  enum Mode {
    no_mode = 0,
    read_mode = 6,
    write_mode = 5,
    command_mode = 4
  };
  
  enum Command {
    disable_system_oscillator =   0,
    enable_system_oscillator =    1,
    leds_off =                    2,
    leds_on =                     3,
    blink_off =                   8,
    blink_on =                    9,
    slave_mode =                  16,
    master_mode =                 20,
    clock_source_internal =       24,
    clock_source_external =       28,
    nmos_8commons =               32,
    nmos_16commons =              36,
    pmos_8commons =               40,
    pmos_16commons =              44,
    pwm_1 =                       160,
    pwm_2,
    pwm_3,
    pwm_4,
    pwm_5,
    pwm_6,
    pwm_7,
    pwm_8,
    pwm_9,
    pwm_10,
    pwm_11,
    pwm_12,
    pwm_13,
    pwm_14,
    pwm_15,
    pwm_16
  };
  
  HT1632( 
    volatile uint8_t *cs_dir,   volatile uint8_t *cs_output_reg,   volatile uint8_t *cs_input_reg,   uint8_t cs_pin, 
    volatile uint8_t *wclk_dir, volatile uint8_t *wclk_output_reg, volatile uint8_t *wclk_input_reg, uint8_t wclk_pin, 
    volatile uint8_t *rclk_dir, volatile uint8_t *rclk_output_reg, volatile uint8_t *rclk_input_reg, uint8_t rclk_pin, 
    volatile uint8_t *data_dir, volatile uint8_t *data_output_reg, volatile uint8_t *data_input_reg, uint8_t data_pin,
    Command commons, 
    bool master = true 
  );

  bool write_to_address( uint8_t addr, uint8_t value );
  bool read_from_address( uint8_t addr, uint8_t *value );  

  void set_mode(Mode mode);
  void send_address( uint8_t addr );
  void send_data( uint8_t data );
  void send_command( Command command );
  uint8_t read_nibble();
  
  void deselect();
  void select();
  
private:
  volatile uint8_t *cs_dir,   *cs_output_reg,   *cs_input_reg;   uint8_t cs_pin;
  volatile uint8_t *wclk_dir, *wclk_output_reg, *wclk_input_reg; uint8_t wclk_pin;
  volatile uint8_t *rclk_dir, *rclk_output_reg, *rclk_input_reg; uint8_t rclk_pin;
  volatile uint8_t *data_dir, *data_output_reg, *data_input_reg; uint8_t data_pin;
  
  Command commons;
  Mode mode;
  
  void write_bits_msb( uint8_t startingbit, uint8_t value );
  void write_bits_lsb( uint8_t finishingbit, uint8_t value );
  uint8_t read_bits_lsb();
  
};

#endif
