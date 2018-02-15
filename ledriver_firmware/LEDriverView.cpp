#include "LEDriverView.h"

LEDriverView::LEDriverView(){

}

void LEDriverView::begin(){
    Wire.begin();
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    oled.setFont(System5x7);
    oled.setScroll(true);
}

size_t LEDriverView::write(uint8_t _c){
    oled.write(_c);
    Serial.write(_c);
}
