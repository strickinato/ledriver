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
LEDriver ledriver;


#define NUM_LEDS_PER_STRIP 410
#define NUM_STRIPS 8
#define NUM_LEDS  NUM_STRIPS * NUM_LEDS_PER_STRIP
// led CRGB setup
CRGB leds[NUM_LEDS];
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB//GRB

#define DATA_PIN 3
#define CLOCK_PIN 2

void setup(){
    Serial.begin(115200);

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
}

void loop(){
    ledriver.update();
}

// custom mode's update method callback, add your own animation without much digging.
void custom(){

    for(int i = 0; i < NUM_LEDS; i++){
        leds[i] = CHSV(pow(sin(i/4.5+millis()/5000.0), 3)*255, 100, 100);
    }
    FastLED.show();
}
