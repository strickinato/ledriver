#include "Mode.h"

// static variables!
uint8_t Mode::buttonPress = 0;
uint16_t Mode::pot1 = 0;
uint16_t Mode::pot2 = 0;
uint16_t Mode::frameCount = 0;
uint16_t Mode::ledCount = 0;
CRGB *Mode::leds = 0;

Mode::Mode(){
    name = "mode";
}

void Mode::update(){

}

////////////////////////////////////////////////////////////////////////////////////////////////

DemoMode::DemoMode(){
    name = "demoMode";
}

void DemoMode::update(){
    Mode::update();
    FastLED.setBrightness(map(pot1, 1023, 0, 0, 255));
    float _speed = map(pot2, 1023, 0, 0, 20);
    int ha = frameCount/_speed;
    for(int i = 0; i < ledCount; i++){
        leds[i] = CHSV(ha+int(ha+i*2+millis()/10.0)%255,255,255);
    }
    FastLED.show();
}

////////////////////////////////////////////////////////////////////////////////////////////////

ArtNetMode::ArtNetMode(){
    name = "artnetMode";
}

void ArtNetMode::update(){
    Mode::update();
    if(network.checkArtnet()){
        // view.printf("got artnet packet\n");
        // for(int i = 0; i < 8; i++){
        //     leds[i] = CRGB(network.artnetData[i*3], network.artnetData[i*3+1], network.artnetData[i*3]+2);
        // }
        // FastLED.show();
        // view.printf("R %03u, G %03u, B %03u \n", network.artnetData[0], network.artnetData[1], network.artnetData[2]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

SerialMode::SerialMode(){
    name = "serialMode";
    errorCount = 0;
}

void SerialMode::update(){
    Mode::update();
    if(Serial.available()){
        int startChar = Serial.read();
        if (startChar == '*') {
            int count = Serial.readBytes((char *)leds, ledCount*3);
            FastLED.show();
        }
        else if (startChar == '?') {
            Serial.print(ledCount);
            while(Serial.available()) Serial.read();
        } else if (startChar >= 0) {
            Serial.print("badheader ");
            Serial.println(errorCount++);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

TestMode::TestMode(){
    name = "testMode";
}

void TestMode::update(){
    Mode::update();

}

////////////////////////////////////////////////////////////////////////////////////////////////

CustomMode::CustomMode(){
    name = "customMode";
}

void CustomMode::update(){
    Mode::update();
    customUpdate();
}
