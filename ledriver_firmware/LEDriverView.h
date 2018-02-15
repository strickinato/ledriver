#ifndef LEDriverView_h
#define LEDriverView_h

#include "Wire.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"


#define I2C_ADDRESS 0x3C

class LEDriverView : public Print {
    public:
        LEDriverView();
        void begin();
        size_t write(uint8_t c);
        SSD1306AsciiWire oled;
        bool useOled;
        bool useSerial;
    private:

};

#endif
