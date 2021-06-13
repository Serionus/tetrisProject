#include "music.h"
#include "../pre_emptive_os/api/general.h"
#include "../startup/lpc2xxx.h"
#include "../startup/config.h"

void mainMenuMusic() {
	tU16 i;
	for(i=0; i < 50; i++) {
		setBuzzer(TRUE);
		osSleep(2);
		setBuzzer(FALSE);
		osSleep(2);
	}
	osSleep(2);
	for(i=0; i < 20; i++) {
		setBuzzer(TRUE);
		osSleep(2);
		setBuzzer(FALSE);
		osSleep(1);
	}
	osSleep(4);
	for(i=0; i < 40; i++) {
		setBuzzer(TRUE);
		osSleep(2);
		setBuzzer(FALSE);
		osSleep(1);
	}
	osSleep(6);
}

void gameOverMusic() {
	tU16 i;
	for (i = 0; i < 10; i++) {
		setBuzzer(TRUE);
		osSleep(15);
		setBuzzer(FALSE);
		osSleep(1);
	}
	osSleep(2);
}

void blockTouchSound() {
	setBuzzer(TRUE);
	osSleep(15);
	setBuzzer(FALSE);
	osSleep(2);
}
