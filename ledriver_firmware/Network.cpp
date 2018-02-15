#include "Network.h"


Network::Network(){
    useDHCP = true;
    ip[0] = 0;
    ip[1] = 0;
    ip[2] = 0;
    ip[3] = 0;
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

    Udp.begin(ART_NET_PORT);

}

bool Network::setIp(const char * _str){
    return (ipAddress.fromString(_str));
}

bool Network::checkArtnet(){
    packetSize = Udp.parsePacket();
    if (packetSize <= MAX_BUFFER_ARTNET && packetSize > 0){
        Udp.read(artnetPacket, MAX_BUFFER_ARTNET);
        for (byte i = 0 ; i < 8 ; i++){
            if (artnetPacket[i] != ART_NET_ID[i]){
                return 0;
            }
        }
        // get opcode either DMX or POLL
        opcode = artnetPacket[8] | artnetPacket[9] << 8;
        if (opcode == ART_DMX){
            sequence = artnetPacket[12];
            incomingUniverse = artnetPacket[14] | artnetPacket[15] << 8;
            dmxDataLength = artnetPacket[17] | artnetPacket[16] << 8;
            artnetData = artnetPacket + ART_DMX_START;
            // if (artDmxCallback) (*artDmxCallback)(incomingUniverse, dmxDataLength, sequence, artnetPacket + ART_DMX_START);
            return 1;
        }
        else if (opcode == ART_POLL){
            return 0;//ART_POLL;
        }
    }
}
