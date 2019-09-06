#include <stdio.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#include "serial.h"
#include "shell.h"
#include "flasherror.h"

#define PROMT "> "             // Estableix promt de la línia de comandes
#define CENTINELLA '\n'        // Estableix caràcter utilitzat com a centinella
#define LEN_MSG_MAX 30         // Estableix la mida màxima dels missatges


uint8_t msg_entrada[LEN_MSG_MAX];
uint8_t * pnt_msg_e=msg_entrada;
uint8_t msg_processat[LEN_MSG_MAX];
uint8_t * pnt_msg_p=msg_processat;
uint8_t punter=0;

static void setup(void);
static void loop(void);

int main(void){
  /*******************************
        PROGRAMA PRINCIPAL
        ------------------
    Estructura el funcionament del 
   programa en la funció setup() i
   la funció loop() 
  ********************************/
  setup();
  while(true) loop();
  return 0;
}

static void setup(void){
  /****************************
         CONFIGURACIONS
         --------------
    Configura les característiques de 
  comunicació i els diferents mòduls 
  per treballar amb ells.
  *****************************/
  serial_init();
  shell_init();
  serial_print(PROMT);
}

static void loop(void){
  /****************************
    PROGRAMA CÍCLIC PRINCIPAL
    -------------------------
  *****************************/

  int8_t value;
  char buffer[20];

  if(serial_can_read()){
    uint8_t c= serial_get();
    if(c!=CENTINELLA){
      msg_entrada[punter++] = c;
    }else{
      msg_entrada[punter] = 0;
      value=process_command(pnt_msg_e);
      if(value<0){
	value=1+~value;
	GET_ERROR(buffer,value);
	serial_print(buffer);
      }
      punter=0;
      serial_print(PROMT);
    }
  }
}
