#include "LEDriver.h"



LEDriver::LEDriver(){}


void LEDriver::begin(){
    view.begin();
    view.println(F("ledriver V0.0000001"));
    view.println(F("init SDcard"));
    if(!SD.begin(SDCARD_CS_PIN)){
        view.println(F("no SDcard..."));
    }
    if (SD.exists("config.ldr")) {
        view.println(F("parsing config.ldr"));
    }
    view.println(F("init network"));
    network.begin();
    if(network.useDHCP) view.print(F("DHCP "));
    else view.print(F("STATIC "));
    view.printf("%d.%d.%d.%d \n", network.ip[0], network.ip[1], network.ip[2], network.ip[3]);
    frameCount = 0;
}


void LEDriver::update(){
    checkInput();
    frameCount++;
    //
}


void LEDriver::checkInput(){
    // make a value changed system to then enable event based actions?
    pot1_value = analogRead(POT1_PIN);
    pot2_value = analogRead(POT2_PIN);
    button_value = analogRead(BUTTON_PIN);
    button_value = analogRead(BUTTON_PIN);
    button1 = false;
    button2 = false;
    button3 = false;
    if(button_value > 900) button1 = true;
    if(button_value > 800) button2 = true;
    if(button_value > 700) button3 = true;

    if(frameCount%1000 == 0){
        // view.oled.printf("%d, %d, %d  \n", pot1_value, pot2_value, button_value);
    }
}
