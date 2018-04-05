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
//
// #include "WebSocketsServer.h"
// WebSocketsServer webSocket = WebSocketsServer(81);


LEDriver ledriver;

#define NUM_LEDS_PER_STRIP 170
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
    //
    // webSocket.begin();
    // webSocket.onEvent(receivePacket);

}

void loop(){
    ledriver.update();
}

// custom mode's update method callback, add your own animation without much digging.
void custom(){
    for(int i = 0; i < NUM_LEDS; i++){
        // leds[i] = CHSV(pow((millis()%2000)/2000.0, 3)*255, 255, 255);
        leds[i] = CRGB(millis()/10 % 255,0,0);

    }
    FastLED.show();
}















//
//
// void sendPacket() {
//     webSocket.broadcastTXT("hahahahi", 8);
// }
//
// void receivePacket(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
//     Serial.printf("ws packet %i \n", length);
//     // Serial.println(payload);
//     switch (type) {
//     case WStype_DISCONNECTED: // DISCONNECTION MESSAGE.
//         Serial.printf("[%u] Disconnected!\n", num);
//         break;
//     case WStype_CONNECTED: // CONNECTION MESSAGE.
//     {
//         IPAddress ip = webSocket.remoteIP(num);
//         Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
//     }
//     break;
//     case WStype_TEXT: // TEXT TO BE PARSED BY AsciiMassageParser.
//         Serial.printf("[%u] got Text: %s\n", num, payload);
//
//         // for ( int i = 0 ; i < length; i++ ) {
//         //     if ( inbound.parse( payload[i] ) ) {
//         //         if ( inbound.fullMatch("d") ) {
//         //             digitalWrite(debugLedPin, inbound.nextInt() );
//         //         } else if ( inbound.fullMatch("ms") ) {
//         //             sendMs = inbound.nextInt();
//         //         } else if ( inbound.fullMatch("a0") ) {
//         //             sendAnalog = inbound.nextInt();
//         //         } else {
//         //             outbound.packEmpty("what?");
//         //             sendPacket();
//         //         }
//         //     }
//         // }
//
//         break;
//     }
// }
