#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "flashtxt.h"
#include "cli.h"
#include "serial.h"   
#include "dds.h"

/* Definició dels desplaçaments (SHIFT's) en que 
   es troben els diferents valors en una EEPROM, 
   a partir d'una adreça base*/
#define SFT_FR0 0
#define SFT_FR1 4
#define SFT_PH0 8
#define SFT_PH1 10
#define SFT_CTRL 12
#define SPAN_M 20
/* Nombre màxim de presets permessos */
#define PRST_MAX 3  

extern int arg_c;
extern void * arg_v[4];


/* Llista de comandes  */

typedef int8_t (*operation_t)(void);

typedef struct{
  operation_t fnc;
}cli_command_t;

/*  Comandes que s'executen com a funcions privades a aquest mòdul */
static int8_t command_setrunfr0(void);
static int8_t command_setrunfr1(void);
static int8_t command_setfr0(void);
static int8_t command_setfr1(void);
static int8_t command_setph0(void);
static int8_t command_setph1(void);
static int8_t command_sine(void);
static int8_t command_square(void);
static int8_t command_triangle(void);
static int8_t command_enableControl(void);
static int8_t command_disableControl(void);
static int8_t command_setfsk(void);
static int8_t command_setask(void);
static int8_t command_setpsk(void);
static int8_t command_setqam(void);
static int8_t command_bell202(void);
static int8_t command_bell103(void);
static int8_t reset_on_init(void);
static int8_t preset_on_init(void);
static int8_t status_boot(void);
static int8_t help_fnc(void);
static int8_t command_store(void);
static int8_t command_load(void);

/* Funcions ocultes i de test */
static int8_t set_ddsraw(void);
static int8_t print_controlreg(void);
static int8_t nofares(void);
static int8_t command_test1(void);
static int8_t command_test2(void);
static int8_t close_encounters(void);

/* Comandes/Funcions interrupció */
static void frequency0(void);
static void frequency1(void);
static void enable0(void);
static void enable1(void);
static void disable_out(void);
static void start_out(void);
static void phase0(void);
static void phase1(void);
static void qam0_0(void);
static void qam0_1(void);
static void qam1_0(void);
static void qam1_1(void);

static int8_t load_from_e2prom(uint8_t inici);

const cli_command_t commands[]={
       /* Comandes d'actuació */
  {disable_out},                   
  {enable0},                   
  {enable1},
  {phase0},
  {phase1},
  {command_setrunfr0},
  {command_setrunfr1},

       /* Comandes de programació */
  {command_setfr0},
  {command_setfr1},
  {command_setph0},
  {command_setph1},
  {command_sine},
  {command_square},
  {command_triangle},

       /* Comandes de comportament */
  {command_enableControl},
  {command_disableControl},
  {nofares},
  {nofares},
  {command_setfsk},
  {command_setpsk},
  {command_setask},
  {command_setqam},   // EXPERIMENTAL:  "Set QAM Mode"
  {command_bell202},
  {command_bell103},

        /* Altres Comandes */
  {help_fnc}, 
  {command_store}, 
  {command_load}, 
  {reset_on_init}, 
  {preset_on_init}, 
  {status_boot}, 
  
       /* Comandes ocultes i  per el procés de debugging   */
  {command_test1},
  {command_test2},
  {close_encounters},
  {print_controlreg},
  {set_ddsraw},

	//Afegir nous comandaments aqui.
  {NULL}
};

/**********************
 *  Funcions-Comanda  *
 **********************/

static int8_t command_enableControl(void){
  /* Comanda que habilita l'execució 
     d'interrupcions per INT0 */
  INT0_enable(true);
  return 0;
}

static int8_t command_disableControl(void){
 /* Comanda que deshabilita l'execució 
     d'interrupcions per INT0 */
  INT0_enable(false);
  return 0;
}

static int8_t command_sine(void){
  /* Estableix que la sortida sigui senoidal */
  DDS_Active();
  DDS_Sine();
  DDS_Idle();
  return 0;
}
static int8_t command_square(void){
  /* Estableix que la sortida sigui cuadrada */
  DDS_Active();
  DDS_Square();
  DDS_Idle();
  return 0;
}

static int8_t command_triangle(void){
  /* Estableix que la sortida sigui triangular */
  DDS_Active();
  DDS_Triangle();
  DDS_Idle();
  return 0;
}

static int8_t command_setrunfr0(void){
  /* Comprova que la funció està acompanyada d'un 
     únic argument i posa en marxa el registre de
     freqüència 0 amb el valor passat com a paràmetre */
  if (arg_c==2){
    DDS_Active();
    int8_t value=DDS_SetFreq_0(*(uint32_t *)arg_v[1]);
    DDS_Start_0();
    DDS_Idle();
    return value;
  } 
  else
    return -2;
}

static int8_t command_setfr0(void){
  /* Comprova que la funció està acompanyada d'un 
     únic argument i programa el registre de 
     freqüència 0 amb el valor passat com a paràmetre */
  if (arg_c==2){
    DDS_Active();
    int8_t value=DDS_SetFreq_0(*(uint32_t *)arg_v[1]);
    DDS_Idle();
    return value;
  }
  else
    return -2;
}

static int8_t command_setrunfr1(void){
  /* Comprova que la funció està acompanyada d'un 
     únic argument i posa en marxa el registre de
     freqüència 1 amb el valor passat com a paràmetre */
  if (arg_c==2){
    DDS_Active();
    int8_t value=DDS_SetFreq_1(*(uint32_t *)arg_v[1]);
    DDS_Start_1();
    DDS_Idle();
    return value;
  }
  else
    return -2;
}

static int8_t command_setfr1(void){
  /* Comprova que la funció està acompanyada d'un 
     únic argument i programa el registre de 
     freqüència 1 amb el valor passat com a paràmetre */
  if (arg_c==2){
    DDS_Active();
    uint8_t value=DDS_SetFreq_1(*(uint32_t *)arg_v[1]);
    DDS_Idle();
    return value;
  }
  else
    return -2;
}

static int8_t command_setph0(void){
  /* Comprova que la funció està acompanyada d'un 
     únic argument i programa el registre de 
     fase 0 amb el valor passat com a paràmetre */
  if (arg_c==2){
    DDS_Active();
    uint8_t value=DDS_SetPhase_0(*(uint16_t *)arg_v[1]);
    DDS_Idle();
    return value;
  }
  else
    return -2;
}

static int8_t command_setph1(void){
  /* Comprova que la funció està acompanyada d'un 
     únic argument i programa el registre de 
     fase 1 amb el valor passat com a paràmetre */
  if (arg_c==2){
    DDS_Active();
    uint8_t value=DDS_SetPhase_1(*(uint16_t *)arg_v[1]);
    DDS_Idle();
    return value;
  }
  else
    return -2;
}

static int8_t command_test1(void){
  /* Funció mutant de debugging  */

  return 0;
}

static int8_t command_test2(void){
  /* Funció mutant de debugging  */

  return 0;
}

static int8_t command_bell202(void){
  /* Programa els registres de freqüència amb 
     els valors nominals del estàndard BELL-202 */
  DDS_Active();
  DDS_Reset();
  DDS_SetFreq_0(1200);
  DDS_SetFreq_1(2200);
  DDS_Idle();
  return 0;
}

static int8_t command_bell103(void){
  /* Programa els registres de freqüència amb 
     els valors nominals del estàndard BELL-103 */
  DDS_Active();
  DDS_Reset();
  DDS_SetFreq_0(1070);
  DDS_SetFreq_1(1270);
  DDS_Idle();
  return 0;
}

static int8_t command_setfsk(void){
    /* Comprova que la funció està acompanyada de dos 
       arguments i programa els dos registres de freqüència
       amb els valors lliurats com a paràmetres. 
       A més estableix la funció a la que han de contestar 
       els terminals de control per interrupció i habilita 
       aquestes interrupcions.
       Retorna (-2) en cas que no tinguem tots els arguments.
    */
  if (arg_c==3){
    DDS_Active();
    DDS_Reset();
    int8_t value = DDS_SetFreq_0(*(uint32_t *)arg_v[1]);
    value = value | DDS_SetFreq_1(*(uint32_t *)arg_v[2]);
    DDS_SetPhase_0(0);
    DDS_SetPhase_1(0);
    DDS_Idle();

    set_INT0(disable_out, start_out);
    set_INT1(frequency1, frequency0);    

    INT0_enable(true);
    INT1_enable(true);
    // TODO: Quedarà a deshabilitar totes les interrupcions externes que no siguin necessàries
    return value;
  }
  else
    return -2;
}

static int8_t command_setpsk(void){
    /* Comprova que la funció està acompanyada de dos 
       arguments i programa els dos registres de fase
       amb els valors lliurats com a paràmetres. 
       A més estableix la funció a la que han de contestar 
       els terminals de control per interrupció i habilita 
       aquestes interrupcions.
       Retorna (-2) en cas que no tinguem tots els arguments.
    */
  if (arg_c==3){
    DDS_Active();
    DDS_Reset();
    int8_t value = DDS_SetPhase_0(*(uint16_t *)arg_v[1]);
    value = value | DDS_SetPhase_1(*(uint16_t *)arg_v[2]);
    DDS_Idle();
    set_INT0(disable_out, enable0);
    set_INT1(phase1, phase0);    

    INT0_enable(true);
    INT1_enable(true);
    // TODO: Quedarà a deshabilitar totes les interrupcions externes que no siguin necessàries
    return value;
  }
  else
    return -2;
}



static int8_t command_setqam(void){
    /* Comprova que la funció està acompanyada d'un
       únic argument i programa el registres de freqüència 1
       amb el valor lliurat com a paràmetre.
       A més estableix la funció a la que ha de contestar
       el terminal de control per interrupció i habilita
       aquesta interrupció.
       Retorna (-2) en cas que no tinguem tots els arguments.
    */
  if (arg_c==2){
    DDS_Active();
    DDS_Reset();
    int8_t value = DDS_SetFreq_1(*(uint32_t *)arg_v[1]);
    DDS_Phase_0();
    DDS_Idle();
    // Estableix la funció dels pins de interrupció
    set_INT0(qam0_1, qam0_0);
    set_INT1(qam1_1, qam1_0);
    // Habilita els pins de interrupció
    INT0_enable(true);
    INT1_enable(true);
    // TODO: Quedarà a deshabilitar totes les interrupcions externes que no siguin necessàries
    return value;
  }
  else
    return -2;
}



static int8_t command_setask(void){
    /* Comprova que la funció està acompanyada d'un 
       únic argument i programa el registres de freqüència 1
       amb el valor lliurats com a paràmetre. 
       A més estableix la funció a la que ha de contestar 
       el terminal de control per interrupció i habilita 
       aquesta interrupció.
       Retorna (-2) en cas que no tinguem tots els arguments.
    */
  if (arg_c==2){
    DDS_Active();
    DDS_Reset();
    int8_t value = DDS_SetFreq_1(*(uint32_t *)arg_v[1]);
    DDS_SetPhase_1(0);
    DDS_Idle();
    // Estableix la funció dels pins de interrupció
    set_INT0(disable_out, enable1);
    // Habilita els pins de interrupció
    INT0_enable(true);
    INT1_enable(false);
    // TODO: Quedarà a deshabilitar totes les interrupcions externes que no siguin necessàries
    return value;
  }
  else
    return -2;
}

static int8_t help_fnc(void){
  /*  Explora totes les comandes i retorna 
      el text de informació associat a cada 
      una d'elles. Aquesta funció incorpora
      un mecanisme per tal de poder ocultar 
      algunes comandes al usuari. Aixó permet 
      incorporar comandes de control intern 
      i debugging
  */
  char txt_help[50];
  char buff_cmd[7];
  char buff_hlp[40];
  char txt_help_format[11];
  GET_FLASH(txt_help_format,HELP_FRMT);
  for (uint8_t i=0; i<NUM_CMNDS; i++){   
    GET_F_HELP(buff_hlp,i);
    if (*buff_hlp != 0){     
      GET_F_CMND(buff_cmd,i);
      sprintf(txt_help, txt_help_format, buff_cmd, buff_hlp);
      serial_print(txt_help);
    }
  }
  return 0;
}

static int8_t print_controlreg(void){
  /* Instrucció que retorna el contingut de
     la copia del registre de control del DDS.
     La copia retornada no té perque coincidir 
     exactament amb el contingut real del 
     registre del DDS en el cas que s'hagi 
     utilitzat la comanda set_ddsraw() prèviament.
     Aquesta funció està associada a una 
     comanda oculta.
  */
  char txt[9];
  char cntrl_format[9];
  GET_FLASH(cntrl_format,CTREG_FRMT);
  sprintf(txt,cntrl_format, DDS_GetCtlReg());
  serial_print(txt);
  return 0;
}

static int8_t set_ddsraw(void){
  /* Aquesta funció permet programar el DDS de 
     manera directa. Permet introduir un nombre 
     variable de arguments fins a un màxim del 
     espai que es tingui reservat per arguments. 
     Els arguments lliurats han de estar en 
     format decimal.
     Aquesta funció està associada a una 
     comanda oculta.     
  */
  if (arg_c>1){
    DDS_Active();
    for (int i=1; i<arg_c; i++){
      DDS_Master16bTransmit(*(uint32_t *)arg_v[i]);
    }
    DDS_Idle();
    return 0;
  }
  else
    return -2;
}

static int8_t nofares(void){
  /* Funció 'dummy' per farcir. Te com a objectiu
     poder-la assignar a les comandes que encara 
     no tenen una funció definida 
  */
  char dtxt[19];
  GET_FLASH(dtxt,DUMMY_MSG);
  serial_print(dtxt);
  return 0;
}

 static int8_t close_encounters(void){
  /* Petita funció oculta de
     prova. Una broma, un ou
     de pascua
  */
  DDS_Active();
  DDS_Reset();
  DDS_SetFreq_0(2349);
  DDS_Start_0();
  DDS_Idle();
  _delay_ms(300);

  DDS_Active();
  DDS_Reset();
  DDS_SetFreq_0(2637);
  DDS_Start_0();
  DDS_Idle();
  _delay_ms(300);

  DDS_Active();
  DDS_Reset();
  DDS_SetFreq_0(2093);
  DDS_Start_0();
  DDS_Idle();
  _delay_ms(300);

  DDS_Active();
  DDS_Reset();
  DDS_SetFreq_0(1046);
  DDS_Start_0();
  DDS_Idle();
  _delay_ms(300);

  DDS_Active();
  DDS_Reset();
  DDS_SetFreq_0(1568);
  DDS_Start_0();
  DDS_Idle();

  _delay_ms(1200);
  disable_out();
  return 0;
 } 

static void enable0(void){
  /* Funció que selecciona el registre 
     de freqüència 0 i habilita la sortida */
  DDS_Active();
  DDS_Start_0();
  DDS_Idle();
}

static void enable1(void){
  /* Funció que selecciona el registre 
     de freqüència 1 i habilita la sortida */
  DDS_Active();
  DDS_Start_1();
  DDS_Idle();
}

static void disable_out(void){
  /* Aquesta funció deshabilita la sortida */
  DDS_Active();
  DDS_Reset();
  DDS_Idle();
}

static void frequency0(void){
  /* Funció que selecciona el registre 
     de freqüència 0, sense modificar l'estat
     d'activació de la sortida */
  DDS_Active();
  DDS_Freq_0();
  DDS_Idle();
}

static void frequency1(void){
  /* Funció que selecciona el registre 
     de freqüència 1, sense modificar l'estat
     d'activació de la sortida */
  DDS_Active();
  DDS_Freq_1();
  DDS_Idle();
}

static void phase0(void){
  /* Funció que selecciona el registre 
     de fase 0, sense modificar l'estat
     d'activació de la sortida */
  DDS_Active();
  DDS_Phase_0();
  DDS_Idle();
}

static void phase1(void){
  /* Funció que selecciona el registre 
     de freqüència 1, sense modificar l'estat
     d'activació de la sortida */
  DDS_Active();
  DDS_Phase_1();
  DDS_Idle();
}

static void start_out(void){
  /* Funció que habilita la sortida, sense 
     modificar cap registre. Per tant respecta 
     las condicions de freqüència i fase prèvies */
  DDS_Active();
  DDS_Start();
  DDS_Idle();
}

static void qam0_0(void){
  static uint8_t fase;
  INT1_enable(false);

  if(PIND & (1<<PIND3)) fase=0;
  else fase=180;

  DDS_Active();
  DDS_SetPhase_0(fase);  
  DDS_Idle();

  INT1_enable(true);
}

static void qam0_1(void){
  static uint8_t fase;
  INT1_enable(false);

  if(PIND & (1<<PIND3)) fase=270;
  else fase=90;

  DDS_Active();
  DDS_SetPhase_0(fase);
  DDS_Idle();

  INT1_enable(true);
}

static void qam1_0(void){
  static uint8_t fase;
  INT0_enable(false);

  if(PIND & (1<<PIND2)) fase=90;
  else fase=0;

  DDS_Active();
  DDS_SetPhase_0(fase);
  DDS_Idle();

  INT0_enable(true);
}

static void qam1_1(void){
  static uint8_t fase;
  INT0_enable(false);

  if(PIND & (1<<PIND2)) fase=270;
  else fase=180;

  DDS_Active();
  DDS_SetPhase_0(fase);
  DDS_Idle();

  INT0_enable(true);
}

static int8_t reset_on_init(void){
  eeprom_update_byte((uint8_t*)BOOTVAR_BYTE, 0);
  return 0;
}

static int8_t preset_on_init(void){
  uint8_t inici;
  if (arg_c>2) return -2;
  else if (arg_c==2 && (*(uint32_t *)arg_v[1]>PRST_MAX || *(uint32_t *)arg_v[1]<1)) return -4;
  else{
    if (arg_c==1) inici=1;
    else inici=*(uint32_t *)arg_v[1];
    eeprom_update_byte((uint8_t*)BOOTVAR_BYTE, inici);
    return 0;
  }
}


static int8_t status_boot(void){
  char stboot[19];
  char txt[19];
  const uint8_t bootvar = eeprom_read_byte((uint8_t *)BOOTVAR_BYTE);
  if (bootvar){
    GET_FLASH(stboot,PRESETBOOT_MSG);
    sprintf(txt,stboot,bootvar);
  }
  else {
    GET_FLASH(txt,DEFAULTBOOT_MSG);
  }
  serial_print(txt);
  return 0;
}

static int8_t command_store(void){
  uint8_t inici;
  if (arg_c>2) return -2;
  else if (arg_c==2 && (*(uint32_t *)arg_v[1]>PRST_MAX || *(uint32_t *)arg_v[1]<1)) return -4;
  else{
    if (arg_c==1) inici=1;
    else inici=*(uint32_t *)arg_v[1];

    uint32_t freq0 = DDS_GetFreqR0();
    uint32_t freq1 = DDS_GetFreqR1();
    uint16_t fase0 = DDS_GetPhaseR0();
    uint16_t fase1 = DDS_GetPhaseR1();
    uint16_t ctreg = DDS_GetCtlReg();
    eeprom_update_dword((uint32_t*)(SPAN_M*(inici-1)+SFT_FR0), freq0);
    eeprom_update_dword((uint32_t*)(SPAN_M*(inici-1)+SFT_FR1), freq1);
    eeprom_update_word((uint16_t*)(SPAN_M*(inici-1)+SFT_PH0), fase0);
    eeprom_update_word((uint16_t*)(SPAN_M*(inici-1)+SFT_PH1), fase1);
    eeprom_update_word((uint16_t*)(SPAN_M*(inici-1)+SFT_CTRL), ctreg);
    return 0;
  }
}


static int8_t command_load(void){
 uint8_t inici;
 
 if (arg_c>2) return -2;
 else if (arg_c==2 && (*(uint32_t *)arg_v[1]>PRST_MAX || *(uint32_t *)arg_v[1]<1)) return -4;
 else
   {
     if (arg_c==1) inici=1;
     else inici=*(uint32_t *)arg_v[1];
     return load_from_e2prom(inici);
   }
}


static int8_t load_from_e2prom(uint8_t inici){
  DDS_Active(); 
  int8_t value = DDS_SetFreq_0(eeprom_read_dword((uint32_t*)(SPAN_M*(inici-1)+SFT_FR0)));
  value = value | DDS_SetFreq_1(eeprom_read_dword((uint32_t*)(SPAN_M*(inici-1)+SFT_FR1)));
  value = value | DDS_SetPhase_0(eeprom_read_word((uint16_t*)(SPAN_M*(inici-1)+SFT_PH0)));
  value = value | DDS_SetPhase_1(eeprom_read_word((uint16_t*)(SPAN_M*(inici-1)+SFT_PH1)));
  DDS_SetCtlReg(eeprom_read_word((uint16_t*)(SPAN_M*(inici-1)+SFT_CTRL)));
  DDS_Idle();
  return value;
}


/***********************
 *  Funcions Públiques *
 ***********************/

void cli_init(const uint8_t inici){
  /* Funció d'inicialització del mòdul cli */
  /* Primer inicia DDS de manera incondicional */
  DDS_Init();   
  if (inici){   
    /* Si és diferent de 0, carrega un preset des de 
       l'E2PROM i el programa en el DDS */
    load_from_e2prom(inici);
  }
}

int8_t exec_command(const uint8_t * command) {
  /* Funció que rep un apuntador a una cadena de 
     caracters i explora totes les comandes integrades 
     en el CLI. Si existeix una funció associada 
     a la comanda entrada, la executa inmmediatament. 
     En cas contrari retorna el error (-1) per marcar
     la funció com desconeguda.  */

  uint8_t i;
  if (*command==0) return 0;
  for (i=0; i<NUM_CMNDS; i++){ 
    if (!CMP_F_CMND(command, i))
      {
	if (commands[i].fnc) {          
	  return commands[i].fnc();
	}
      }
  }
  return -1;  
}
