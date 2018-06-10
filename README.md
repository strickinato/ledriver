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

#### UI Board ####
Move pots, butons, OLED to a second poard with dedicated controller?

#### Todo ####
- Make sure teensy 3.5 fits...

#### RJPowerData ####
Connects to a ledriver board rj45 output and distributes the 4 channels of ws2811 with power.
- Fits 3.96mm and 5.08mm pitch connectors
- Fused output (microblades)
- LEDs after fuzes
- second 8p8c connector if not breaking out all the data lines.

## Firmware ##

https://forum.pjrc.com/threads/91-teensy-3-MAC-address

Sketch uses 70248 bytes (26%) of program storage space. Maximum is 262144 bytes.

### LED setup ###
Strategy : use a fixed number of LED per strips, config defines exacte number of LED per strip output. Then incomming buffers are spread to the correct slots.

#### Command System ####
- OSC based ?
- Control via serial
- network -> generate web interface?
- websocket
- osc
- map commands to dmx? brightness etc.

`/lr/brightness/ 255` -> BRIGHTNESS_CMD_BYTE + value

```
"dmx_map": {
        "0" : "/lr/brightness",
        "1" : "/lr/speed",
        "2" : "/lr/animation"
    }
```

##### Command List #####
- mode 2
- speed 255
- brightness 255
-

### webriver interface ###
Perhaps the controller should change the interface as needed. In example if set to SD card playback, populate a drop menu with names of saved animations.
- Make phone / desktop friendly.


#### LED lookup ####
- Lower Res feature? control 2 3 or 4 pixels as 1? or do in computer software...
- Add support for freeliner LED files!!


#### LED Packet ####
Reserve 2-3 bytes for controller specific info, such as :
- encode "lerp" timer and "beatCount".
- a packet type? (ramp to in X seconds);
- - command packet, append longer commands (ie save "sweetAnimation.ani");

#### Controller Commands ####
- Record animation
- play animation
- run test patterns
###### inbound
- `/record 1 (fileName)` make a filename if none provided
- `/record 0` stop recording
- `/bright 255`
- `/mode 2`
- `/speed 255`
- `/getinfo`
###### outbound

#### SD Card Saved animations
Somehow freeliner needs to to send its sync through Artnet, perhaps with sequence value?
SMPTE?
Or ArtTimeCode
Paire sequence number with lerp value?


#### Test Patterns ####
Dank test patterns for production tests.

#### config.json ####
SD card config file. Uses JSON now.
```
{

}
name=stjean
dhcp=true
ip=10.0.0.42
debug=oled
debug=Serial1/2/3
```
#### Mapping Assist ####
- Use up down button to select led,
- Set this led as the first of universe X
- Set the end?
#### Logging on SD ####
Save information as running time, errors, input frequency

#### Dependencies ####
- Ethernet
- FastLED
- SD
- SDconfigFile
- TeensyDMX

#### Optimisation ####
- between each frame pick one task from a list and increment.
    - (check websocket, check udp for OSC, ???)

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

<!-- ### Glossary ###
 - `template` set of parameters for animations, such as color and easing, templates are applied to `groups`
- `group` group of segments with a center point.
- -->
### Notes ###

##### DMX #####
- DMX+ 485A xlr pin 3 yellow
- DMX- 485B xlr pin 2 blue
