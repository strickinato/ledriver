#include "LrBuffer.h"

LrBuffer::LrBuffer(){
    memset(drawBuffer, 0,  sizeof(drawBuffer));
    ledCount = LR_NUM_LEDS;
    ledsEnabled = false;
}

void LrBuffer::begin(){
    leds = (CRGB *)drawBuffer;
    // #if OCTOWSMODE
    //     LEDS.addLeds<OCTOWS2811>(leds, LR_LED_PER_STRIP);
    // #else
    //     FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, LR_NUM_LEDS);
    // #endif
    // FastLED.setDither( 0 );
    // FastLED.show();
}

void LrBuffer::enableFastLED(){//uint8_t _mode){
    ledsEnabled = true;
    #if OCTOWSMODE
        LEDS.addLeds<OCTOWS2811>(leds, LR_LED_PER_STRIP);
    #else
        FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, LR_NUM_LEDS);
    #endif
    FastLED.setDither( 0 );
    FastLED.show();
}

CRGB * LrBuffer::getLEDs(){
    return leds;
}

uint8_t * LrBuffer::getDMX(uint8_t _u){
    if(_u == 0){
        return drawBuffer + LR_NUM_LEDS*3;
    }
    else {
        return drawBuffer + LR_NUM_LEDS*3 + 512;
    }
}

void LrBuffer::show(){
    if(ledsEnabled){
        FastLED.show();
    }
}
