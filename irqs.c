#include "LPC17xx.h"
#include <stdint.h>

#define LED4_PIN (23)

char a=1, b=1, c=1;
void inicioPINSEL() {
  LPC_PINCON->PINSEL4 |= 1 << (10 * 2);
  LPC_PINCON->PINSEL4 |= 1 << (11 * 2);
  LPC_PINCON->PINSEL4 |= 1 << (12 * 2);

  LPC_PINCON->PINSEL4 &= ~(3 << (0 * 2));
  LPC_PINCON->PINSEL4 &= ~(3 << (1 * 2));
  LPC_PINCON->PINSEL4 &= ~(3 << (2 * 2));
}

void inicioIRQs() {
  // Asignamos donde se desaloja
  NVIC_SetPriorityGrouping(5);

  // Asignamos la prioridad a las interrupciones
  NVIC_SetPriority(EINT0_IRQn, 0x4);
  NVIC_SetPriority(EINT1_IRQn, 0x6);
  NVIC_SetPriority(EINT2_IRQn, 0x2);

  // Habilitamos las IRQs
  NVIC_EnableIRQ(EINT0_IRQn);
  NVIC_EnableIRQ(EINT1_IRQn);
  NVIC_EnableIRQ(EINT2_IRQn);
}

// ISR de las interrupciones
void EINT0_IRQHandler() {
  LPC_SC->EXTINT |= (1); // Borrar el flag de la EINT0 --> EXTINT.0
  // Activar pin p2.0
  LPC_GPIO2->FIOSET = 0x00000001;
  // Desactivar pin p2.0
  while (a)
    LPC_GPIO2->FIOCLR = 0x00000001;
}

void EINT1_IRQHandler() {
  LPC_SC->EXTINT |= (1 << 1); // Borrar el flag de la EINT0 --> EXTINT.0
  // Activar pin p2.1
  LPC_GPIO2->FIOSET = 0x00000002;
  // Desactivar pin p2.1
  while (a)
    LPC_GPIO2->FIOCLR = 0x00000002;
}

void EINT2_IRQHandler() {
  LPC_SC->EXTINT |= (1 << 2); // Borrar el flag de la EINT0 --> EXTINT.0
  // Activar pin p2.2
  LPC_GPIO2->FIOSET = 0x00000004;
  // Desactivar pin p2.2
  while (a)
    LPC_GPIO2->FIOCLR = 0x00000004;
}
