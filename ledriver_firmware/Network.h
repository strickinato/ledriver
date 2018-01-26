#ifndef Network_h
#define Network_h

#include <SPI.h>
#include <Ethernet.h>
#include "PinMap.h"

#define ARTNET_PORT 6454

class Network {
    public:
        bool useDHCP;
        bool isConnected;
        uint8_t mac[6];
        uint8_t ip[4];

        Network();
        void begin();

    private:
        EthernetUDP Udp;

};

#endif
