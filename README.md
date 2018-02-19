### LEDRIVER ###
Addressable lighting controller and accessories.

### Hardware ###
#### ledriver ####
(l-e-d river)
A teensy 3.2 versatile addressable lighting controller based on the OctoWS811 shield with lots of goodies.
- Wiznet module
- SDcard
- OLED screen
- UART breakouts
- Other clk+data output
- three buttons
- two pots
- one status led
- (piezo?? vibration detection tap tempo)

#### Todo ####
- Make sure teensy 3.5 fits...

#### RJPowerData ####
Connects to a ledriver board rj45 output and distributes the 4 channels of ws2811 with power.
- Fits 3.96mm and 5.08mm pitch connectors
- Fused output (microblades)
- LEDs after fuzes
- second 8p8c connector if not breaking out all the data lines.

## Firmware ##
Burrito supreme.

#### Command System ####
- Control via serial
- network -> generate web interface?
- websocket
- osc
- map to dmx?

|action|   |   |   |
|---|---|---|---|
|setMode|   |   |   |
|setConfigProperty|   |   |   |
|setSpeed|   |   |   |
|setBrightness|   |   |   |




#### LED Packet ####
Reserve 2-3 bytes for controller specific info, such as :
- encode "lerp" timer and "beatCount".
- a packet type? (ramp to in X seconds);
- - command packet, append longer commands (ie save "sweetAnimation.ani");

#### Controller Commands ####
- Record animation
- play animation
- run test patterns

#### Test Patterns ####
Dank test patterns for production tests.

#### config.ldr ####
SD card config file.
```
name=stjean
dhcp=true
ip=10.0.0.42
mac=42-E2-42-66-F3-21
debug=oled
debug=Serial1/2/3
```

#### Logging on SD ####


#### Dependencies ####
- Ethernet
- FastLED
- SD
- SDconfigFile
- TeensyDMX

### Bonus Round ###
- Its also a midi controller so you can control the software to control your controller.
- OSC controller too
- NTP time + rtc, scheduler, `schedule.scd`
- Weather
- OSC message printing on oled. Debug the network.
- pinging tool (ping list of hosts.txt)
- DMX controller,
- ArtNet controller
- DMX to usbMIDI, MIDI
- MIDI to DMX
Q
### Notes ###

##### DMX #####
- DMX+ 485A xlr pin 3 yellow
- DMX- 485B xlr pin 2 blue
