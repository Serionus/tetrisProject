#include "music.h"
#include "../pre_emptive_os/api/general.h"
#include "../startup/lpc2xxx.h"
#include "../startup/config.h"

void mainMenuMusic() {
	tU16 i;
	for(i=0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 2);
		setBuzzer(FALSE);
		delayMS(TRUE, 3);
	}
	delayMS(TRUE, 30);
	for(i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 1);
		setBuzzer(FALSE);
		delayMS(TRUE, 2);
	}
	delayMS(TRUE, 40);
	for (i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 3);
		setBuzzer(FALSE);
		delayMS(TRUE, 1);
	}
	delayMS(TRUE, 30);
    	for(i=0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 2);
		setBuzzer(FALSE);
		delayMS(TRUE, 3);
	}
	delayMS(TRUE, 40);
	for(i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 1);
		setBuzzer(FALSE);
		delayMS(TRUE, 2);
	}
	delayMS(TRUE, 40);
	for (i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 3);
		setBuzzer(FALSE);
		delayMS(TRUE, 1);
	}
	delayMS(TRUE, 30);
}

void gameMusic() {
	tU16 i;
	for (i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 3);
		setBuzzer(FALSE);
	    delayMS(TRUE, 2);
	}
	delayMS(TRUE, 20);
	for (i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 5);
		setBuzzer(FALSE);
		delayMS(TRUE, 2);
	}
	delayMS(TRUE, 20);
	for (i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 3);
		setBuzzer(FALSE);
		delayMS(TRUE, 5);
	}
	delayMS(TRUE, 30);
    	for (i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 7);
		setBuzzer(FALSE);
	    delayMS(TRUE, 2);
	}
	delayMS(TRUE, 30);
	for (i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 5);
		setBuzzer(FALSE);
		delayMS(TRUE, 1);
	}
	delayMS(TRUE, 30);
	for (i = 0; i < 100; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 2);
		setBuzzer(FALSE);
		delayMS(TRUE, 3);
	}
	delayMS(TRUE, 40);
}

void failureSound() {
	setBuzzer(TRUE);
	delayMS(TRUE, 1);
	setBuzzer(FALSE);
	delayMS(TRUE, 2);
}

void switchMusic(tU8 option) {
	switch (option){
	case 1:
		mainMenuMusic();
		break;
	case 2: 
		gameMusic();
		break;
	case 3:
		failureSound();
		break;
	default:
		break;
	}
}

void delayMS(tBool whichTimer,tU16 delay) {
	if(whichTimer == TRUE) {
		T1TCR = 0x02;
		T1PR  = 0x00;
		T1MR0 = delay * ((CORE_FREQ)/ (1000 * PBSD));
		T1IR  = 0xff;
		T1MCR = 0x04;
		T1TCR = 0x01;         
		while (T1TCR & 0x01);
	} else {
		T1TCR = 0x02;
		T1PR  = 0x00;
		T1MR0 = delay * ((CORE_FREQ)/ (1000 * PBSD));
		T1IR  = 0xff;
		T1MCR = 0x04;
		T1TCR = 0x01;
	}
}
