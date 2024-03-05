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
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>  // sleep(3)
#include <stdbool.h> // bool, true, false

typedef enum  {OFF, SLOW, MEDIUM, FAST} CurrState;
CurrState currState = OFF;
typedef uint32_t RPM;
RPM currentSpeed_rpm = 0;
static RPM const stop_rpm = 0;
static RPM const slow_rpm = 60;
static RPM const medium_rpm = 90;
static RPM const fast_rpm = 120;

void setSpeed(RPM rpm) {
    currentSpeed_rpm = rpm;
}

bool onOffButton() {
    static const bool sequence[] = {1,0,0,0,0,0,0,};
    uint32_t sequence_size = sizeof(sequence)/sizeof(sequence[0]);
    static uint32_t idx = 0;

    bool retval = sequence[idx];
    idx = (idx+1) % sequence_size;
    return retval;
}
bool changeSpeedButton() {
    static const bool sequence[] = {0,1,0,0,1,1,0,0,1,1,0,0,};
    uint32_t sequence_size = sizeof(sequence)/sizeof(sequence[0]);
    static uint32_t idx = 0;

    bool retval = sequence[idx];
    idx = (idx+1) % sequence_size;
    return retval;
}

void displayState(CurrState inState,
                  bool onoff_pressed, bool speed_pressed,
                  CurrState outState) {
    typedef struct {CurrState s; char const * const txt;} MapEnumToText;
    MapEnumToText map[] = {
        {OFF, "OFF"},
        {SLOW, "SLOW"},
        {MEDIUM, "MEDIUM"},
        {FAST, "FAST"},
    };
    printf("in-state: %7s, ", map[inState].txt);

    printf("onoff pressed: %d, speed pressed: %d, ",
           onoff_pressed, speed_pressed);

    printf("out-state: %7s\n", map[outState].txt);
}
void updateState() {
    // poll the two buttons
    bool onoff_pressed = onOffButton();
    bool speed_pressed = changeSpeedButton();

    CurrState inState = currState;

    switch (currState) {
    case OFF:
        setSpeed(stop_rpm);
        if (onoff_pressed || speed_pressed) currState = SLOW;
        break;
    case SLOW:
        setSpeed(slow_rpm);
        if (onoff_pressed) currState = OFF;
        if (speed_pressed) currState = MEDIUM;
        break;
    case MEDIUM:
        setSpeed(medium_rpm);
        if (onoff_pressed) currState = OFF;
        if (speed_pressed) currState = FAST;
        break;
    case FAST:
        setSpeed(fast_rpm);
        if (onoff_pressed) currState = OFF;
        if (speed_pressed) currState = SLOW;
        break;
    default:
        printf("ERROR: unexpected state: %d", currState);
        exit(1);
    }
    CurrState outState = currState;
    displayState(inState, onoff_pressed, speed_pressed, outState);
}

int main() {
    while (1) {
        updateState();
        sleep(1);  // sleep 1 second
    }
}
