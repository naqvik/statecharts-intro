/** -*- c++ -*-
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

typedef enum  {OFF, SLOW, MEDIUM, FAST} FanState;
typedef uint32_t RPM;
static RPM const stop_rpm = 0;
static RPM const slow_rpm = 60;
static RPM const medium_rpm = 90;
static RPM const fast_rpm = 120;

typedef struct {
    FanState currState;
    RPM currentSpeed_rpm;
} Fan;

void fan_init(Fan *f) {
    f->currState = OFF;
    f->currentSpeed_rpm = stop_rpm;
}

void fan_setSpeed(Fan *f, RPM rpm) {
    f->currentSpeed_rpm = rpm;
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

void displayState(FanState inState,
                  bool onoff_pressed, bool speed_pressed,
                  FanState outState) {
    typedef struct {FanState s; char const * const txt;} MapEnumToText;
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
void fan_updateState(Fan* f, bool onoff_pressed, bool speed_pressed) {
    FanState inState = f->currState;

    switch (f->currState) {
    case OFF:
        fan_setSpeed(f, stop_rpm);
        if (onoff_pressed || speed_pressed) f->currState = SLOW;
        break;
    case SLOW:
        fan_setSpeed(f, slow_rpm);
        if (onoff_pressed) f->currState = OFF;
        if (speed_pressed) f->currState = MEDIUM;
        break;
    case MEDIUM:
        fan_setSpeed(f, medium_rpm);
        if (onoff_pressed) f->currState = OFF;
        if (speed_pressed) f->currState = FAST;
        break;
    case FAST:
        fan_setSpeed(f, fast_rpm);
        if (onoff_pressed) f->currState = OFF;
        if (speed_pressed) f->currState = SLOW;
        break;
    default:
        printf("ERROR: unexpected state: %d", f->currState);
        exit(1);
    }
    FanState outState = f->currState;
    displayState(inState, onoff_pressed, speed_pressed, outState);
}

#ifndef TEST
int main() {
    Fan f;
    fan_init(&f);
    
    while (1) {
        // poll the two buttons
        bool onoff_pressed = onOffButton();
        bool speed_pressed = changeSpeedButton();

        fan_updateState(&f, onoff_pressed, speed_pressed);
        sleep(1);  // sleep 1 second
    }
}
#else // TEST defined

typedef struct {
    FanState inState;
    bool onoff_pressed;
    bool speed_pressed;
    FanState outState;
} TestRecord;
TestRecord testRecords[] = {
    // in-state, onoff, speed, out-state
    {OFF, false, false, OFF,},
    {OFF, false, true, SLOW,},
    {OFF, true, false, SLOW,},
    {OFF, true, true, OFF,},

    {SLOW, false, false, SLOW,},
    {SLOW, false, true, MEDIUM,},
    {SLOW, true, false, OFF,},
    {SLOW, true, true, OFF,},

    {MEDIUM, false, false, MEDIUM,},
    {MEDIUM, false, true, FAST,},
    {MEDIUM, true, false, OFF,},
    {MEDIUM, true, true, OFF,},

    {FAST, false, false, FAST,},
    {FAST, false, true, SLOW,},
    {FAST, true, false, OFF,},
    {FAST, true, true, OFF,},
};

int main() {
    Fan f;
    fan_init(&f);

    uint32_t n_records = sizeof(testRecords)/sizeof(testRecords[0]);

    for (int i=0; i < n_records; ++i) {
        // given the current state and button press inputs
        f.currState = testRecords[i].inState;
        bool onoff_pressed = testRecords[i].onoff_pressed;
        bool speed_pressed = testRecords[i].speed_pressed;

        // then, when the state is updated
        fan_updateState(&f, onoff_pressed, speed_pressed);

        // the new state is equal to the predicted state
        if (f.currState == testRecords[i].outState)
            continue;
        else {
            fprintf(stderr, "Error: expected state %d\n",
                    testRecords[i].outState);
            fprintf(stderr, "       got state %d\n",
                    f.currState);
        }
    }
}
#endif
