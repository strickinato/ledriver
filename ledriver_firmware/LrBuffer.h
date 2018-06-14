#ifndef LrBuffer_h
#define LrBuffer_h

#include "Arduino.h"
#include "Defaults.h"


#define LR_LED_PER_STRIP 340
#define LR_NUM_STRIPS 8
#define LR_NUM_LEDS LR_NUM_STRIPS * LR_LED_PER_STRIP

#define LR_MAX_BUFFER_SIZE LR_NUM_LEDS * 3 + 512 * 2

// tmeplate thing that might help
// https://stackoverflow.com/questions/15774534/c-how-to-get-the-length-of-a-pointer-array
// or stash the value at the begining of array, and point to array+1
class LrBuffer {
    public:
        LrBuffer();
        void begin();
        void enableFastLED();
        bool ledsEnabled;
        // this is the main buffer that gets output.
        uint8_t drawBuffer[LR_MAX_BUFFER_SIZE];
        // this can get set by configuration
        uint16_t ledCount;
        CRGB * leds;
        CRGB * getLEDs();
        // CRGB leds[23];
        // getDMX output a or b
        uint8_t * getDMX(uint8_t _u);

        // show call FastLED.show()
        // but in LrDoubleBuffer, it would do the mix operation first
        virtual void show();

    private :
        uint8_t somevar;

};


// class LrDoubleBuffer : LrBuffer {
//     public:
//         LrDoubleBuffer();
//         // the mix buffer is ment to suplement
//         uint8_t mixBuffer[LR_MAX_BUFFER_SIZE];
//
//
//         // crossfade between main and mix
//         // void crossFade(uint8_t _v);
//         // // crossFade with DMX ?
//         // void crossFade(uint8_t _v, 1);
// };


#endif
