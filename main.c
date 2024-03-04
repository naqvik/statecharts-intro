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
#include <unistd.h>  // sleep(3)
#include <stdbool.h> // bool, true, false

typedef enum  {OFF, SLOW, MEDIUM, FAST} CurrState;
CurrState currState = OFF;

bool onOffButton() {
    static const bool sequence[] = {1,0,0,0,0,0,0,};
    uint32_t sequence_size = sizeof(sequence)/sizeof(sequence[0]);
    static uint32_t idx = 0;

    bool retval = sequence[idx];
    idx = (idx+1) & sequence_size;
    return retval;
}

int main() {
    while (1) {
        printf("currState: %d\n", currState);
        printf("on-off state: %d\n", onOffButton());
        sleep(1);  // sleep 1 second
    }
}
