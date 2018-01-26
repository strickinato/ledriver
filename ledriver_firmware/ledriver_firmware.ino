/**
 * ##copyright##
 * See LICENSE.md
 *
 * @author    Maxime Damecour (http://nnvtn.ca)
 * @version   0.4
 * @since     2017-01-16
 */
#include "LEDriver.h"
LEDriver ledriver;

void setup(){
    ledriver.begin();
}

void loop(){
    ledriver.update();
}

//
//
// // Now the idea would be to support regular arduino/FastLED and teensy/OctoWS811 and SD card playback :) all in one.
// // Optional for OctoWS811
// #define OCTOWSMODE true
// #if OCTOWSMODE
//     #define USE_OCTOWS2811
//     #include<OctoWS2811.h>
// #endif
//
// // FastLED
// #include <FastLED.h>
// // Other libs
// #include <SD.h>
// #include <Bounce2.h>
//
// #include <Ethernet.h>
// #include <EthernetUdp.h>
//
//
// int mode = STANDBY;
// // ledCount, if using single output, set NUM_STRIPS to 1
// #define NUM_LEDS_PER_STRIP 80//170
// #define NUM_STRIPS 8
// #define NUM_LEDS  NUM_STRIPS * NUM_LEDS_PER_STRIP
//
// // fastLED settings
// #define BRIGHTNESS  100
// #define LED_TYPE    WS2812B
// #define COLOR_ORDER RGB//GRB
//
// /////////////////////////// Pin Definition
// // fastLED Pin settings
// #define DATA_PIN 6
// #define CLOCK_PIN 2
// // sdcard pins
// #define SD_CS 3 // 3 on other setups...
// //    pin 3:  SD Card, CS
// //    pin 11: SD Card, MOSI
// //    pin 12: SD Card, MISO
// //    pin 13: SD Card, SCLK
// #define WIZ_RESET 23
// #define WIZ_CS 4
// // input pins, if they are 0, then they will not be used.
// #define BUTTON_PIN 17
// #define SPEED_POT_PIN 4 // A4 D18
// #define DIM_POT_PIN 5 // A5 D19
//
// // led CRGB setup
// CRGB leds[NUM_LEDS];
// // serial in buffer
// const int BUFFER_SIZE = NUM_LEDS * 3;
// int errorCount = 0;
//
// // file playback stuff
// #define HEADER_SIZE 2
// File myFile;
// int animationNumber = 0;
// char fileName[8];
//
// // input, pots or buttons
// Bounce bouncer = Bounce();
//
//
//
// // UDP specific
// #define ART_NET_PORT 6454
// // Artnet settings
// // Opcodes
// #define ART_POLL 0x2000
// #define ART_DMX 0x5000
// // Buffers
// #define MAX_BUFFER_ARTNET 530
// uint8_t artnetPacket[MAX_BUFFER_ARTNET];
//
// // Packet
// #define ART_NET_ID "Art-Net\0"
// #define ART_DMX_START 18
// // Change ip and mac address for your setup
// byte mac[] = { 0xDE, 0xAD, 0x23, 0x54, 0xFE, 0xED };
// //the IP address for the shield:
// byte ip[] = { 192, 168, 2, 84};
// // the router's gateway address:
// byte gateway[] = { 192, 168, 2, 1};
// // the subnet:
// byte subnet[] = { 255, 255, 255, 0 };
//
//
// EthernetUDP Udp;
// bool hasWiznet = false;
//
// void setup() {
//     Serial.begin(115200);
//     #if OCTOWSMODE
//         LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
//     #else
//         FastLED.addLeds<LED_TYPE, DATA_PIN, GRB>(leds, NUM_LEDS);
//     #endif
//
//     #if BUTTON_PIN
//         pinMode(BUTTON_PIN, INPUT_PULLUP);
//         bouncer.attach(BUTTON_PIN);
//         bouncer.interval(5);
//     #endif
//
//     FastLED.setDither( 0 );
//     for(int y = 0 ; y < NUM_LEDS ; y++) leds[y] = CRGB::Black;
//     doShow();
//     initSD();
//     // initTest();
//     pinMode(WIZ_RESET, OUTPUT);
//     digitalWrite(WIZ_RESET, LOW);    // begin reset the WIZ820io
//     pinMode(WIZ_CS, OUTPUT);
//     digitalWrite(WIZ_CS, HIGH);  // de-select WIZ820io
//     pinMode(SD_CS, OUTPUT);
//     digitalWrite(SD_CS, HIGH);   // de-select the SD Card
//     digitalWrite(WIZ_RESET, HIGH);
//
//     // Ethernet.begin(mac, ip);
//     Ethernet.begin(mac, ip, gateway, subnet);
//     if (true) {
//         Serial.println(F("Wiznet Found"));
//         Udp.begin(ART_NET_PORT);
//         hasWiznet = true;
//         // printIPAddress();
//     }
//     else {
//         Serial.println(F("no wiznet"));
//     }
// }
//
// int ha = 0;
// char  ReplyBuffer[] = "acknowledged";
// void loop() {
//
//     // Serial.printf("%d \n", millis());
//     //    byte _ip[] = {192,168,2,83};
//     //    // send a reply to the IP address and port that sent us the packet we received
//     //    Udp.beginPacket( _ip, 4444);
//     //    Udp.write(ReplyBuffer);
//     //    Udp.endPacket();
//     //  delay(100);
//
//
//
//     return;
//     switch(mode){
//         case STANDBY:
//             standby();
//             break;
//         case SDCARD:
//             playAnimationFromSD();
//             break;
//         case SERIALMODE:
//             serialMode();
//             break;
//         case ARTNET:
//             checkForPacket;
//             break;
//     }
//     if(hasWiznet){
//         checkForPacket();
//     }
//     updateOtherThings();
// }
//
//
// void printIPAddress()
// {
//   Serial.print("My IP address: ");
//   for (byte thisByte = 0; thisByte < 4; thisByte++) {
//     // print the value of each byte of the IP address:
//     Serial.print(Ethernet.localIP()[thisByte], DEC);
//     Serial.print(".");
//   }
//   Serial.println();
// }
//
// uint16_t checkForPacket(){
//     uint16_t packetSize = Udp.parsePacket();
//     if (packetSize <= MAX_BUFFER_ARTNET && packetSize > 0){
//         Udp.read(artnetPacket, MAX_BUFFER_ARTNET);
//         // Check that packetID is "Art-Net" else ignore
//         for (uint8_t i = 0 ; i < 8 ; i++){
//             if (artnetPacket[i] != ART_NET_ID[i])
//             return 0;
//         }
//         mode = ARTNET;
//         uint8_t opcode = artnetPacket[8] | artnetPacket[9] << 8;
//         if (opcode == ART_DMX) {
//             uint16_t sequence = artnetPacket[12];
//             uint8_t incomingUniverse = artnetPacket[14] | artnetPacket[15] << 8;
//             uint16_t dmxDataLength = artnetPacket[17] | artnetPacket[16] << 8;
//             processArtnetPacket(incomingUniverse, dmxDataLength, sequence, artnetPacket + ART_DMX_START);
//             return ART_DMX;
//         }
//         if (opcode == ART_POLL) {
//             return ART_POLL;
//         }
//         return 0;
//     }
//     else{
//     return 0;
//     }
// }
//
//
// void processArtnetPacket(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data){
//     for(int i = 0; i < length/3; i++){
//         if(i < NUM_LEDS){
//             leds[i] = CRGB(data[i*3], data[i*3+1], data[i*3+2]);
//         }
//     }
//     FastLED.show();
//     // sendFrame = 1;
//     // // Store which universe has got in
//     // if ((universe - startUniverse) < maxUniverses)
//     //     universesReceived[universe - startUniverse] = 1;
//     //
//     // for (int i = 0 ; i < maxUniverses ; i++){
//     //     if (universesReceived[i] == 0){
//     //         sendFrame = 0;
//     //         break;
//     //     }
//     // }
//     //
//     // // read universe and put into the right part of the display buffer
//     // for (int i = 0; i < length / 3; i++){
//     //     int led = i + (universe - startUniverse) * (previousDataLength / 3);
//     //     if (led < NUM_LEDS){
//     //         leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
//     //     }
//     // }
//     // previousDataLength = length;
//     //
//     // if (sendFrame){
//     //     FastLED.show();
//     //
//     //     // Reset universeReceived to 0
//     //     memset(universesReceived, 0, maxUniverses);
//     // }
// }
//
//
//
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
//
// // initialise SDcard
// void initSD(){
//     Serial.print(F("Initializing SD card..."));
//     pinMode(SD_CS, OUTPUT);
//     if (!SD.begin(SD_CS)) {
//       Serial.println(F("initialization SD failed! ready on serial"));
//       mode = SERIALMODE;
//       return;
//     }
//     else Serial.println(F("SD Card detected"));
// }
//
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
//
// void doShow(){
//     // for(int i = 0; i < NUM_STRIPS; i += NUM_LEDS_PER_STRIP){
//     //     leds[i] = CRGB(0,0,0);
//     // }
//     FastLED.show();
// }
//
//
// bool updateOtherThings(){
//     #if DIM_POT_PIN
//     FastLED.setBrightness(map(analogRead(DIM_POT_PIN), 0, 1023, 255, 0));
//     #endif
//     #if BUTTON_PIN
//         bouncer.update();
//         if(bouncer.read() != 1){
//             animationNumber++;
//             Serial.println(animationNumber);
//             while(bouncer.read() != 1) bouncer.update();
//             delay(100);
//             return true;
//         }
//     #endif
//
//     if(Serial.available()){
//         mode = SERIALMODE;
//         return true;
//     }
//     return false;
// }
//
// // little animation to test leds
// void initTest() {
//     int del = 3;
//     for (int i = 0 ; i < NUM_LEDS; i++) {
//         leds[i] = CRGB(100, 10, 10);
//         delay(del);
//         doShow();
//     }
//     for (int i = 0 ; i < NUM_LEDS; i++) {
//         leds[i] = CRGB(0, 0, 0);
//         delay(del);
//         doShow();
//     }
// }
//
// void standby(){
//     ha++;
//     for(int i = 0; i < NUM_LEDS; i++){
//         leds[i] = CHSV(ha+int(ha+i*2+millis()/1.0)%255,255,255);
//     }
//     FastLED.show();
//
// }
