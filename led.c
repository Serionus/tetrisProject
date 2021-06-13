#include "led.h"
#include "../pre_emptive_os/api/general.h"
#include "../pre_emptive_os/api/osapi.h"
#include "hw.h"

void gameOverLights() {
    tU8 i;
    for (i = 0; i < 10; i++) {
        setLED(1, TRUE);
        osSleep(8);
        setLED(2, TRUE);
        osSleep(8);
        setLED(1, FALSE);
        osSleep(8);
        setLED(2, FALSE);
        osSleep(8);
    }
}

void gameLights() {
    tU8 i;
    for (i = 0; i < 6; i++) {
        setLED(1, TRUE);
        osSleep(2);
        setLED(2, TRUE);
        osSleep(2);
        setLED(1, FALSE);
        osSleep(1);
        setLED(2, FALSE);
        osSleep(1);
    }
}

void mainMenuLights() {
    tU8 i;
    for (i = 0; i < 10; i++) {
        setLED(1, TRUE);
        osSleep(5);
        setLED(2, TRUE);
        osSleep(10);
        setLED(1, FALSE);
        osSleep(2);
        setLED(2, FALSE);
        osSleep(2);
    }
}
