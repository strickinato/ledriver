/**
 * ##copyright##
 * See LICENSE.md
 *
 * @author    Maxime Damecour (http://nnvtn.ca)
 * @version   0.4
 * @since     2017-01-16
 */
 // FastLED




#include "LEDriver.h"
// #include "Ethernet.h"

// #include "TeensyDmx.h"
// TeensyDmx dmx_output_1(Serial1, 21);

LEDriver ledriver;

EthernetServer serverForSocket(80);

void setup(){
    Serial.begin(115200);
    ledriver.begin();
    // initialise the system
    // set the callback
    ledriver.customMode.setCallback(custom);
    ledriver.setCallback(updateCallback);
    // dmx_output_1.setMode(TeensyDmx::DMX_OUT);
}

void loop(){
    // runs once
    ledriver.runWithWebsocket(&serverForSocket);
    // otherwise :
    // ledriver.update();
}

void updateCallback(){
    if(ledriver.gotNewData){
        // dmx_output_1.setChannels(0, (const uint8_t *)leds, 512);
    }
}


// custom mode's update method callback, add your own animation without much digging.
void custom(){
    for(int i = 0; i < 16; i++){
        // leds[i] = CHSV(pow((millis()%2000)/2000.0, 3)*255, 255, 255);
        // leds[i] = CRGB(millis()/10 % 255,0,0);
    }
    FastLED.show();
}
