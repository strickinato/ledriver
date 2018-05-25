#ifndef Mode_h
#define Mode_h

#include "PinMap.h"
#include "FastLED.h"
#include "Commands.h"
// #include "LEDriver.h"
#include "SD.h"

#define MAX_UNIVERSE_SUPPORTED 32


class Mode {
    public:
        Mode();
        virtual void update();
        virtual void receiveCommand(uint8_t _cmd, uint8_t _val);

        // virtual void buttonPress(uint8_t _button);
        const char * name;// = "mode";

        static CRGB * leds;
        static uint16_t ledCount;
        static uint16_t frameCount;
        static uint16_t pot1;
        static uint16_t pot2;
        static uint8_t buttonPress;


        static uint8_t brightness;
        uint16_t framesPerSecond;
};

////////////////////////////////////////////////////////////////////////////////////////////////

class DemoMode : public Mode {
    public:
        DemoMode();
        virtual void update();

};

////////////////////////////////////////////////////////////////////////////////////////////////

class ArtNetMode : public Mode {
    public:
        ArtNetMode();
        virtual void update();
        void setup();
        //total universe count
        uint8_t totalUniverseCount;
        uint32_t receivedUniverses; // starts at lowest universe
        uint32_t gotAllUniverses;
        uint16_t currentSequence;

        uint16_t universeToIndex[MAX_UNIVERSE_SUPPORTED];
        void receivePacket(uint8_t * _data, uint8_t _sequence, uint16_t _universe, uint16_t _dataLenght);
        // 8X
        // struct OutputMapping
        // {
        //     uint8_t output; // output a,b,c
        //     uint16_t startUniverse; //
        //     uint16_t startChannel; // 0 or offset within universe
        //     uint16_t channelCount;
        //     // reverse??
        // };
        // OutputMapping mappings[8];
};

////////////////////////////////////////////////////////////////////////////////////////////////

class SerialMode : public Mode {
    public:
        SerialMode();
        virtual void update();
        uint16_t errorCount;
};

////////////////////////////////////////////////////////////////////////////////////////////////

class TestMode : public Mode {
    public:
        TestMode();
        virtual void update();

};

////////////////////////////////////////////////////////////////////////////////////////////////

class CustomMode : public Mode {
    public:
        CustomMode();
        virtual void update();
        void (*customUpdate)();
        void setCallback(void (*callback)())
        { customUpdate = callback; }
};

class SDCardPlaybackMode : public Mode {
    public:
        SDCardPlaybackMode();
        virtual void update();
        // void setProperty();
        void playAnimation(uint8_t _animation);
        File currentFile;
        uint8_t animationNumber;
};


#endif
