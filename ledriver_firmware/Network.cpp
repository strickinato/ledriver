#include "Network.h"


Network::Network(){
    useDHCP = true;
    ip[0] = 10;
    ip[1] = 0;
    ip[2] = 0;
    ip[3] = 42;
    mac[0] = 0x00;
    mac[1] = 0xAA;
    mac[2] = 0xBB;
    mac[3] = 0xCC;
    mac[4] = 0xDE;
    mac[5] = 0x02;
}


void Network::begin(){


    pinMode(WIZNET_RESET_PIN, OUTPUT);
    digitalWrite(WIZNET_RESET_PIN, LOW);    // begin reset the WIZ820io
    pinMode(WIZNET_CS_PIN, OUTPUT);
    digitalWrite(WIZNET_CS_PIN, HIGH);  // de-select WIZ820io
    pinMode(SDCARD_CS_PIN, OUTPUT);
    digitalWrite(SDCARD_CS_PIN, HIGH);   // de-select the SD Card
    digitalWrite(WIZNET_RESET_PIN, HIGH);
    digitalWrite(WIZNET_CS_PIN, LOW);
    if(useDHCP){
        if(Ethernet.begin(mac)){
            for(int i = 0; i < 4; i++){
                ip[i] = Ethernet.localIP()[i];
            }
        }
        else {
            useDHCP = false;
        }
    }
    if(!useDHCP){

        Ethernet.begin(mac, ip);
    }

}
