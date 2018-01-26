### LEDRIVER ###
Addressable lighting controller and accessories.

#### Dependencies ####
- Ethernet
- FastLED
- SD
- SDconfigFile
- TeensyDMX

#### Todo ####
- Make sure teensy 3.5 fits...

#### DMX ####
- DMX+ 485A xlr pin 3 yellow
- DMX- 485B xlr pin 2 blue

### Ideas ###

#### LED Packet ####
Reserve 3-4 bytes for controller specific info, such as :
- encode "lerp" timer and "beatCount".
- a packet type? (ramp to in X seconds);

#### Controller Commands ####
- Record animation ()


#### SDCard config file ####
Store config file on a SDCard.
