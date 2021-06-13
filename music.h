/*************************************************
* Plik:                                           
*     music.h                                    
* Opis:                                          
*   Biblioteka odpowiedzialna za                 
*   wystawienie publicznych funkcji do muzyki    
*************************************************/
#ifdef _MUSIC_H_
#define _MUSIC_H_
/***********************************************
* 
* Include
* 
***********************************************/
#include "../pre_emptive_os/api/general.h"
#include "../startup/lpc2xxx.h"
#include "../startup/config.h"

void gameOverMusic();
void blockTouchSound();
void mainMenuMusic();

#endif
