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

/*
 *
 *
 * gotowe:
 * 4 lcd spi gpio timer
 * todo:
 * zapis danych i2c i eeprom  2
 * przerwania 1
 * wypisywanie na komputerze 1
 * rozna czestotliwosc dzwieku 0.5
 *
 *
 */

/* Stale */
#define timer (T1TCR & 0x01)
tU8 board[12][18];
tU8 activeBlock[4][2];

tBool isInActiveBlock(tU8 x, tU8 y) {
	int i;
	for(i = 0; i < 4; i++) {
		if(activeBlock[i][0] == x && activeBlock[i][1] == y) {
			return TRUE;
		}
	}
	return FALSE;
}

tBool detectCollisionLeft() {
	int i;
	for (i = 0; i < 4; i++) {
		int X = activeBlock[i][0];
		int Y = activeBlock[i][1];
		if ((board[X-1][Y] == 1 && !isInActiveBlock(X -1, Y)) || X == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

tBool detectCollisionRight() {
	int i;
	for (i = 0; i < 4; i++) {
		int X = activeBlock[i][0];
		int Y = activeBlock[i][1];
		if((board[X + 1][Y] == 1 && !isInActiveBlock(X + 1, Y)) || X == 11) {
			return TRUE;
		}
	}
	return FALSE;
}

tBool detectCollisionDown() {
	int i;
	for (i = 0; i < 4; i++) {
		int X = activeBlock[i][0];
		int Y = activeBlock[i][1];
		if((board[X][Y+1] == 1 && !isInActiveBlock(X, Y + 1)) || Y == 17) {
			return TRUE;
		}
	}
	return FALSE;
}

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
	lcdRect(24, 125, 82, 5, 53);
	int i;
	int j;
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 18; j++) {
			if (board[i][j] != 0) {
				lcdRect(i*7 + 24, j*7, 5, 5, 255);
			}
		}
	}
}

void moveActiveBlock(tBool left, tBool right, tBool down) {
	int i;
	for(i = 0; i < 4; i++) {
		int X = activeBlock[i][0];
		int Y = activeBlock[i][1];
		if(left) {
			board[X][Y] = 0;
			board[X - 1][Y] = 1;
			activeBlock[i][0] = X - 1;
		}
		else if(right) {
			board[X][Y] = 0;
			board[X + 1][Y] = 1;
			activeBlock[i][0] = X + 1;
		}
		else if(down) {
			board[X][Y] = 0;
			board[X][Y + 1] = 1;
			activeBlock[i][1] = Y + 1;
		}
	}
}

void rotateActiveBlock() {

}

void applyJoystickMovement() {
	tU8 anyKey;
	anyKey = checkKey();
	if (anyKey != KEY_NOTHING) {
	  if (anyKey == KEY_CENTER) {
		// nic sie nie dzieje
	  }else if (anyKey == KEY_LEFT && !detectCollisionLeft()) {
		  moveActiveBlock(TRUE, FALSE, FALSE);
	  }else if (anyKey == KEY_RIGHT && !detectCollisionRight()) {
		  moveActiveBlock(FALSE, TRUE, FALSE);
	  }else if (anyKey == KEY_UP) {
		  rotateActiveBlock();
	  }else if (anyKey == KEY_DOWN) {
		  // nic się nie dzieje
	  }
	}
}


void checkGameConditions(tBool* serafin) {
	if (detectCollisionDown()) {
		int i;
		for(i = 0; i < 12; i++) {
			if(board[i][1] == 1) {
				*serafin = FALSE;
			}
		}
		activeBlock[0][0] = 5;
		activeBlock[1][0] = 6;
		activeBlock[2][0] = 6;
		activeBlock[3][0] = 5;
		activeBlock[0][1] = 1;
		activeBlock[1][1] = 1;
		activeBlock[2][1] = 0;
		activeBlock[3][1] = 0;
	}
}

void playTetris () {
	lcdClrscr();
	int i;
	int j;
	for (i = 0; i < 12; i++) {
		for (j = 0; j <18; j++) {
			board[i][j] = 0;
		}
	}
	activeBlock[0][0] = 4;
	activeBlock[1][0] = 5;
	activeBlock[2][0] = 5;
	activeBlock[3][0] = 4;
	activeBlock[0][1] = 1;
	activeBlock[1][1] = 1;
	activeBlock[2][1] = 0;
	activeBlock[3][1] = 0;
	tBool serafin = TRUE;
	while (serafin) {
		lcdClrscr();
		applyJoystickMovement();
		checkGameConditions(&serafin);
		moveActiveBlock(FALSE, FALSE, TRUE);
		drawBoard();
		delayMS(TRUE, 50);
	}

}