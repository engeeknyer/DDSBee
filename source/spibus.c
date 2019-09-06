#include <stdint.h>
#include <avr/io.h>

#include "spibus.h"



/* Master */

void SPI_MasterInit(void){
  /* Set SCK, MOSI & SS as outputs, left unchanged others
   (MISO must!!! be set as input as default of SPI master mode) */
  DDRB = DD_MOSI | DD_SCK | DD_SS;
  PORTB |= DD_SS| DD_MOSI | DD_SCK;

  /* Set mode 2, Enable SPI, set clock, master mode and order 
     The Speed is selected at F_CPU/4 (SPI2X = SPR1 = SPR0 = 0) 
         
      F_spi   | F_CPU
     ---------+--------
       4 MHz  | 16 MHz
     2.5 MHz  | 10 MHz  

  */  
  SPSR &= ~(1<<SPI2X);
  SPCR &= ~((1<<SPIE) | (1<<CPHA) | (1<<DORD) | (1<<SPR1) | (1<<SPR0));  
  SPCR |=  (1<<SPE) | (1<<MSTR) | (1<<CPOL); 
}


uint8_t SPI_MasterTransmit(const uint8_t cData){
  SPDR=cData;

  while(!(SPSR & (1<<SPIF)))
    ;
  return SPDR;
}

void SPI_Master16bTransmit(uint16_t wData){
  SPI_MasterTransmit(wData>>8);
  SPI_MasterTransmit(wData & 0xff);
}

void SPI_Idle(void){
  PORTB |= DD_SS;
}

void SPI_Active(void){
  PORTB &= ~DD_SS;
}


/* Slave */

void SPI_SlaveInit(void){
  /* Enable SPI, Slave set clock and Mode */
  SPCR &= ~((1<<CPHA) | (1<<MSTR) | (1<<DORD)) ;  
  SPCR |= (1<<SPE) | (1<<CPOL);
}

uint8_t SPI_SlaveReceive(void){
  while(!(SPSR & (1<<SPIF)))
    ;
  return SPDR;
}
