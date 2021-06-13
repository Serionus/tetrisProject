#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <ea_init.h>
#include <stdlib.h>
#include "lcd.h"
#include "key.h"
#include "select.h"
#include "tetris.h"
#include "select.h"
#include "eeprom.h"
#include "led.h"
#include "music.h"
#include "VIC.h"
#include "timer.h"
#include "../startup/lpc2xxx.h"
#include "../startup/config.h"

#include "../Common_Def.h"
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

tU8 board[12][18];
tU8 activeBlock[4][2];

void IRQ_sound(void) {
	if (TIMER_MR0_INT & T1IR)
    {
		setBuzzer(TRUE);
        T1IR   = TIMER_MR0_INT;     // Informacja dla Timera #1,
	                                // że jego przerwanie zostało obsłużone.
    }
    else if (TIMER_MR1_INT & T1IR)
    {
		setBuzzer(FALSE);
    	T1IR   = TIMER_MR1_INT;     // Informacja dla Timera #1,
	                                // że jego przerwanie zostało obsłużone.
    }
    VICVectAddr = 0x00;             // End of Interrupt -> informacja dla
                                    // kontrolera przerwań
}

static void init_irq(tU32 period, tU8 duty_cycle) {
    //Zainicjuj VIC dla przerwań od Timera #1
    VICIntSelect &= ~TIMER_1_IRQ;           //Przerwanie od Timera #1 przypisane do IRQ (nie do FIQ)
    VICVectAddr5  = (tU32)IRQ_sound;         //adres procedury przerwania
    VICVectCntl5  = VIC_ENABLE_SLOT | TIMER_1_IRQ_NO;
    VICIntEnable  = TIMER_1_IRQ;            // Przypisanie i odblokowanie slotu w VIC od Timera #1

    T1TCR = TIMER_RESET;                    //Zatrzymaj i zresetuj
    T1PR  = 0;                              //Preskaler nieużywany
    T1MR0 = ((tU64)period)*((tU64)CORE_FREQ)/1000;
    T1MR1 = (tU64)T1MR0 * duty_cycle / 50; //Wypełnienie
    T1IR  = TIMER_ALL_INT;                  //Resetowanie flag przerwań
    T1MCR = MR0_I | MR1_I | MR0_R;          //Generuj okresowe przerwania dla MR0 i dodatkowo dla MR1
    T1TCR = TIMER_RUN;                      //Uruchom timer
}

static void stop_irq() {
	T1TCR = TIMER_RESET;
}

/************************************************
 * Implementacja funkcji						*
 ************************************************/

/***********************************************************
 * Funkcja odpowiedzialna za zapis gry do pamięci trwałej  *
 * Korzystamy tutaj z eepromu 							   *
 ***********************************************************/

void saveGame() {
	int i;
	for(i = 0; i < 216; i+=16) {
		eepromWrite(i, &board[i/18][i%18], sizeof(board[0][0])*16);
		eepromPoll();
	}
}

/***********************************************************
 * Funkcja odpowiedzialna za
 ***********************************************************/

void loadGame() {
	int i;
	for(i = 0; i < 216; i+=16) {
		eepromPageRead(i, &board[i/18][i%18], sizeof(board[0][0])*16);
		eepromPoll();
	}
}

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
		  // nic sie nie dzieje
	  }else if (anyKey == KEY_DOWN) {
		  // nic się nie dzieje
	  }
	}
}

void checkActiveBlockConditions(tBool* serafin) {
	if (detectCollisionDown()) {
		saveGame();
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
		checkFullRows();
		blockTouchSound();
		gameLights();
	}
}

void checkFullRows() {
	int i;
	int j;
	for (i = 0; i < 18; i++) {
		tU8 counter = 0;
		for (j = 0; j < 12; j++) {
			if(board[j][i] == 1) {
				counter ++;
			}
		}
		if (counter == 12) {
			for (j = 0; j < 12; j++) {
				board[j][i] = 0;
			}
		}
	}
}

/*******************************************************************
 * Funkcja odpowiedzialna za inicjalizację planszy				   *
 * Jeżeli to jest nowa gra to nie wczytuje gry z pamięci trwałej   *
 * @params isNewGame											   *
 *******************************************************************/

void initBoard(tBool isNewGame) {
	if (isNewGame) {
		int i;
		int j;
		for (i = 0; i < 12; i++) {
			for (j = 0; j <18; j++) {
				board[i][j] = 0;
			}
		}
	} else {
		loadGame();
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

void playTetris (tBool isNewGame) {
	init_irq(50, 2);
	tBool serafin = TRUE;
	lcdClrscr();
	initBoard(isNewGame);
	while (serafin) {
		checkActiveBlockConditions(&serafin);
		lcdClrscr();
		applyJoystickMovement();
		moveActiveBlock(FALSE, FALSE, TRUE);
		drawBoard();

		osSleep(10);
	}
	stop_irq();
	gameOverMusic();
	gameOverLights();

}
