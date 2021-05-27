#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <ea_init.h>
#include <stdlib.h>
#include "lcd.h"
#include "key.h"
#include "select.h"
#include "tetris.h"
#include "../startup/lpc2xxx.h"

#include "select.h"
#define timer (T1TCR & 0x01)

/* Stale */
tU8 boxesList[12][18];

void delayMS(tBool whichTimer,tU16 delay)
{
	if(whichTimer == TRUE){
		//funkcja ustawia timer i czeka na jego zakoñczenie
		T1TCR = 0x02;          //stop and reset timer
		T1PR  = 0x00;          //ustaw prescaler na 0
		T1MR0 = delay * ((CORE_FREQ)/ (1000 * PBSD));
		T1IR  = 0xff;          //ustaw flagi przerwan
		T1MCR = 0x04;          //zatrzymaj timer po MR0
		T1TCR = 0x01;          //start

		//czekaj na przerwanie
		while (T1TCR & 0x01);
	}
	else
	{
		//funkcja ustawia timer i pozwala na zastosowanie go w zewnêtrznej petli poprzez While(timer)
		T1TCR = 0x02;
		T1PR  = 0x00;
		T1MR0 = delay * ((CORE_FREQ)/ (1000 * PBSD));
		T1IR  = 0xff;
		T1MCR = 0x04;
		T1TCR = 0x01;
	}
}

void drawBoard() {
	int i;
	int j;
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 18; j++) {
			if (boxesList[i][j] != 0) {
				lcdRect(i*7 + 22, j*7, 5, 5, 255);
			}
		}
	}
}

void applyGravity() {
	int i;
		int j;
		for (i = 12; i > 0; i--) {
			for (j = 17; j > 0; j--) {
				if (boxesList[i][j + 1] == 0 && boxesList[i][j] != 0) {
					boxesList[i][j] = 0;
					boxesList[i][j + 1] = 1;
				}
			}
		}
}

void playTetris () {
	lcdClrscr();
	int i;
	int j;
	for (i = 0; i < 12; i++) {
		for (j = 0; j <18; j++) {
			boxesList[i][j] = 0;
		}
	}
	boxesList[5][0] = 1;
	boxesList[6][0] = 1;
	boxesList[7][0] = 1;
	boxesList[8][0] = 1;

	while (1) {
		lcdClrscr();
		applyGravity();
		drawBoard();
		delayMS(TRUE, 1000);
	}
//	while(1) {
//	    tU8 anyKey;
//	    anyKey = checkKey();
//	    if (anyKey != KEY_NOTHING) {
//	      if (anyKey == KEY_CENTER) {
//	    	  lcdClrscr();
//	      }else if (anyKey == KEY_LEFT) {
//	    	  lcdPuts("\n" + boxesList[0][0]+40);
//	      }else if (anyKey == KEY_RIGHT) {
//	    	  lcdPuts("\n wcisles RIGHT");
//	      }else if (anyKey == KEY_UP) {
//	    	  lcdPuts("\n wcisles UP");
//	      }else if (anyKey == KEY_DOWN) {
//	    	  lcdPuts("\n wcisles DOWN");
//	      }
//	    }
//	}
}







