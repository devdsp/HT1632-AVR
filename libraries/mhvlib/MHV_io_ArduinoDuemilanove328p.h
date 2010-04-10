/*
 * MHV_io_ArduinoDuemilanove328p.h
 *
 *  Created on: 21/02/2010
 *      Author: deece

Copyright (C) 2010 Alastair D'Silva

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

#ifndef MHV_IO_ARDUINO_DUIMILANOVE_328P_H_
#define MHV_IO_ARDUINO_DUIMILANOVE_328P_H_

#include <avr/io.h>

#define MHV_ARDUINO_PIN_0	&DDRD,	&PORTD,	&PIND,	0	//USART0_RX
#define MHV_ARDUINO_PIN_1	&DDRD,	&PORTD,	&PIND,	1	//USART0_TX
#define MHV_ARDUINO_PIN_2	&DDRD,	&PORTD,	&PIND,	2
#define MHV_ARDUINO_PIN_3	&DDRD,	&PORTD,	&PIND,	3 	//PWM
#define MHV_ARDUINO_PIN_4	&DDRD,	&PORTD,	&PIND,	4
#define MHV_ARDUINO_PIN_5	&DDRD,	&PORTD,	&PIND,	5 	//PWM
#define MHV_ARDUINO_PIN_6	&DDRD,	&PORTD,	&PIND,	6 	//PWM
#define MHV_ARDUINO_PIN_7	&DDRD,	&PORTD,	&PIND,	7

#define MHV_ARDUINO_PIN_8	&DDRB,	&PORTB,	&PINB,	0
#define MHV_ARDUINO_PIN_9	&DDRB,	&PORTB,	&PINB,	1 	//PWM
#define MHV_ARDUINO_PIN_10	&DDRB,	&PORTB,	&PINB,	2 	//PWM
#define MHV_ARDUINO_PIN_11	&DDRB,	&PORTB,	&PINB,	3 	//PWM
#define MHV_ARDUINO_PIN_12	&DDRB,	&PORTB,	&PINB,	4
#define MHV_ARDUINO_PIN_13	&DDRB,	&PORTB,	&PINB,	5

#endif
