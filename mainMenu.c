/*
 * mainMenu.c
 *
 *  Created on: May 21, 2021
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
#include "eeprom.h"
#include "music.h"
#include "led.h"
#include "select.h"


void drawMainMenu(){
	lcdRect(0, 0, 130, 130, 53);
	tBool isEnd;
	tMenu mainMenu;
	tBool mainMenuFlag = TRUE;

	mainMenu.xPos = 5;
	mainMenu.yPos = 26;
	mainMenu.xLen = 6+(14*8);
	mainMenu.yLen = 5*14;
	mainMenu.noOfChoices = 4;
	mainMenu.initialChoice = 0;
	mainMenu.pHeaderText = "!TETRYS!";
	mainMenu.headerTextXpos = 31;
	mainMenu.pChoice[0] = "GRAJ";
	mainMenu.pChoice[1] = "WCZYTAJ";
	mainMenu.pChoice[2] = "AUTORZY";
	mainMenu.pChoice[3] = "RESTART";


	mainMenu.bgColor       = 0x0;
	mainMenu.borderColor   = 0x6d;
	mainMenu.headerColor   = 0x0;
	mainMenu.choicesColor  = 0xfd;
	mainMenu.selectedColor = 0xe0;

	mainMenuMusic();
	mainMenuLights();

	while(mainMenuFlag){
		lcdClrscr();
		switch(drawMenu(mainMenu)){
		case 0:
			playTetris(TRUE); break;
		case 1:
			playTetris(FALSE); break;
		case 2:
			lcdClrscr();
			lcdPuts("TWORCY:\n");
			lcdPuts("Antoni Karwowski\n");
			lcdPuts("Piotr Tomczak\n");
			lcdPuts("Michal Gebel");
			osSleep(400);
			lcdClrscr();
			break;
		case 3:
			mainMenuFlag = FALSE;
			break;
		}
	}
}

