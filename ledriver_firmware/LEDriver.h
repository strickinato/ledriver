
#ifndef LEDriver_h
#define LEDriver_h

#include "Arduino.h"
#include "View.h"
#include "Network.h"
#include "PinMap.h"
#include "Mode.h"
#include "Defaults.h"
#include "Commands.h"
#include "LrBuffer.h"

#include "SD.h"
#include "SPI.h"
// #include "SDConfigFile.h"
// #include "FastLED.h"

#include <WebSocketServer.h>
// #include <WebSocket.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

#define VERSION 0.00001

#define SD_HEADER_SIZE 2
#define MODE_COUNT 7
#define INPUT_POLL_RATE 100

enum Modes{
    DEMO_MODE,
    ARTNET_MODE,
    SERIAL_MODE,
    TEST_MODE,
    CUSTOM_MODE,
    SDPLAY_MODE,
    FUN_MODE
};

class LEDriver {
    public:
        LEDriver();
        void begin();
        void update();
        void runWithWebsocket(EthernetServer * serverForSocket);
        void loadConfigFile(const char * _file);
        void saveConfigFile(JsonObject &root, const char * _fileName);
        void parseJsonConfig(JsonObject &config);
        void receiveJson(const char * _received);

        void pushConfig();
        void makeConfig();
        void receiveCommand(uint8_t _cmd, uint8_t _val);
        void receiveOSC(uint8_t * _mess, uint8_t _sz);

        void checkUdpForOSC();
        void checkWebsocket();
        // void onData(WebSocket &socket, char* dataString, byte frameLength);
        LrBuffer dataBuffer;
        CRGB * leds;
        // CRGB * ledStrips[8];
        uint16_t NUM_LEDS;
        uint8_t currentMode = 0;

        // websocket
        WebSocketServer webSocketServer;
        EthernetClient client;
        // EthernetServer * serverForSocket;

        // modes
        Mode * modePointers[MODE_COUNT];
        DemoMode demoMode;
        ArtNetMode artnetMode;
        SerialMode serialMode;
        TestMode testMode;
        CustomMode customMode;
        SDCardPlaybackMode sdPlaybackMode;
        FunMode funMode;
        void setMode(uint8_t _mode);

        // sync and timming
        uint16_t frameCount;
        uint16_t fpsCount;
        uint16_t fps;
        uint32_t timeStamp;

        View view;

        bool gotNewData;
        bool gotFullArtnet;

        void (*updateCallback)();
        void setCallback(void (*callback)())
        { updateCallback = callback; }

    private:
        Network network;
        bool flasher;

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
        bool button4;


};


#endif
