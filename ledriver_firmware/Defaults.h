#ifndef Defaults_h
#define Defaults_h

#define OCTOWSMODE 1
#if OCTOWSMODE
    #define USE_OCTOWS2811
    #include "OctoWS2811.h"
#endif

#include "FastLED.h"
// CRGB leds;
#define DEFAULT_LED_TYPE    WS2812B
// could change? do ledriver_setup
#define DEAFULT_COLOR_ORDER RGB//GRB
// data 1 pin for non octows user
#define DATA_PIN 2
#define CLOCK_PIN 14


// This is is set to 3000 because the js client for websocket mode
// Sends a JSON stringified array that is close to that size
#define JSON_BUFFER_SIZE 3000

#define DHCP_DEFAULT 1
// static const uint8_t DEFAULT_STATIC_IP[4] = {10,0,0,42};
const char DEFAULT_STATIC_IP[] = "10.0.0.41";
const char CONFIG_FILE[] = "config.jso";
const char LED_CONFIG_FILE[] = "ledcfg.jso";
const char DMX_CONFIG_FILE[] = "dmxcfg.jso";


#define LR_OSC_PORTIN 4200
#define STARTUP_TEST_DEFAULT 1

static uint8_t debug_level = 2;


enum Modes{
    DEMO_MODE,
    ARTNET_MODE,
    SERIAL_MODE,
    TEST_MODE,
    CUSTOM_MODE,
    SDPLAY_MODE,
    FUN_MODE,
    WEBSOCKET_CONTROL_MODE
};

enum Outputs{
    NONE_OUTPUT,
    DMX_OUT,
    DMX_IN,
    FAST_OCTO,
    FAST_WS2801,
    APA_102
};


#endif
