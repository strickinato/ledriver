#ifndef Network_h
#define Network_h

#include "SPI.h"
#include "Ethernet.h"
#include "PinMap.h"

// UDP specific
#define ART_NET_PORT 6454
// Opcodes
#define ART_POLL 0x2000
#define ART_DMX 0x5000
// Buffers
#define MAX_BUFFER_ARTNET 530
// Packet
#define ART_NET_ID "Art-Net\0"
#define ART_DMX_START 18

class Network {
    public:
        Network();
        void begin();

        bool useDHCP;
        bool isConnected;
        uint8_t mac[6];
        uint8_t ip[4];
        IPAddress ipAddress;

        bool setIp(const char * _str);
        bool checkArtnet();

        #define MAX_UNIVERSE_SUPPORTED 16
        bool universesReceived[16];

        EthernetUDP Udp;
        uint8_t artnetPacket[MAX_BUFFER_ARTNET];
        uint16_t packetSize;
        uint16_t opcode;
        uint8_t sequence;
        uint16_t incomingUniverse;
        uint16_t dmxDataLength;
        uint8_t * artnetData;
    private:
};

#endif
