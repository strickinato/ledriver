
#ifndef LEDriver_h
#define LEDriver_h

// #define MAX_FRAME_LENGTH 512

#include "Arduino.h"
#include "View.h"
#include "Network.h"
#include "PinMap.h"
#include "Mode.h"
#include "Defaults.h"
#include "Commands.h"
#include "LrBuffer.h"
#include "LrDMX.h"


#include "SD.h"
#include "SPI.h"
// #include "SDConfigFile.h"
// #include "FastLED.h"

#include <WebSocketServer.h>
// #include <WebSocket.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
// #include <TeensyDmx.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

#define VERSION 0.00001

#define SD_HEADER_SIZE 2
#define MODE_COUNT 8
#define INPUT_POLL_RATE 100

#define CPU_REBOOT (_reboot_Teensyduino_());

class LEDriver {
    public:
        LEDriver();
        void begin();
        void update();
        void runWithWebsocket(EthernetServer * serverForSocket);
        void loadConfigFile(const char * _file);
        void saveConfigFile(JsonObject &root, const char * _fileName);
        void parseJsonConfig(JsonObject &config);
        void receiveJson(const char * _received, size_t _size);

        void makeConfig();
        void receiveCommand(uint8_t _cmd, uint8_t _val);
        void receiveOSC(uint8_t * _mess, uint8_t _sz);
        // void onData(WebSocket &socket, char* dataString, byte frameLength);
        LrBuffer dataBuffer;
        CRGB * leds;
        // CRGB * ledStrips[8];
        uint16_t NUM_LEDS;
        uint8_t currentMode = 0;
        // configured option
        uint8_t ledType;
        uint8_t ledColorOrder;
        uint8_t ledOutputMode = FAST_OCTO;
        uint8_t ledStartUniverse;

        LrDMX dmxOne;
        LrDMX dmxThree;
        void updateDMX();
        // uint8_t dmxOneMode = NONE_OUTPUT;
        // uint8_t dmxOneUniverse = 0;
        // uint8_t dmxTwoMode = NONE_OUTPUT;
        // uint8_t dmxTwoUniverse = 0;
        // uint8_t dmxThreeMode = NONE_OUTPUT;
        // uint8_t dmxThreeUniverse = 0;

        // websocket
        WebSocketServer webSocketServer;
        EthernetClient client;
        String socket_data;
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
        WebsocketControlMode websocketControlMode;
        void setMode(uint8_t _mode);

        // sync and timming
        uint16_t frameCount;
        uint16_t fpsCount;
        uint16_t fps;
        uint32_t timeStamp;

        View view;

        // a variable to signal new data from any source? usb/serial/sd
        bool gotNewData;
        bool gotFullArtnet;

        void (*updateCallback)();
        void setCallback(void (*callback)())
        { updateCallback = callback; }

    private:
        Network network;
        void checkUdpForOSC();
        void checkWebsocket();
        void checkArtnet();

        bool flasher;
        void pushConfig();
        void pushConfigFile(const char * _file);

        uint8_t stringMatcher(const char * _str);
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
