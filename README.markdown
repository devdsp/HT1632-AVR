Intro
=====
This project contains code to interface AVR micro controllers to LED matrix 
displays containing the Holtek HT1632 LED Driver IC. Sure Electronics is 
a great source of these displays. They make 8x32 and 16x24 displays, both of
which can be found on ebay in either red or green mono colour displays.

This code has been built in the Arduino environment and has some Arduino
idioms. I have tried to remove as much of the Arduino dependencies from the
core library as possible but I'm yet to write test code outside of the Arduino
environment so can't claim that it works.

The driver class supports all three modes (READ, WRITE & READ-MODIFY-WRITE) 
and should handle both 8x32 and 16x24 display layouts. As I only have 8x32 
displays I can only test on them.

See the example Arduino sketch for an example of streaming columns of data to
the displays. I'll be fleshing out examples as my time permits. These examples
will be released under a more permissive license than the current example.

Usage
=====
Copy both subdirectories of the 'libraries/' from this repository into the 
'libraries' subdirectory of your Arduino environment's 'sketches' directory.
You should then be able to #include <ht1632.h> in your sketches. To build the
examples copy them to 'sketches' and open them in the Arduino environment.

Arduino and Sure Electronics 0832 or 1624
-----------------------------------------

To run the examples on an Arduino and a Sure Electronics display; connect 
the Arduino to the display as follows:
  
    | Display Label | Arduino DIO | Sure's Connector |
    |      CS1      |      4      |        3         |
    |      CS2      |      5      |        1         |
    |      CS3      |      6      |        2         |
    |      CS4      |      7      |        4         |
    |       WR      |      8      |        5         |
    |     DATA      |      9      |        7         |
    |       RD      |     10      |        6         |
    |      GND      |    gnd      |       11         |
    |      +5V      |    +5V      |       12         |

Don't forget that if you are looking at an IDC plug from the bottom, as you
will if you are plugging in jumper cables, the rows are reversed. Just
remember that CS1, CS2, WR, DATA and GND lines are on the keyed side of the 
connector and the rest go on the flat side. 

License
=======
The MHVlib AVR IO macros have been re-released under a BSD style license. The 
HT1632 class is released under LGPL. The Marquee example sketch is released 
under GPL. The other examples are public domain. 
That's hardly complicated at all.

