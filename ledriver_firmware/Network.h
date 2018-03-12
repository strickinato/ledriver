#ifndef Network_h
#define Network_h

#include "SPI.h"
#include "Ethernet.h"
#include "PinMap.h"
#include "Defaults.h"


// UDP specific
#define ART_NET_PORT 6454
// Opcodes
#define ART_POLL 0x2000
#define ART_DMX 0x5000
#define ART_POLL_REPLY 0x2100
// Buffers
#define MAX_BUFFER_ARTNET 530

// Packet
#define ART_NET_ID "Art-Net\0"
#define ART_DMX_START 18


struct artnet_reply_s {
  uint8_t  id[8];
  uint16_t opCode;
  uint8_t  ip[4];
  uint16_t port;
  uint8_t  verH;
  uint8_t  ver;
  uint8_t  subH;
  uint8_t  sub;
  uint8_t  oemH;
  uint8_t  oem;
  uint8_t  ubea;
  uint8_t  status;
  uint8_t  etsaman[2];
  uint8_t  shortname[18];
  uint8_t  longname[64];
  uint8_t  nodereport[64];
  uint8_t  numbportsH;
  uint8_t  numbports;
  uint8_t  porttypes[4];//max of 4 ports per node
  uint8_t  goodinput[4];
  uint8_t  goodoutput[4];
  uint8_t  swin[4];
  uint8_t  swout[4];
  uint8_t  swvideo;
  uint8_t  swmacro;
  uint8_t  swremote;
  uint8_t  sp1;
  uint8_t  sp2;
  uint8_t  sp3;
  uint8_t  style;
  uint8_t  mac[6];
  uint8_t  bindip[4];
  uint8_t  bindindex;
  uint8_t  status2;
  uint8_t  filler[26];
} __attribute__((packed));



class Network {
    public:
        Network();
        void begin();

        bool useDHCP;
        bool isConnected;
        uint8_t mac[6];
        uint8_t ip[4];
        IPAddress ipAddress;
        IPAddress broadcast;
        bool setIp(const char * _str);
        bool checkArtnet();
        void replyArtnetPoll();
        bool universesReceived[16];

        EthernetUDP Udp;
        uint8_t artnetPacket[MAX_BUFFER_ARTNET];
        struct artnet_reply_s ArtPollReply;
        uint16_t packetSize;
        uint16_t opcode;
        uint8_t sequence;
        uint16_t incomingUniverse;
        uint16_t dmxDataLength;
        uint8_t * artnetData;
    private:
};

#endif
