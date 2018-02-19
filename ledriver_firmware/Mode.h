#ifndef Mode_h
#define Mode_h

#include "PinMap.h"
#include "FastLED.h"


class Mode {
    public:
        Mode();
        virtual void update();

        // virtual void buttonPress(uint8_t _button);
        const char * name;// = "mode";

        static CRGB * leds;
        static uint16_t ledCount;
        static uint16_t frameCount;
        static uint16_t pot1;
        static uint16_t pot2;
        static uint8_t buttonPress;

        uint16_t framesPerSecond;
};



class DemoMode : public Mode {
    public:
        DemoMode();
        virtual void update();

};

//
class ArtNetMode : public Mode {
    public:
        ArtNetMode();
        virtual void update();

        struct OutputMapping
        {
            uint8_t output; // output a,b,c
            uint16_t startUniverse; //
            uint16_t startChannel; // 0 or offset within universe
            uint16_t channelCount;
            // reverse??
        };
};

class SerialMode : public Mode {
    public:
        SerialMode();
        virtual void update();
        uint16_t errorCount;
};

class TestMode : public Mode {
    public:
        TestMode();
        virtual void update();

};

class CustomMode : public Mode {
    public:
        CustomMode();
        virtual void update();
        void (*customUpdate)();
        void setCallback(void (*callback)())
        { customUpdate = callback; }
};

#endif
