#ifndef LrDMX_h
#define LrDMX_h

#include "Arduino.h"

#include "PinMap.h"
#include "Defaults.h"
#include "TeensyDmx.h"

class LrDMX {
    public :
        LrDMX();
        uint8_t mode; // output or input
        uint16_t channelCount;
        uint16_t deviceChannel;
        uint8_t universe;
        bool newData;
        // set the above first then call begin
        void setDmx(TeensyDmx * _dmx);
        void begin();
        void update(uint8_t * data);
        TeensyDmx * dmx;//(Serial1, DMX_REDE);
};


#endif
