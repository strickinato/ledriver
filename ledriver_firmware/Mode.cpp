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
    // all modes should call this?
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

void ArtNetMode::setup(){
    // setup
    // default setup -> map LEDs to continuous univereses;
    totalUniverseCount = ledCount / 170;
    receivedUniverses = 0;
    uint8_t _startUniverse = 1;
    memset(universeToIndex, 0, sizeof(universeToIndex));
    // make map of univereses
    for(int i = 0; i < totalUniverseCount; i++){
        universeToIndex[i] = i*170;
    }
    // calculate value of all universe received
    for(int i = 0; i < totalUniverseCount; i++){
        bitSet(receivedUniverses, i);
    }
    gotAllUniverses = receivedUniverses;
}

void ArtNetMode::update(){
    Mode::update();
    if(receivedUniverses == gotAllUniverses){
        // do output
        FastLED.show();
        receivedUniverses = 0;
    }
}

void ArtNetMode::receivePacket(uint8_t * _data, uint8_t _sequence, uint16_t _universe, uint16_t _dataLenght){
    bitSet(receivedUniverses, _universe);
    // check if new sequence, if yes output previous sequence
    if(_sequence != currentSequence){
        FastLED.show();
        currentSequence = _sequence;
    }
    for(int i = 0; i < _dataLenght/3; i++){
        leds[universeToIndex[_universe]+i] = CRGB(_data[i*3], _data[i*3+1], _data[i*3+2]);
    }
    // view.printf("R %03u, G %03u, B %03u \n", _data[0], network.artnetData[1], network.artnetData[2]);
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
            // int count = Serial.readBytes((char *)leds, ledCount*3);
            Serial.readBytes((char *)leds, ledCount*3);
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

////////////////////////////////////////////////////////////////////////////////////////////////

SDCardPlaybackMode::SDCardPlaybackMode(){
    name = "SDCardPlaybackMode";
    animationNumber = 0;
}

void SDCardPlaybackMode::playAnimation(uint8_t _animation){
    char _fileName[11];
    sprintf(_fileName, "ani_%02d.bin", animationNumber);
    // Serial.println(fileName);
    currentFile = SD.open(_fileName);
}

void SDCardPlaybackMode::update(){
    // if (currentFile) {
    //     byte _header[HEADER_SIZE];
    //     currentFile.readBytes(_header, HEADER_SIZE);
    //     uint16_t _fileBufferSize = ((_header[0] << 8) | (_header[1] & 0xFF));
    //     if(_fileBufferSize > BUFFER_SIZE){
    //         Serial.println(F("Not enough LEDs to play animation"));
    //         updateOtherThings();
    //         delay(500);
    //     }
    //     else {
    //         // read from the file until there's nothing else in it:
    //         while (currentFile.available()) {
    //             currentFile.readBytes((char*)leds, _fileBufferSize);
    //             doShow();
    //             #if SPEED_POT_PIN
    //                 delay(analogRead(SPEED_POT_PIN)/30);
    //             #else
    //                 delay(20);
    //             #endif
    //             if(updateOtherThings()) break;
    //         }
    //     }
    //     currentFile.close();
    // }
    // else {
    //     Serial.print(F("error opening "));
    //     Serial.println(fileName);
    //     animationNumber = 0;
    //     delay(20);
    // }
}
