#include <stdio.h>
#include <ctype.h>
#include <avr/eeprom.h>

#include "shell.h"
#include "cli.h"
#include "gestirq.h"

char comanda[8];
uint32_t arg_lst[3];
int arg_c;
void * arg_v[4];

static uint8_t boot_device(void);

int8_t process_command(const uint8_t *command){
  if (*command==0) return 0;
  arg_c=0;
  arg_c=sscanf(command,"%7s %lu %lu %lu",arg_v[0],arg_v[1],arg_v[2],arg_v[3]);
  if (!arg_c) return 0;
  else{
    return exec_command(arg_v[0]);
  }
}

void shell_init(void){
  arg_v[0]=comanda;
  arg_v[1]=&arg_lst[0];
  arg_v[2]=&arg_lst[1];
  arg_v[3]=&arg_lst[2];
  uint8_t bootv=boot_device();
  cli_init(bootv);
  gestirq_init();
}

static uint8_t boot_device(void){
  /* Llegeix el contingut de la posició BOOTVAR_BYTE 
     de la E2PROM. Si el valor és 0 carrega 
     el mode de funcionament per defecte. 
     Si no, carrega el preset. */
  uint8_t bootvar = eeprom_read_byte((uint8_t*)BOOTVAR_BYTE);
  if(bootvar==255){
    /* Aquesta part asegura que l'equip funcionarà 
       despres de ser flasexat, asegurant el valor 0 
       dins del BOOTVAR_BYTE */
    eeprom_write_byte((uint8_t*)BOOTVAR_BYTE, 0);
    bootvar=0;
  }
  return bootvar;
}
