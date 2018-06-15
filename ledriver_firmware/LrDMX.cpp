#include "LrDMX.h"


LrDMX::LrDMX(){
    mode = NONE_OUTPUT;
    channelCount = 512;
    deviceChannel = 0;
    universe = 1;
    newData = false;
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

// either grabs data or outputs data to the passed buffer
void LrDMX::update(uint8_t * data){
    if(mode == DMX_IN){
        dmx->loop();
        if(dmx->newFrame()) {
            newData = true;
            memcpy(data, (uint8_t *) dmx->getBuffer()[deviceChannel], channelCount);
        }
        else newData = false;
    }
    else if(mode == DMX_OUT){
        dmx->setChannels(0, data, channelCount);
    }
}
