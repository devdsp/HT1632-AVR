/*
 * ht1632.h
 * defintions for Holtek ht1632 LED driver.
 */
#ifndef __HT1632_H_
#define __HT1632_H_

#include "WProgram.h"
#include "MHV_io.h"

class HT1632 {
  
public:
  enum Mode {
    no_mode = 0,
    read_mode = B110,
    write_mode = B101,
    command_mode = B100
  };
  
  enum Command {
    disable_system_oscillator =   B00000000,
    enable_system_oscillator =    B00000001,
    leds_off =                    B00000010,
    leds_on =                     B00000011,
    blink_off =                   B00001000,
    blink_on =                    B00001001,
    slave_mode =                  B00010000,
    master_mode =                 B00010100,
    clock_source_internal =       B00011000,
    clock_source_external =       B00011100,
    nmos_8commons =               B00100000,
    nmos_16commons =              B00100100,
    pmos_8commons =               B00101000,
    pmos_16commons =              B00101100,
    pwm_1 =                       B10100000,
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
    boolean master = true 
  );
  
  void set_mode(Mode mode);
  void send_address( byte addr );
  void send_data( byte data );
  void send_command( Command command );
  byte read_nibble();
  
  void deselect();
  void select();
  
  boolean map_coordinate( byte x, byte y, byte &addr, byte &bitmask );
  
  void set_max_coordinates( byte x, byte y );
  
  byte get_maxx() {return maxx;}
  byte get_maxy() {return maxy;}
  
protected:  
  

private:
  volatile uint8_t *cs_dir,   *cs_output_reg,   *cs_input_reg;   uint8_t cs_pin;
  volatile uint8_t *wclk_dir, *wclk_output_reg, *wclk_input_reg; uint8_t wclk_pin;
  volatile uint8_t *rclk_dir, *rclk_output_reg, *rclk_input_reg; uint8_t rclk_pin;
  volatile uint8_t *data_dir, *data_output_reg, *data_input_reg; uint8_t data_pin;
  
  byte maxx, maxy;
  Command commons;
  Mode mode;
  
  void write_bits_msb( byte startingbit, byte value );
  void write_bits_lsb( byte finishingbit, byte value );
  byte read_bits_lsb();
  
};

#endif
