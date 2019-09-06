#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include "queue.h"
#include "serial.h"



//------------------------------------------------------------

static queue_t recieve_q;
static queue_t transmit_q;

//------------------------------------------------------------

void serial_init(void){
  /*Inicialitza el modul i deixa la UART a punt per enviar/rebre
    caracters de 8 bit, amb 1 bit d’stop, sense paritat
    i en mode asíncron.
    La velocitat de transmissió es defineix de forma automàtica 
    a través de les macros que incorpora el mòdul <util/setbaud> 
    i el arxiu Makefile que defineix els parametres de velocitat 
    de CPU i velocitat de BAUD  */

   UBRR0H = UBRRH_VALUE;
   UBRR0L = UBRRL_VALUE;
   UCSR0A = 32;
#if USE_2X
   UCSR0A |= (1 << U2X0);
#else
   UCSR0A &= ~(1 << U2X0);
#endif

  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);                          //Mode,paritat,bits d'estop i mida
  UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0) | _BV(UDRIE0);//Enable rx i tx amb interrupció de rx i UDRE
  queue_empty(&recieve_q);
  queue_empty(&transmit_q);
  UCSR0B |= _BV(RXCIE0);     //inicialment habilitem interrució de recepció
  UCSR0B &= ~(_BV(UDRIE0));  //inicialment deshabilitem interrupció de transmissió
}

//------------------------------------------------------------

uint8_t serial_get(void){
  /*Retorna un byte llegit del port sèrie. Es bloqueja indefinidament
    fins que hi ha un caràcter disponible per a ser llegit*/
  uint8_t r;
  while(queue_is_empty(&recieve_q)){;}
  r = queue_front(&recieve_q);
  queue_dequeue(&recieve_q);
  return r;
}
 
//------------------------------------------------------------

void serial_put(const uint8_t c){
  /*Envia un byte pel port sèrie. En cas que estigui ocupat enviant
    una altra dada, es bloqueja fins que l’enviament en curs acaba */
  while(queue_is_full(&transmit_q)){;}
  queue_enqueue(&transmit_q, c);
  if (!(UCSR0B & _BV(UDRIE0))) UCSR0B |= _BV(UDRIE0);

}

//------------------------------------------------------------

bool serial_can_read(void){
  /*Retorna true si hi ha un caràcter disponible per a ser llegit*/
  return !queue_is_empty(&recieve_q);
}

//------------------------------------------------------------
uint8_t serial_print(const uint8_t *msg){
  /*******************************
     Funció que crida a la funció 
   'serial_put()' per cada un dels 
   elements continguts en l'apuntador 
   *msg, fins trobar el caràcter \0.
   Retorna el nombre de caràcters 
   enviats, sense comptar CENTINELLA.
  *********************************/
  uint8_t i;
  for(i=0; msg[i]!=0;i++)
    serial_put(msg[i]);
  serial_put(CENTINELLA);
  return i;
}

uint8_t serial_write(const uint8_t *msg){
  /*******************************
     Funció que crida a la funció 
   'serial_put()' per cada un dels 
   elements continguts en l'apuntador 
   *msg, fins trobar el caràcter \0.
   Retorna el nombre de caràcters 
   enviats. Permet concatenar sortides
   per el port serie.
  *********************************/
  uint8_t i;
  for(i=0; msg[i]!=0;i++)
    serial_put(msg[i]);
  return i;
}



//------------------------------------------------------------

ISR(USART_RX_vect){
  /*Rutina d'atenció a la interrupció de recepció.
   Encua el byte rebut a la cua de recepció.*/
  queue_enqueue(&recieve_q, UDR0);
}


ISR(USART_UDRE_vect){
  /*Rutina d'atenció a la interrupció de registre de dades buit.
   Desencua el primer byte de la cua de transmissió i l'envia.*/
  uint8_t r;
  r = queue_front(&transmit_q);
  queue_dequeue(&transmit_q);
  UDR0 = r;
  if (queue_is_empty(&transmit_q)) UCSR0B &= ~(_BV(UDRIE0));
}
