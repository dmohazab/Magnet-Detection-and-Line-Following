#include "mte220.c" //Must be included, C functions needed for labs
#include <stdbool.h>

#define DELAY_LENGTH (uns16)160

void main(void){

  Initialization(); //Sets up ADCs, timing, interrupts
  WaitForButton(); //Waits until the button is pressed before starting

  for (int i = 0; i < 7; i++) {
    OnLED
    LongDelay(4000);
    OffLED
    LongDelay(4000)
  }

  OnLED
  LongDelay(56000);
  OffLED
}
