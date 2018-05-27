
#include "LEDriver.h"

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

#define NO_POTS 0

LEDriver::LEDriver(){
    modePointers[DEMO_MODE] = &demoMode;
    modePointers[ARTNET_MODE] = &artnetMode;
    modePointers[SERIAL_MODE] = &serialMode;
    modePointers[TEST_MODE] = &testMode;
    modePointers[CUSTOM_MODE] = &customMode;
    modePointers[SDPLAY_MODE] = &sdPlaybackMode;
    dataBuffer.begin();
}

void LEDriver::begin(){
    // serverForSocket.begin();
    // dataBuffer.begin();
    Mode::leds = dataBuffer.getLEDs();
    Mode::ledCount = dataBuffer.ledCount;
    pinMode(STATUS_LED_PIN, OUTPUT);

    artnetMode.setup();

    view.begin();
    view.printf("ledriver V%f\n", VERSION);

    enableSDcard();
    if(!SD.begin(SDCARD_CS_PIN)){
        view.println(F("no SDcard..."));
    }
    else {
        parseConfig("config.ldr");
    }

    if(debug_level > 0) view.println(F("- init network"));

    // init network, may take a moment
    network.useDHCP = false;
    network.begin();
    // print resulting network information
    if(debug_level > 0){
        if(network.useDHCP){
            view.printf("DHCP %i.%i.%i.%i \n", network.ip[0], network.ip[1], network.ip[2], network.ip[3]);
        }
        else {
            view.printf("STATIC %i.%i.%i.%i \n", network.ip[0], network.ip[1], network.ip[2], network.ip[3]);
        }
    }

    frameCount = 0;
    fpsCount     = 0;
    timeStamp = millis();
    setMode(DEMO_MODE);//ARTNET_MODE);//MO_MODE);
    // setMode(DEMO_MODE);//ARTNET_MODE);//MO_MODE);
    // receiveCommand(SET_BRIGHTNESS_CMD, 128);
}


void LEDriver::runWithWebsocket(EthernetServer * serverForSocket){
    EthernetClient client = serverForSocket->available();
    if(client.connected() && webSocketServer.handshake(client)){
        if(debug_level > 0) view.println(F("- ws cnnctn"));
        while(client.connected()){
            String data;
            data = webSocketServer.getData();

            int _len = data.length() + 1;
            char _array[_len];
            data.toCharArray(_array, _len);
            // todo? implement websocket as a Stream?? support printf and such
            if(data.length() > 0){
                receiveOSC((uint8_t *)_array, _len);
                // if(debug_level > 1) view.println(data);

                // String haha = "fps ";
                // haha += fps;
                // webSocketServer.sendData(haha);
            }
            // call the regular update
            update();
        }
        if(debug_level > 0) view.println(F("! ws dscnct"));
        delay(100);
        client.stop();
    }
    else {
        update();
    }
}


void LEDriver::receiveOSC(uint8_t * _mess, uint8_t _sz){
    OSCMessage messageIn;
    messageIn.fill(_mess, _sz);

    if(!messageIn.hasError()){
        // view.printf("not match %i \n", messageIn.getInt(0));
        if(messageIn.match("/a/")){
            view.printf("set bright %i \n", messageIn.getInt(0));
        }
    }
    else {
        if(debug_level > 0) view.printf("osc error : %i \n",messageIn.getError());
    }
}



bool flasher = false;
// kind of the main loop
void LEDriver::update(){

    updateCallback();
    if(frameCount % 4 == 1){
        digitalWrite(STATUS_LED_PIN, flasher);
        flasher = !flasher;
    }
    checkInput();
    frameCount++;
    fpsCount++;
    if(millis() - timeStamp > 1000){
        timeStamp = millis();
        fps = fpsCount;
        // view.printf("fps %i \n", fpsCount);
        fpsCount = 0;
    }
    Mode::frameCount = frameCount;
    // check for serial connection
    if(currentMode != SERIAL_MODE){
        // auto detect Serial mode
        // if(Serial.available()){
        //     view.useSerial = false;
        //     setMode(SERIAL_MODE);
        // }
    }

    gotNewData = false;
    if(currentMode == ARTNET_MODE){
        if(buttonPress == 1){
        }
        if(network.checkArtnet()){
            gotNewData = true;
            // Serial.printf("u = %i s = %i \n",network.incomingUniverse, network.sequence);
            artnetMode.receivePacket(network.artnetData, network.sequence, network.incomingUniverse, network.dmxDataLength);
        }
    }

    // interpret input
    if(buttonPress == 1) receiveCommand(SET_MODE_CMD, currentMode+1);
    else if(buttonPress == 3) receiveCommand(SET_MODE_CMD, currentMode-1);

    // update Mode
    modePointers[currentMode]->update();

}





void LEDriver::receiveCommand(uint8_t _cmd, uint8_t _val){
    // view.printf("received commend %i %i", _cmd, _val);
    switch(_cmd){
        case SET_MODE_CMD:
            setMode(_val);
            break;
        default :
            modePointers[currentMode]->receiveCommand(_cmd, _val);
        // case
    }
}


void LEDriver::parseConfig(const char * _file){
    enableSDcard();
    if (SD.exists(_file)) {
        SDConfigFile cfg;
        if(cfg.begin(_file, 64)){
            view.printf("Parsing %s \n", _file);
            while (cfg.readNextSetting()) {
                if (cfg.nameIs("name")) {
                    view.printf("name %s\n", cfg.getValue());
                }
                else if(cfg.nameIs("dhcp")){
                    network.useDHCP = cfg.getIntValue();
                    view.printf("dhcp %i \n", network.useDHCP);
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
            view.println(F("config.ldr NOT FOUND"));
        }
    }
}

void LEDriver::setMode(uint8_t _mode){
    if(_mode >= MODE_COUNT) currentMode = MODE_COUNT-1;
    else if(_mode < 0) currentMode = 0;
    else currentMode = _mode;
    view.printf("set to %s \n", modePointers[currentMode]->name);
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
    if(lastInputCheck < millis() - INPUT_POLL_RATE){
        lastInputCheck = millis();
        pot1_value = analogRead(POT1_PIN);
        pot2_value = analogRead(POT2_PIN);
        Mode::pot1 = pot1_value;
        Mode::pot2 = pot2_value;
        #if NO_POTS
        Mode::pot1 = 200;
        Mode::pot2 = 10;
        #endif
        analogRead(BUTTON_PIN);
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

    }
    //Mode::buttonPress = buttonPress;
    // if(buttonPress != 0){
    //     view.printf("pressed %i\n", buttonPress);
    // }
}
