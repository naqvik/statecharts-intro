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

typedef enum  {OFF, SLOW, MEDIUM, FAST} CurrState;
CurrState currState = OFF;

int main() {
    printf("currState: %d\n", currState);
}
