#include "Mode.h"

// static variables!
uint8_t Mode::brightness = 0;

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
    FastLED.setBrightness(map(pot1, 0, 1023, 0, 255));
    // all modes should call this?
}

void Mode::receiveCommand(uint8_t _cmd, uint8_t _val){
    if(_cmd == SET_BRIGHTNESS_CMD){
        brightness = _val;
        FastLED.setBrightness(brightness);
        // Serial.printf("brightness %i", brightness);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

DemoMode::DemoMode(){
    name = "demoMode";
}

void DemoMode::update(){
    Mode::update();
    float _speed = map(pot2, 0, 1023, 0, 20);
    int ha = frameCount/_speed;
    for(int i = 0; i < ledCount; i++){
        leds[i] = CHSV(ha+int(ha+i*2+millis()/10.0)%255,255,255);
    }
    newData = true;
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
    memset(universeToIndex, 0, sizeof(universeToIndex));
    // make map of univereses
    for(int i = 0; i < totalUniverseCount; i++){
        universeToIndex[i] = i * 170;
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
    else {
    }
}

void ArtNetMode::receivePacket(uint8_t * _data, uint8_t _sequence, uint16_t _universe, uint16_t _dataLenght){
    // bitSet(receivedUniverses, _universe);
    // bool _incomplete = false;
    // for(int i = 0; i < 3; i++){
    //     if(bitRead(receivedUniverses, i) == 0){
    //         _incomplete = true;
    //     }
    // }
    // if(!_incomplete){
    //     FastLED.show();
    //     receivedUniverses = 0;
    // }

    // check if new sequence, if yes output previous sequence
    // if(_universe == 3) _universe = 7;
    newData = false;
    if(_sequence != currentSequence){
        FastLED.show();
        newData = true;
        currentSequence = _sequence;
    }
    else {
    }
    if(_universe >= startUniverse && _universe < startUniverse+9){
        _universe -= startUniverse;
        for(int i = 0; i < _dataLenght/3; i++){
            leds[universeToIndex[_universe]+i] = CRGB(_data[i*3], _data[i*3+1], _data[i*3+2]);
        }
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
    newData = false;
    if(Serial.available()){
        int startChar = Serial.read();
        if (startChar == '*') {
            // int count = Serial.readBytes((char *)leds, ledCount*3);
            Serial.readBytes((char *)leds, ledCount*3);
            newData = true;
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
    int _test = map(pot2, 0, 1023, 0, 7);
    switch(_test){
        case 0:
            for(int y = 0 ; y < ledCount ; y++) leds[y] = CRGB(0,0,0);
            break;
        case 1:
            for(int y = 0 ; y < ledCount ; y++) leds[y] = CRGB(255,0,0);
            break;
        case 2:
            for(int y = 0 ; y < ledCount ; y++) leds[y] = CRGB(0,255,0);
            break;
        case 3:
            for(int y = 0 ; y < ledCount ; y++) leds[y] = CRGB(0,0,255);
            break;
        case 4:
            for(int y = 0 ; y < ledCount ; y++) leds[y] = (y%20 == (frameCount/2)%20 ? CRGB::White : CRGB::Black);
            break;
        case 5:
            for(int y = 0 ; y < ledCount ; y++) leds[y] = CRGB::White;
            break;
    }
    newData = true;
    FastLED.show();
}



////////////////////////////////////////////////////////////////////////////////////////////////

CustomMode::CustomMode(){
    name = "customMode";
}

void CustomMode::update(){
    // Mode::update();
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


////////////////////////////////////////////////////////////////////////////////////////////////

FunMode::FunMode(){
    name = "funMode";
    speed = 127;
    super = false;
    colorA = CRGB(255,0,0);
    flash = 0;
}

// void FunMode::update(){
//     Mode::update();
//     int ha = frameCount/speed;
//
//     for(int i = 0; i < ledCount; i++){
//         leds[i] = blend(colorA,CRGB(10,10,10), ha+int(ha+i*2+millis()/10.0)%255);
//     }
//     if(super){
//         for(int i = 0; i < 20; i++){
//             leds[(int)random(ledCount) % 20] = colorA;
//         }
//     }
//     if(flash){
//         --flash;
//         for(int i = 0; i < ledCount; i++){
//             leds[i] = CRGB::White;
//         }
//     }
//     newData = true;
//     FastLED.show();
// }
void FunMode::update(){

    Mode::update();
    int output = map(pot2, 0, 1023, 0, 7);

    for(int i = 0; i < ledCount; i++){
        if(i > output*340 && i < output*340+340){
            leds[i] = CRGB(10,10,10);
        }
        else {
            leds[i] = CRGB(0,0,0);

        }
    }

    FastLED.show();
    //
    // if(super){
    //     for(int i = 0; i < 20; i++){
    //         leds[(int)random(ledCount) % 20] = colorA;
    //     }
    // }
    // if(flash){
    //     --flash;
    //     for(int i = 0; i < ledCount; i++){
    //         leds[i] = CRGB::White;
    //     }
    // }
    // newData = true;
}
