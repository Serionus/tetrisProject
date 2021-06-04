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
//#include "music.h"

#include "select.h"


void drawMainMenu(){
	lcdRect(0, 0, 130, 130, 53);
	tBool isEnd;
	tMenu mainMenu;
	tBool mainMenuFlag = TRUE;

	tU8 xd[2];
	tU8 xxd[2];
	xd[0] = 'X';
	xd[1] = 'D';
	eepromWrite(0x0000, xd, sizeof(xd));
	eepromPageRead(0x0000, xxd, sizeof(xd));

	mainMenu.xPos = 5;
	mainMenu.yPos = 26;
	mainMenu.xLen = 6+(14*8);
	mainMenu.yLen = 5*14;
	mainMenu.noOfChoices = 4;
	mainMenu.initialChoice = 0;
	mainMenu.pHeaderText = "!TETRYS!";
	mainMenu.headerTextXpos = 31;
	mainMenu.pChoice[0] = "GRAJ";
	mainMenu.pChoice[1] = "AUTORZY";
	mainMenu.pChoice[2] = "RESTART";
	mainMenu.pChoice[3] = xxd;

	mainMenu.bgColor       = 0x0;
	mainMenu.borderColor   = 0x6d;
	mainMenu.headerColor   = 0x0;
	mainMenu.choicesColor  = 0xfd;
	mainMenu.selectedColor = 0xe0;

	while(mainMenuFlag){
//		switchMusic(1);
		lcdClrscr();
		switch(drawMenu(mainMenu)){
		case 0:
			playTetris(); break;
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
}

