/*
 * tetris.c
 *
 *  Created on: May 7, 2021
 *      Author: embedded
 */

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

#define GAME_NOT_STARTED 0
#define GAME_RUNNING     1
#define GAME_OVER        2
#define GAME_END         3

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 16

#define FIGURE_SIZE 4
#define NUM_OF_FIGURE 19

#define timer (T1TCR & 0x01)

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

void bzzbzz() {
	tU16 i;
	for(i=0; i < 50; i++) {
		setBuzzer(TRUE);
		delayMS(TRUE, 2);
		setBuzzer(FALSE);
		delayMS(TRUE, 2);
	}
}

void logKeys () {
	lcdClrscr();
	while(1) {
	    tU8 anyKey;
	    anyKey = checkKey();
	    if (anyKey != KEY_NOTHING) {
	      if (anyKey == KEY_CENTER) {
	    	  bzzbzz();
	    	  lcdClrscr();
	      }else if (anyKey == KEY_LEFT) {
	    	  lcdPuts("\n wcisles LEFT");
	      }else if (anyKey == KEY_RIGHT) {
	    	  lcdPuts("\n wcisles RIGHT");
	      }else if (anyKey == KEY_UP) {
	    	  lcdPuts("\n wcisles UP");
	      }else if (anyKey == KEY_DOWN) {
	    	  lcdPuts("\n wcisles DOWN");
	      }
	    }
	}
}

void playTetris(){
	lcdClrscr();

	tBool isEnd;
	tMenu mainMenu;
	tBool mainMenuFlag = TRUE;

	mainMenu.xPos = 5;
	mainMenu.yPos = 26;
	mainMenu.xLen = 6+(14*8);
	mainMenu.yLen = 5*14;
	mainMenu.noOfChoices = 3;
	mainMenu.initialChoice = 0;
	mainMenu.pHeaderText = "!TETRYS!";
	mainMenu.headerTextXpos = 31;
	mainMenu.pChoice[0] = "GRAJ";
	mainMenu.pChoice[1] = "AUTORZY";
	mainMenu.pChoice[2] = "WYJDZ";
	mainMenu.bgColor       = 0x0;
	mainMenu.borderColor   = 0x6d;
	mainMenu.headerColor   = 0x0;
	mainMenu.choicesColor  = 0xfd;
	mainMenu.selectedColor = 0xe0;

	while(mainMenuFlag){
		switch(drawMenu(mainMenu)){
		case 0:
			logKeys(); break;
		case 1:
			lcdClrscr();
			lcdPuts("TWORCY:\n");
			lcdPuts("Antoni Karwowski\n");
			lcdPuts("Piotr Tomczak\n");
			lcdPuts("Michal Gebel");
			osSleep(400);
			lcdClrscr();
			break;
		case 2:
			mainMenuFlag = FALSE;
			break;
		}

	}
//	int counter = 0;
//
//	while (1) {
//		bzzbzz();
//
//		delayMS(TRUE, 500);
//		if(counter % 2 == 0) {
//			lcdPuts("\n rysowanie");
//			counter ++;
//		} else if(counter % 2 == 1) {
//			lcdPuts("\n na ekranie");
//			counter ++;
//		}
//		if(counter == 6) {
//			lcdClrscr();
//			counter = 0;
//			lcdRect(0, 0, 130, 130, 0xFFFFFF);
//			lcdRect(55, 55, 20, 20, 0xFFF000);
//			delayMS(TRUE, 5000);
//			lcdClrscr();
//		}

}


