
#ifndef LEDriver_h
#define LEDriver_h

#include "Arduino.h"
#include "View.h"
#include "Network.h"
#include "PinMap.h"
#include "Mode.h"
#include "Defaults.h"
#include "Commands.h"

#include "SD.h"
#include "SPI.h"
#include "SDConfigFile.h"
#include "FastLED.h"

#include <WebSocketServer.h>
#include <Ethernet.h>

#define VERSION 0.00001

#define SD_HEADER_SIZE 2
#define MODE_COUNT 6
#define INPUT_POLL_RATE 100

enum Modes{
    DEMO_MODE,
    ARTNET_MODE,
    SERIAL_MODE,
    TEST_MODE,
    CUSTOM_MODE,
    SDPLAY_MODE
};

EthernetServer serverForSocket(80);



class LEDriver {
    public:
        LEDriver();
        void begin(CRGB *_leds, uint16_t _count);
        void update();
        void runWithWebsocket();
        void parseConfig(const char * _file);
        void receiveCommand(uint8_t _cmd, uint8_t _val);
        CRGB * leds;
        // CRGB * ledStrips[8];
        uint16_t NUM_LEDS;
        uint8_t currentMode = 0;

        // websocket
        WebSocketServer webSocketServer;
        EthernetClient client;

        // modes
        Mode * modePointers[MODE_COUNT];
        DemoMode demoMode;
        ArtNetMode artnetMode;
        SerialMode serialMode;
        TestMode testMode;
        CustomMode customMode;
        SDCardPlaybackMode sdPlaybackMode;
        void setMode(uint8_t _mode);

        // sync and timming
        uint16_t frameCount;
        uint16_t fpsCount;
        uint16_t fps;
        uint32_t timeStamp;

        View view;

    private:
        Network network;

        // input
        void checkInput();
        uint16_t pot1_value;
        uint16_t pot2_value;
        uint16_t button_value;
        uint8_t buttonState;
        uint8_t buttonPress;
        uint16_t lastInputCheck;
        bool button1;
        bool button2;
        bool button3;

        // SPI switching
        void enableSDcard();
        void enableWiznet();

};





#endif
