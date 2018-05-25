/**
 * ##copyright##
 * See LICENSE.md
 *
 * @author    Maxime Damecour (http://nnvtn.ca)
 * @version   0.4
 * @since     2017-01-16
 */
 // FastLED
#define OCTOWSMODE 1
#if OCTOWSMODE
    #define USE_OCTOWS2811
    #include "OctoWS2811.h"
#endif
#include "FastLED.h"
#include "LEDriver.h"
#include "Ethernet.h"

#include "TeensyDmx.h"
TeensyDmx dmx_output_1(Serial1, 21);

LEDriver ledriver;

// things that cant be changed
#define NUM_LEDS_PER_STRIP 240
#define NUM_STRIPS 8
#define NUM_LEDS  NUM_STRIPS * NUM_LEDS_PER_STRIP
uint8_t generalBuffer[(NUM_LEDS*3) + (512*3)];
CRGB * leds;
// CRGB leds;
// CRGB leds[NUM_LEDS];
#define LED_TYPE    WS2812B
// could change? do ledriver_setup
#define COLOR_ORDER RGB//GRB

// data 1 pin for non octows user
#define DATA_PIN 3
#define CLOCK_PIN 2
EthernetServer serverForSocket(80);

void setup(){
    Serial.begin(115200);
    // step one retrieve config.
    // check for DMX modes,
    // strip mode, using octows or other, which should be specified.

    leds = (CRGB *)generalBuffer;
    // setup LEDs
    #if OCTOWSMODE
        LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
    #else
        FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);
    #endif
    FastLED.setDither( 0 );
    // set to black
    for(int y = 0 ; y < NUM_LEDS ; y++) leds[y] = CRGB::Black;
    FastLED.show();
    // initialise the system
    ledriver.begin(leds, NUM_LEDS);
    // set the callback
    ledriver.customMode.setCallback(custom);
    ledriver.setCallback(updateCallback);
    dmx_output_1.setMode(TeensyDmx::DMX_OUT);
}

void loop(){
    // runs once
    ledriver.runWithWebsocket(&serverForSocket);
    // otherwise :
    // ledriver.update();
}

void updateCallback(){
    if(ledriver.gotNewData){
        dmx_output_1.setChannels(0, (const uint8_t *)leds, 512);
    }
}


// custom mode's update method callback, add your own animation without much digging.
void custom(){
    for(int i = 0; i < NUM_LEDS; i++){
        // leds[i] = CHSV(pow((millis()%2000)/2000.0, 3)*255, 255, 255);
        leds[i] = CRGB(millis()/10 % 255,0,0);

    }
    FastLED.show();
}
