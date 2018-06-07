#include "Network.h"
#include "TeensyID.h"

Network::Network() {
    useDHCP = true;
    memcpy(ip, DEFAULT_STATIC_IP, 4);
    teensyMAC(mac);
    // memcpy(mac, DEFAULT_MAC, 6);

}


void Network::begin() {
    // do the pin toggling to reset the wiznet
    pinMode(WIZNET_RESET_PIN, OUTPUT);
    digitalWrite(WIZNET_RESET_PIN, LOW);    // begin reset the WIZ820io

    pinMode(WIZNET_CS_PIN, OUTPUT);
    digitalWrite(WIZNET_CS_PIN, HIGH);  // de-select WIZ820io

    pinMode(SDCARD_CS_PIN, OUTPUT);
    digitalWrite(SDCARD_CS_PIN, HIGH);   // de-select the SD Card
    digitalWrite(WIZNET_RESET_PIN, HIGH);
    digitalWrite(WIZNET_CS_PIN, LOW);

    Ethernet.init(WIZNET_CS_PIN);
    if(useDHCP) {
        if(Ethernet.begin(mac)) { //}, 30000, 4000)){
            for(int i = 0; i < 4; i++) {
                ip[i] = Ethernet.localIP()[i];
                broadcast[i] = Ethernet.localIP()[i];
            }
        }
        else {
            useDHCP = false;
        }
    }

    if(!useDHCP) {
        Ethernet.begin(mac, ip);
    }

    artnetUDP.begin(ART_NET_PORT);
    oscUDP.begin(LR_OSC_PORTIN);

    for(int i = 0; i < 4; i++) {
        broadcast[i] = Ethernet.localIP()[i];
    }
    broadcast[3] = 255;
}

bool Network::setIp(const char * _str) {
    return (ipAddress.fromString(_str));
}

// bool Network::setMac(const char * _str) {
//     bool _valid = true;
//     // check for dashes in the right place
//     for(int i = 0; i < 5; i++) {
//         if(_str[i*3+2] != '-') _valid = false;
//     }
//     if(_valid) {
//         char _buf[2];
//         for(int i = 0; i < 6; i++) {
//             memcpy(_buf, _str+i*3, 2);
//             mac[i] = strtol(_buf, 0, 16);
//         }
//     }
//     return _valid;
// }

bool Network::checkArtnet() {
    packetSize = artnetUDP.parsePacket();
    if (packetSize <= MAX_BUFFER_ARTNET && packetSize > 0) {
        artnetUDP.read(artnetPacket, MAX_BUFFER_ARTNET);
        for (byte i = 0 ; i < 8 ; i++) {
            if (artnetPacket[i] != ART_NET_ID[i]) {
                return 0;
            }
        }
        // get opcode either DMX or POLL
        opcode = artnetPacket[8] | artnetPacket[9] << 8;
        if (opcode == ART_DMX) {
            sequence = artnetPacket[12];
            incomingUniverse = artnetPacket[14] | artnetPacket[15] << 8;
            // Serial.println(incomingUniverse);
            dmxDataLength = artnetPacket[17] | artnetPacket[16] << 8;
            artnetData = artnetPacket + ART_DMX_START;
            // if (artDmxCallback) (*artDmxCallback)(incomingUniverse, dmxDataLength, sequence, artnetPacket + ART_DMX_START);
            return 1;
        }
        else if (opcode == ART_POLL) {
            replyArtnetPoll();
            return 0;//ART_POLL;
        }
    }
}


void Network::replyArtnetPoll() {

    IPAddress local_ip = Ethernet.localIP();
    ip[0] = local_ip[0];
    ip[1] = local_ip[1];
    ip[2] = local_ip[2];
    ip[3] = local_ip[3];

    // sprintf((char *)id, "Art-Net\0");
    memcpy(ArtPollReply.id, ART_NET_ID, sizeof(ArtPollReply.id));
    memcpy(ArtPollReply.ip, ip, sizeof(ArtPollReply.ip));

    ArtPollReply.opCode = ART_POLL_REPLY;
    ArtPollReply.port =  ART_NET_PORT;

    memset(ArtPollReply.goodinput,  0x08, 4);
    memset(ArtPollReply.goodoutput,  0x80, 4);
    memset(ArtPollReply.porttypes,  0xc0, 4);

    uint8_t shortname [18];
    uint8_t longname [64];
    sprintf((char *)shortname, "LEDRiver\0");
    sprintf((char *)longname, "teensy based led controller\0");
    memcpy(ArtPollReply.shortname, shortname, sizeof(shortname));
    memcpy(ArtPollReply.longname, longname, sizeof(longname));

    ArtPollReply.etsaman[0] = 0;
    ArtPollReply.etsaman[1] = 0;
    ArtPollReply.verH       = 1;
    ArtPollReply.ver        = 0;
    ArtPollReply.subH       = 0;
    ArtPollReply.sub        = 0;
    ArtPollReply.oemH       = 0;
    ArtPollReply.oem        = 0xFF;
    ArtPollReply.ubea       = 0;
    ArtPollReply.status     = 0xd2;
    ArtPollReply.swvideo    = 0;
    ArtPollReply.swmacro    = 0;
    ArtPollReply.swremote   = 0;
    ArtPollReply.style      = 0;

    ArtPollReply.numbportsH = 0;
    ArtPollReply.numbports  = 4; // can be set by LED_count.
    ArtPollReply.status2    = 0x08;

    ArtPollReply.bindip[0] = ip[0];
    ArtPollReply.bindip[1] = ip[1];
    ArtPollReply.bindip[2] = ip[2];
    ArtPollReply.bindip[3] = ip[3];

    uint8_t swin[4]  = {0x01,0x02,0x03,0x04};
    uint8_t swout[4] = {0x01,0x02,0x03,0x04};
    for(uint8_t i = 0; i < 4; i++)
    {
        ArtPollReply.swout[i] = swout[i];
        ArtPollReply.swin[i] = swin[i];
    }
    sprintf((char *)ArtPollReply.nodereport, "%i DMX output universes active.\0", ArtPollReply.numbports);
    artnetUDP.beginPacket(broadcast, ART_NET_PORT);//send the packet to the broadcast address
    artnetUDP.write((uint8_t *)&ArtPollReply, sizeof(ArtPollReply));
    artnetUDP.endPacket();

}
