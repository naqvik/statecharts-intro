/**
   Implementation of a three-speed fan with two pushbuttons:
   Buttons:
   1. CHANGE (momentary contact?)
      - change speed button
      - changes speed to off, slow, medium, or fast
      - input is true for one cycle when change speed button is pressed

   2. ONOFF (momentary contact?)
      - on/off button
      - turns the fan on or off
      - input is true for one cycle when on/off button is pressed
 */

#include <stdio.h>
#include <stdint.h>

static enum currSpeed {OFF, SLOW, MEDIUM, FAST};

int main() {
    printf("hello world\n");
}