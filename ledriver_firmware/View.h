#ifndef View_h
#define View_h

#include "Wire.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"


#define I2C_ADDRESS 0x3C

class View : public Print {
    public:
        View();
        void begin();
        size_t write(uint8_t c);
        SSD1306AsciiWire oled;
        bool useOled;
        bool useSerial;
    private:

};

#endif
