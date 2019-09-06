#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "gestirq.h"

static nexus_t int0_high;
static nexus_t int0_low;
static nexus_t int1_high;
static nexus_t int1_low;
static nexus_t pcint1_high;
static nexus_t pcint1_low;

void gestirq_init(void){
  /****************************
    FUNCIO INICIALITZACIO IRQ
    --------------------------
     Aquesta funció s'ha de 
  encarregar de inicialitzar les
  condicions de inerrupció generals 
  del microcontrolador 
  *****************************/
  DDRD &= 0xF3;            // INT0 i INT1 configurats com entrades
  EICRA = 0x05;            // Interrupcions en INT0 i INT1 per canvi de nivell


  /*
  DDRC &= 0xC0;            // Port C Com entrades
  PCICR = 0x02;            // Habilitar les interrupcions de PCIE1 (port C)
  PCMSK1 = 0x01;           // Habilita la interrupció unicament del pin 1
  */

  sei();                   // Habilita les inerrupcions generals 
 
}

void INT0_enable(bool active){
  /* habilita o deshabilita INT0 
     de EIMSK */
  if(active) EIMSK |= 0x01;   
  else EIMSK &= 0xFE;         
}

void INT1_enable(bool active){
  /* habilita o deshabilita INT1 
     de EIMSK */
  if(active) EIMSK |= 0x02;   
  else EIMSK &= 0xFD;         
}

void set_INT0(nexus_t fl, nexus_t fh){
  int0_high=fh;
  int0_low=fl;
}

void set_INT1(nexus_t fl, nexus_t fh){
  int1_high=fh;
  int1_low=fl;
}

void set_PCINT1(nexus_t fl, nexus_t fh){
  pcint1_high=fh;
  pcint1_low=fl;
}



 /**********************
    RUTINES DE SERVEI 
      D'INTERRUPCIO
  **********************/
ISR(PCINT1_vect){
  /* CAL REPLANTEJAR LA FUNCIÓ PER 
     TAL D'HABILITAR LA SORTIDA, 
     INDEPENDENTMENT DEL REGISTRE ?¿ */
  /*
  if(PINC & 0x01) pcint1_high();
  else pcint1_low();
  */
}

ISR(INT0_vect){
  /* Utilitza la interrupció INT0 
     per seleccionar un dels dos 
     registres de freqüència. 
     Modulació FSK.             */
  if(PIND & (1<<PIND2)) int0_high();
  else int0_low();
}

ISR(INT1_vect){
  /* Utilitza la interrupció INT1 
     per seleccionar un dels dos 
     registres de fase. Modulació PSK. */
  if(PIND & (1<<PIND3)) int1_high();
  else int1_low();
}
