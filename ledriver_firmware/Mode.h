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

};



class DemoMode : public Mode {
    public:
        DemoMode();
        virtual void update();

};



class ArtNetMode : public Mode {
    public:
        ArtNetMode();
        virtual void update();
};

class SerialMode : public Mode {
    public:
        SerialMode();
        virtual void update();

};

class TestMode : public Mode {
    public:
        TestMode();
        virtual void update();

};

#endif
