#ifndef Defaults_h
#define Defaults_h

#define OCTOWSMODE 1
#if OCTOWSMODE
    #define USE_OCTOWS2811
    #include "OctoWS2811.h"
#endif

#include "FastLED.h"
// CRGB leds;
#define LED_TYPE    WS2812B
// could change? do ledriver_setup
#define COLOR_ORDER RGB//GRB
// data 1 pin for non octows user
#define DATA_PIN 3
#define CLOCK_PIN 2




#define DHCP_DEFAULT 1
// static const uint8_t DEFAULT_STATIC_IP[4] = {10,0,0,42};
const char DEFAULT_STATIC_IP[] = "10.0.0.41";
const char CONFIG_FILE[] = "config.jso";
const char LED_CONFIG_FILE[] = "ledcfg.jso";
const char DMX_CONFIG_FILE[] = "dmxcfg.jso";


#define LR_OSC_PORTIN 4200
#define STARTUP_TEST_DEFAULT 1

static uint8_t debug_level = 2;

#endif
