#include "LPC17xx.h"
#include "aux.h"
#include <stdint.h>

void TIMInit() {
  // We set the MR0 of timer 1 to interrupt and reset the timer
  setmatch(LPC_TIM1, MR_0, MATCH_INT | MATCH_RST);

  LPC_SC->PCLKSEL0 |=
      1 << 5;         // This sets the clock source to be the internal timer / 2
  LPC_TIM1->PR = 50;  // We increase the timer by 1 every 1us
  LPC_TIM1->MR0 = 20; // Set the match register to trigger every 20 us

  LPC_TIM1->TCR |= 1 << 1;    // Reset the timer
  LPC_TIM1->TCR &= ~(1 << 1); // Clear the reset
  LPC_TIM1->TCR |= 1 << 0;    // Start the timer
}

void init() {
	LPC_GPIO1->FIODIR &= ~(0xF<<21);      // Set the pins 21 to 25 as GPIO out
	LPC_PINCON->PINSEL4 |= (1<<(12*2));		// P2.12 (KEY2) as Interrupt Input

	LPC_SC->EXTMODE  |= (1<<1); // Trigger by edge to avoid instances of active
                              // and pending request.
  LPC_SC->EXTPOLAR |= (1<<1); // Set the trigger on rising edge

  // Clear pending interrupts
  NVIC_ClearPendingIRQ(TIMER1_IRQn);
  NVIC_ClearPendingIRQ(EINT2_IRQn);

  NVIC_SetPriorityGrouping(4);        //	0000 | 0000
  NVIC_SetPriority(TIMER1_IRQn, 0x0); //	0010 | xxxx
  NVIC_SetPriority(EINT2_IRQn, 0x1);	//	0100 | xxxx

  NVIC_EnableIRQ(TIMER1_IRQn);	// Enable interruptions from Timer 1
	NVIC_EnableIRQ(EINT2_IRQn);		// Enable interrupts from EINT1
}

void DACInit(void) {
  // Set pin P0.26 as DAC output
  LPC_PINCON->PINSEL1 &= ~(0x1 << 20); // PIN 20 to 0
  LPC_PINCON->PINSEL1 |= (0x1 << 21);  // PIN 21 to 1

  return;
}

void TIMER1_IRQHandler(void) {
  if (sample_idx < 50) {
    LPC_DAC->DACR = sine[sample_idx] << 6;
    sample_idx++;
  } else {
    sample_idx = 0;
  }
}

void EINT2_IRQHandler(void) {
  uint8_t val = LPC_GPIO1->FIOPIN & (0xF << 21) >> 21;

  LPC_SC->EXTINT |= (1 << 2); // Erase EINT2 flag

  if (val == 0)
    LPC_TIM1->MR0 = 200;
  else {
    // We have to set it to the right values
    LPC_TIM1->MR0 = 20 / val;
  }
}

int main (void) {
  DACInit();
  TIMInit();
	init();

  while (1) {}
}
