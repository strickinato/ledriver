#include "LEDriver.h"



LEDriver::LEDriver(){
    modePointers[0] = &demoMode;
    modePointers[1] = &artnetMode;
    modePointers[2] = &serialMode;
    modePointers[3] = &testMode;
}

void LEDriver::begin(CRGB * _leds, uint16_t _count){
    Mode::leds = _leds;
    Mode::ledCount = _count;
    pinMode(STATUS_LED_PIN, OUTPUT);
    view.begin();
    view.println(F("ledriver V0.0000001"));
    view.println(F("init SDcard"));
    enableSDcard();
    if(!SD.begin(SDCARD_CS_PIN)){
        view.println(F("no SDcard..."));
    }
    if (SD.exists("config.ldr")) {
          // The open configuration file.
        SDConfigFile cfg;
        if(cfg.begin("config.ldr", 64)){
            view.println(F("parsing config.ldr"));
            while (cfg.readNextSetting()) {
                if (cfg.nameIs("name")) {
                    view.printf("name %s\n", cfg.getValue());
                }
                else if(cfg.nameIs("dhcp")){
                    network.useDHCP = cfg.getIntValue();
                    view.printf("dhcp %i \n", network.useDHCP);
                }
                else if(cfg.nameIs("mac")){
                    // memcpy(&network.mac, cfg.getMacAddressValue(), 7);
                    // view.printf("mac %x-%x-%x-%x-%x-%x \n", network.mac[0], network.mac[1], network.mac[2], network.mac[3], network.mac[4], network.mac[5], network.mac[6] );
                }
                else if(cfg.nameIs("ip")){
                    const char *_ip = cfg.getValue();
                    if(network.setIp(_ip)){
                        view.printf("static ip %s \n", _ip);
                    }
                    else {
                        view.printf("not valid ip %s \n", _ip);
                    }
                }
            }
        }
        else {
            view.println(F("no file config.ldr"));
        }
    }
    view.println(F("init network"));
    network.begin();
    if(network.useDHCP){
        view.printf("DHCP %i.%i.%i.%i \n", network.ip[0], network.ip[1], network.ip[2], network.ip[3]);
    }
    else view.print(F("STATIC "));
    frameCount = 0;
    currentMode = 0;
    view.println(modePointers[currentMode]->name);

}


void LEDriver::update(){
    checkInput();
    frameCount++;
    Mode::frameCount = frameCount;
    // currentMode = DEMO_MODE;
    modePointers[currentMode]->update();
}

void LEDriver::enableSDcard(){
    digitalWrite(WIZNET_CS_PIN, HIGH);
    digitalWrite(SDCARD_CS_PIN, LOW);
}

void LEDriver::enableWiznet(){
    digitalWrite(SDCARD_CS_PIN, HIGH);
    digitalWrite(WIZNET_CS_PIN, LOW);
}


void LEDriver::checkInput(){
    // make a value changed system to then enable event based actions?
    buttonPress = 0;// reset the input state
    if(frameCount % 10 == 0){
        pot1_value = analogRead(POT1_PIN);
        pot2_value = analogRead(POT2_PIN);
        Mode::pot1 = pot1_value;
        Mode::pot2 = pot2_value;
        button_value = analogRead(BUTTON_PIN);
        button_value = analogRead(BUTTON_PIN);
        button1 = false;
        button2 = false;
        button3 = false;
        uint8_t _prev = buttonState;
        buttonState = 0;
        if(button_value > 900) {
            button1 = true;
            buttonState = 1;
        }
        else if(button_value > 800) {
            button2 = true;
            buttonState = 2;
        }
        else if(button_value > 700) {
            button3 = true;
            buttonState = 3;
        }
        if(buttonState != 0){
            if(_prev != buttonState){
                buttonPress = buttonState;
            }
        }
        // if(frameCount%1000 == 0){
        //     view.oled.printf("%d, %d, %d  \n", pot1_value, pot2_value, button_value);
        // }
    }
    Mode::buttonPress = buttonPress;
    if(buttonPress != 0){
        view.printf("pressed %i\n", buttonPress);
    }
}
//
//
// void LEDriver::standbyMode(){
//     digitalWrite(STATUS_LED_PIN, millis()%500 < 60);
//     // modeSelector
//     if(buttonPress != 0){
//
//     }
//     if(network.checkArtnet()){
//         // view.printf("got artnet packet\n");
//         for(int i = 0; i < 8; i++){
//             leds[i] = CRGB(network.artnetData[i*3], network.artnetData[i*3+1], network.artnetData[i*3]+2);
//         }
//         FastLED.show();
//         view.printf("R %03u, G %03u, B %03u \n", network.artnetData[0], network.artnetData[1], network.artnetData[2]);
//     }
// }
//
// void LEDriver::sdCardPlaybackMode(){
//     digitalWrite(STATUS_LED_PIN, millis()%500 < 60);
// }
//
// void LEDriver::serialMode(){
//     digitalWrite(STATUS_LED_PIN, millis()%500 < 60);
// }
//
// void LEDriver::artnetMode(){
//     digitalWrite(STATUS_LED_PIN, millis()%500 < 60);
// }
//
// void LEDriver::demoMode(){
//     digitalWrite(STATUS_LED_PIN, millis()%100 < 60);
//     int ha = frameCount/10;
//     for(int i = 0; i < NUM_LEDS; i++){
//         leds[i] = CHSV(ha+int(ha+i*2+millis()/100.0)%255,255,255);
//     }
//     FastLED.show();
// }
//
// void LEDriver::testMode(){
//     digitalWrite(STATUS_LED_PIN, millis()%500 < 60);
// }

// void standby(){

//
// }

// void serialMode(){
//     if(Serial.available()){
//         int startChar = Serial.read();
//         if (startChar == '*') {
//             int count = Serial.readBytes((char *)leds, BUFFER_SIZE);
//             FastLED.show();
//         }
//         else if (startChar == '?') {
//             Serial.print(NUM_LEDS);
//             while(Serial.available()) Serial.read();
//         } else if (startChar >= 0) {
//             Serial.print("badheader ");
//             Serial.println(errorCount++);
//         }
//     }
// }





// // play animation from SD card
// void playAnimationFromSD(){
//     sprintf(fileName, "ani_%02d.bin", animationNumber);
//     Serial.println(fileName);
//     myFile = SD.open(fileName);
//
//     if (myFile) {
//         byte _header[HEADER_SIZE];
//         myFile.readBytes(_header, HEADER_SIZE);
//         uint16_t _fileBufferSize = ((_header[0] << 8) | (_header[1] & 0xFF));
//         if(_fileBufferSize > BUFFER_SIZE){
//             Serial.println(F("Not enough LEDs to play animation"));
//             updateOtherThings();
//             delay(500);
//         }
//         else {
//             // read from the file until there's nothing else in it:
//             while (myFile.available()) {
//                 myFile.readBytes((char*)leds, _fileBufferSize);
//                 doShow();
//                 #if SPEED_POT_PIN
//                     delay(analogRead(SPEED_POT_PIN)/30);
//                 #else
//                     delay(20);
//                 #endif
//                 if(updateOtherThings()) break;
//             }
//         }
//         myFile.close();
//     }
//     else {
//         Serial.print(F("error opening "));
//         Serial.println(fileName);
//         animationNumber = 0;
//         delay(20);
//     }
// }
