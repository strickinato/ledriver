
// #define BOARD_REV 002
#define BOARD_REV 003
#include "LEDriver.h"

#define NO_POTS 0

LEDriver::LEDriver(){
    modePointers[DEMO_MODE] = &demoMode;
    modePointers[ARTNET_MODE] = &artnetMode;
    modePointers[SERIAL_MODE] = &serialMode;
    modePointers[TEST_MODE] = &testMode;
    modePointers[CUSTOM_MODE] = &customMode;
    modePointers[SDPLAY_MODE] = &sdPlaybackMode;
}

void LEDriver::begin(){
    dataBuffer.begin();
    view.begin();
    view.printf("ledriver V%f\n", VERSION);
    // serverForSocket.begin();

    // setup buffer
    leds = dataBuffer.getLEDs();
    Mode::leds = dataBuffer.getLEDs();
    Mode::ledCount = dataBuffer.ledCount;

    // setup some pins
    pinMode(STATUS_LED_PIN, OUTPUT);

    // setup modes
    artnetMode.setup();

    // fetch config
    // enableSDcard();
    if(!SD.begin(SDCARD_CS_PIN)){
        view.println(F("no SDcard..."));
    }
    else {
        loadConfigFile(CONFIG_FILE);
    }

    network.reset();
    view.println("reset");
    if(debug_level > 0) view.println(F("- init network"));
    view.println(network.useDHCP);
    // init network, may take a moment
    // network.useDHCP = false;
    network.begin();
    // print resulting network information
    if(debug_level > 0){
        if(network.useDHCP){
            // view.printf("DHCP %i.%i.%i.%i \n", network.ip[0], network.ip[1], network.ip[2], network.ip[3]);
            view.print("DHCP ");
            view.println(network.ipAddress);
        }
        else {
            // view.printf("STATIC %i.%i.%i.%i \n", network.ip[0], network.ip[1], network.ip[2], network.ip[3]);
            view.print("STATIC ");
            view.println(network.ipAddress);
        }
    }

    // SD.exists("config.jso");

    frameCount = 0;
    fpsCount     = 0;
    timeStamp = millis();\
    setMode(DEMO_MODE);//ARTNET_MODE);//MO_MODE);
    // setMode(DEMO_MODE);//ARTNET_MODE);//MO_MODE);
    // receiveCommand(SET_BRIGHTNESS_CMD, 128);


    // void * _func = &LEDriver::onData;
    // webSocketServer.registerDataCallback(_func);
}

///////////////////////////////////////////////////////////////////////////////
// Messaging
///////////////////////////////////////////////////////////////////////////////


void LEDriver::runWithWebsocket(EthernetServer * serverForSocket){
    EthernetClient client = serverForSocket->available();
    if(client.connected() && webSocketServer.handshake(client)){
        if(debug_level > 0) view.println(F("- ws cnnctn"));
        // push config :
        pushConfig();
        while(client.connected()){
            checkUdpForOSC();
            checkWebsocket();
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

void LEDriver::checkUdpForOSC(){
    int _sz = network.oscUDP.parsePacket();
    if(_sz > 0){
        char _array[_sz];
        network.oscUDP.read(_array, _sz);
        receiveOSC((uint8_t *)_array, _sz);
    }
}

void LEDriver::checkWebsocket(){
    String _data = webSocketServer.getData();
    if(_data.length()> 0){
        if(debug_level > 1) view.println(_data);
        receiveJson(_data.c_str());
        webSocketServer.sendData("ahhaha");
    }
}

void LEDriver::receiveJson(const char * _received){
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(_received);
    if(root.success()){
        if(debug_level > 1){
            view.println("- receivedJson");
            root.printTo(view);
            if(root.containsKey("config")){
                saveConfigFile(root, CONFIG_FILE);
            }
        }
    }
}

void LEDriver::receiveOSC(uint8_t * _mess, uint8_t _sz){
    OSCMessage messageIn;
    messageIn.fill(_mess, _sz);

    if(!messageIn.hasError()){
        if(messageIn.match("/record")){
            // view.printf("set bright %i \n", messageIn.getString(0));
        }
        else {
            view.printf("not match %i \n", messageIn.getInt(0));
        }
    }
    else {
        if(debug_level > 0) view.printf("osc error : %i \n",messageIn.getError());
    }
}

void sendFPS(){
    // OSCMessage msg("/fps");
    // msg.add((int32_t) fps);
}

///////////////////////////////////////////////////////////////////////////////
// command parsing
///////////////////////////////////////////////////////////////////////////////

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


///////////////////////////////////////////////////////////////////////////////
// Update
///////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////
// CONFIG
///////////////////////////////////////////////////////////////////////////////

void LEDriver::makeConfig(){
    // remove file or config will be appended
    // enableSDcard();
    SD.remove(CONFIG_FILE);
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonObject& config = root.createNestedObject("config");
    config["DHCP"] = 0;
    config["ip"] = "10.0.0.42";
    config["name"] = "majestic";
    config["startupTest"] = 0;

}

void LEDriver::saveConfigFile(JsonObject &root, const char * _fileName){
    if(SD.exists(_fileName)){
        SD.remove(_fileName);
    }
    File _file = SD.open(_fileName, FILE_WRITE);
    if(_file){
        if(root.printTo(_file) == 0){
            view.println(F("failed to write file"));
        }
    }
    else {
        view.println(F("failed to make file"));
    }
    _file.close();
}

void LEDriver::loadConfigFile(const char * _fileName){
    if (SD.exists(_fileName)) {
        File _file = SD.open(_fileName);
        StaticJsonBuffer<512> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(_file);
        if(root.success()){
            if(root.containsKey("config")){
                parseJsonConfig(root["config"]);
            }
        }
        else {
            if(debug_level > 0) view.println("! problem w config");
            // retry
            // delay(200);
            // parseConfig(_fileName);
        }
    }
    else {
        if(debug_level > 0) view.printf("%s NOT FOUND \n", _fileName);
        // perhaps make prompt for making a new file?
        view.println(F("making a config file"));
        makeConfig();
    }
}

void LEDriver::parseJsonConfig(JsonObject &config){
    view.printf("name %s\n", config["name"] | "NO_NAME");
    // if(debug_level > 0) root.printTo(view);
    network.useDHCP = config["DHCP"] | 0;
    const char * _ip = config["ip"] | DEFAULT_STATIC_IP;
    network.setIp(_ip);
}
// in cpp code
// push the local configuration
void LEDriver::pushConfig(){
    // String _send = "ahah nope this is a string";
    // webSocketServer.sendData(_send);

    if (SD.exists("config.jso")) {
        view.println("haha");
        File _file = SD.open("config.jso");
        StaticJsonBuffer<512> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(_file);
        if(root.success()){
            String _send;
            root.printTo(_send);
            // root.printTo(view);

            // view.println(_send);
            webSocketServer.sendData(_send);
        }
        else {
            view.println("nope");
        }
    }
    else {
        view.println(F("no config file"));
        // makeConfig();
    }

}

///////////////////////////////////////////////////////////////////////////////
// Set
///////////////////////////////////////////////////////////////////////////////

void LEDriver::setMode(uint8_t _mode){
    if(_mode >= MODE_COUNT) currentMode = MODE_COUNT-1;
    else if(_mode < 0) currentMode = 0;
    else currentMode = _mode;
    view.printf("set to %s \n", modePointers[currentMode]->name);
}



///////////////////////////////////////////////////////////////////////////////
// Input
///////////////////////////////////////////////////////////////////////////////
#if BOARD_REV == 002
void LEDriver::checkInput(){
    // make a value changed system to then enable event based actions?
    buttonPress = 0;// reset the input state
    if(lastInputCheck < millis() - INPUT_POLL_RATE){
        lastInputCheck = millis();
        pot1_value = analogRead(POT1_PIN)-1023;
        pot2_value = analogRead(POT2_PIN)-1023;
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
#endif

#if BOARD_REV == 003
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
        button4 = false;
        // view.println(button_value);

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
        else if(button_value > 600) {
            button4 = true;
            buttonState = 4;
        }
        if(buttonState != 0){
            if(_prev != buttonState){
                buttonPress = buttonState;
                if(debug_level > 1) view.printf("- btn press %i\n", buttonPress);
            }
        }

    }
    //Mode::buttonPress = buttonPress;
    // if(buttonPress != 0){
    //     view.printf("pressed %i\n", buttonPress);
    // }
}
#endif
