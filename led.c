#include "led.h"
#include "../pre_emptive_os/api/general.h"
#include "../pre_emptive_os/api/osapi.h"
#include "hw.h"

void gameLights() {
    tU8 i;
    for (i = 0; i < 100; i++) {
        setLED(1, TRUE);
        osSleep(16);
        setLED(2, TRUE);
        osSleep(16);
        setLED(1, FALSE);
        osSleep(10);
        setLed(2, FALSE);
        osSleep(10);
    }
}

void gameOverLights() {
    tU8 i;
    for (i = 0; i < 20; i++) {
        setLED(1, TRUE);
        osSleep(10);
        setLED(2, TRUE);
        osSleep(10);
        setLED(1, FALSE);
        osSleep(5);
        setLed(2, FALSE);
        osSleep(5);
    }
}

void mainMenuLights() {
    tU8 i;
    for (i = 0; i < 150; i++) {
        setLED(1, TRUE);
        osSleep(20);
        setLED(2, TRUE);
        osSleep(15);
        setLED(1, FALSE);
        osSleep(5);
        setLed(2, FALSE);
        osSleep(5);
    }
}
