#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include <ea_init.h>
#include <stdlib.h>

#include "lcd.h"
#include "key.h"
#include "uart.h"
#include "exampleGame.h"
#include "hw.h"
#include "version.h"
#include "configAppl.h"
#include "tetris.h"
#include "mainMenu.h"

#define PROC1_STACK_SIZE 800
#define INIT_STACK_SIZE  600

volatile tU32 ms;

static tU8 proc1Stack[PROC1_STACK_SIZE];
static tU8 initStack[INIT_STACK_SIZE];
static tU8 pid1;

static void proc1(void* arg);
static void initProc(void* arg);

int
main(void)
{
  tU8 error;
  tU8 pid;
  
  osInit();

  immediateIoInit();
  
  osCreateProcess(initProc, initStack, INIT_STACK_SIZE, &pid, 1, NULL, &error);
  osStartProcess(pid, &error);
  
  osStart();
  return 0;
}

static void proc1(void* arg)
{
  //shortly bleep with the buzzer and flash with the LEDs
  tU8 i,j;

  for(i=0; i<3; i++)
  {
    for(j=0; j<3; j++)
    {
      setBuzzer(TRUE);
      setLED(LED_GREEN, FALSE);
      setLED(LED_RED,   TRUE);
      osSleep(1);

      setBuzzer(FALSE);
      setLED(LED_GREEN, TRUE);
      setLED(LED_RED, FALSE);
      osSleep(1);
    }
    osSleep(10);
  }
  setLED(LED_GREEN, FALSE);
  setLED(LED_RED,   FALSE);

  //display startup message
  resetLCD();
  lcdInit();

  //print menu
  drawMainMenu();
}

static void
initProc(void* arg)
{
  tU8 error;

  eaInit();
  printf("\n*********************************************************");
  printf("\n*                  aplikacja tetris                     *");
  printf("\n*                wykonana jako projekt                  *");
  printf("\n*                    na przedmiot                       *");
  printf("\n*                  systemy wbudowane                    *");
  printf("\n*                                                       *");
  printf("\n*                       tworcy:                         *");
  printf("\n*                    Michal Gebels                      *");
  printf("\n*                 Piotr Tomczaszewski                   *");
  printf("\n*                  Antoni Karwowsky                     *");
  printf("\n*********************************************************\n");
  
  osCreateProcess(proc1, proc1Stack, PROC1_STACK_SIZE, &pid1, 3, NULL, &error);
  osStartProcess(pid1, &error);
  initKeyProc();
  osDeleteProcess();
}

void
appTick(tU32 elapsedTime)
{
  ms += elapsedTime;
}





