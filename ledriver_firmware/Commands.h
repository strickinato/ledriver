#ifndef Commands_h
#define Commands_h


enum Commands{
    LED_DATA_CMD = 42,       // '*' means a fresh packet of data
    SET_BRIGHTNESS_CMD = 1,  //
    BLACKOUT_CMD = 43,
    SET_SDCONFIG_CMD = 44,
    SET_MODE_CMD = 44,
    REBOOT_CMD,
};


#endif
