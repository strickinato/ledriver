
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
#include "Defaults.h"

#define VERSION 0.00001

#define SD_HEADER_SIZE 2
#define MODE_COUNT 5

enum Modes{
    DEMO_MODE,
    ARTNET_MODE,
    SERIAL_MODE,
    TEST_MODE,
    CUSTOM_MODE
};

enum Commands{
    LED_DATA_CMD = 42,
    SET_BRIGHTNESS_CMD = 1,
    BLACKOUT_CMD = 43,
};


class LEDriver {
    public:
        LEDriver();
        void begin(CRGB *_leds, uint16_t _count);
        void update();
        void parseConfig(const char * _file);
        CRGB * leds;
        uint16_t NUM_LEDS;
        uint8_t currentMode = 0;


        // modes
        Mode * modePointers[MODE_COUNT];
        DemoMode demoMode;
        ArtNetMode artnetMode;
        SerialMode serialMode;
        TestMode testMode;
        CustomMode customMode;

        void setMode(uint8_t _mode);

        // sync and timming
        uint16_t frameCount;

    private:
        Network network;
        LEDriverView view;

        // input
        void checkInput();
        uint16_t pot1_value;
        uint16_t pot2_value;
        uint16_t button_value;
        uint8_t buttonState;
        uint8_t buttonPress;

        bool button1;
        bool button2;
        bool button3;

        // SPI switching
        void enableSDcard();
        void enableWiznet();

};





#endif
