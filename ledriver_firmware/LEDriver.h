
#ifndef LEDriver_h
#define LEDriver_h

#include "Arduino.h"
#include "LEDriverView.h"
#include "Network.h"
#include "PinMap.h"
#include "SD.h"
#include "SPI.h"


enum Modes {STANDBY_MODE, SDCARD_MODE, SERIAL_MODE, ARTNET_MODE};

class LEDriver {
    public:
        LEDriver();
        void begin();
        void update();

        uint8_t currentMode = STANDBY_MODE;

        void checkInput();

    private:
        Network network;
        LEDriverView view;
        File configFile;

        uint16_t frameCount;

        uint16_t pot1_value;
        uint16_t pot2_value;
        uint16_t button_value;

        bool button1;
        bool button2;
        bool button3;

};





#endif
