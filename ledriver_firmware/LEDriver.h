
#ifndef LEDriver_h
#define LEDriver_h

#include "Arduino.h"
#include "LEDriverView.h"
#include "Network.h"
#include "PinMap.h"
#include "SD.h"
#include "SPI.h"
#include "SDConfigFile.h"
#include "FastLED.h"
#include "Mode.h"

// enum Modes {STANDBY_MODE,
//             SDCARD_MODE,
//             SERIAL_MODE,
//             ARTNET_MODE,
//             DEMO_MODE,
//             TEST_MODE};

#define SD_HEADER_SIZE 2
#define MODE_COUNT 4
class LEDriver {
    public:
        LEDriver();
        void begin(CRGB *_leds, uint16_t _count);
        void update();
        CRGB * leds;
        uint16_t NUM_LEDS;
        uint8_t currentMode = 0;

        void checkInput();

        Mode * modePointers[MODE_COUNT];

        DemoMode demoMode;
        ArtNetMode artnetMode;
        SerialMode serialMode;
        TestMode testMode;

        uint16_t frameCount;
    private:
        Network network;
        LEDriverView view;
        File configFile;
        // const char *modeNames[6];

        uint16_t pot1_value;
        uint16_t pot2_value;
        uint16_t button_value;
        uint8_t buttonState;
        uint8_t buttonPress;

        bool button1;
        bool button2;
        bool button3;

        void enableSDcard();
        void enableWiznet();

        // void standbyMode();
        // void sdCardPlaybackMode();
        // void serialMode();
        // void artnetMode();
        // void demoMode();
        // void testMode();

};





#endif
