#include "LrDMX.h"


LrDMX::LrDMX(){
    mode = NONE_OUTPUT;
    channelCount = 512;
    deviceChannel = 0;
    universe = 1;
}

void LrDMX::setDmx(TeensyDmx * _dmx){
    dmx = _dmx;
}

void LrDMX::begin(){
    if(mode == DMX_OUT){
        dmx->setMode(TeensyDmx::DMX_OUT);
    }
    else if(mode == DMX_IN){
        dmx->setMode(TeensyDmx::DMX_IN);
    }
}

void LrDMX::output(uint8_t * data){
    if(mode == DMX_OUT){
        dmx->setChannels(0, data, channelCount);
    }
}
