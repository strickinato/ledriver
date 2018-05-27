#ifndef LrBuffer_h
#define LrBuffer_h

#include "Arduino.h"
#include "FastLED.h"

// #define LR_MAX_LED_PER_STRIP 240
// #define LR_NUM_STRIPS 8
// #define LR_MAX_NUM_LEDS LR_NUM_STRIPS * LR_MAX_LED_PER_STRIP
//
// #define LR_MAX_BUFFER_SIZE LR_MAX_NUM_LEDS * 3 + 512 * 2

// tmeplate thing that might help
// https://stackoverflow.com/questions/15774534/c-how-to-get-the-length-of-a-pointer-array
// or stash the value at the begining of array, and point to array+1
class LrBuffer {
    public:
        LrBuffer();
        // // this is the main buffer that gets output.
        // uint8_t drawBuffer[LR_MAX_BUFFER_SIZE];
        // uint8_t mixBuffer[LR_MAX_BUFFER_SIZE];
        // // this can get set by configuration
        // uint16_t ledCount;
        //
        // CRGB * getLEDs();
};
#endif
