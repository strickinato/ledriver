#include "LEDriverView.h"

LEDriverView::LEDriverView(){
    useOled = true;
    useSerial = true;
}

void LEDriverView::begin(){
    Wire.begin();
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    oled.setFont(System5x7);
    oled.setScroll(true);
}

size_t LEDriverView::write(uint8_t _c){
    if(useOled) oled.write(_c);
    if(useSerial) Serial.write(_c);
}
