#include "Mode.h"


uint8_t Mode::buttonPress = 0;
uint16_t Mode::pot1 = 0;
uint16_t Mode::pot2 = 0;
CRGB *Mode::leds = 0;
uint16_t Mode::ledCount = 0;
uint16_t Mode::frameCount = 0;

Mode::Mode(){
    name = "mode";
}

////////////////////////////////////////////////////////////////////////////////////////////////

DemoMode::DemoMode(){
    name = "demoMode";

}

void DemoMode::update(){
    int ha = frameCount/10;
    FastLED.setBrightness(map(pot1, 1023, 0, 0, 255));
    float _speed = map(pot2, 1023, 0, 0, 255);
    for(int i = 0; i < ledCount; i++){
        leds[i] = CHSV(ha+int(ha+i*2+millis()/_speed)%255,255,255);
    }
    FastLED.show();
}

////////////////////////////////////////////////////////////////////////////////////////////////

ArtNetMode::ArtNetMode(){
    name = "artnetMode";

}

void ArtNetMode::update(){

}


////////////////////////////////////////////////////////////////////////////////////////////////

SerialMode::SerialMode(){
    name = "serialMode";

}

void SerialMode::update(){

}

////////////////////////////////////////////////////////////////////////////////////////////////

TestMode::TestMode(){
    name = "testMode";

}

void TestMode::update(){

}
