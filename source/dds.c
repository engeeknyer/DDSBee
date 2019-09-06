#include <stdint.h>
#include <inttypes.h>
#include "spibus.h"
#include "dds.h"

/*   Límits màxims de freq. i fase 
     -----------------------------
     TO-DO: Automatitzar des de compilació
*/
#define DDS_F_MAX 12500000L
#define DDS_PH_MAX 359 


/*   Multiplicadors de Freqüència i Fase
     -----------------------------------
     Es trien diferents multiplicadors en funció de la
freqüència de MCLK. 

  - FREQ_MUL 
    clk= 5MHz 53.6870912
    clk=16Mhz 16.777216f
    clk=25Mhz 10.73741824f

    Per el multiplicador de fase es tria en funció de les
unitats del valor d'entrada
 
    11.3777778f per graus
    651.8986469f per radians
*/
#define FREQ_MUL 10.73741824f
#define PHSE_MUL 11.3777778f

/*  Constants de Registres de Control
    ---------------------------------
  Aquests són els valors de les constants necessàries per 
programar els registres de el DDS.
*/
#define SIN_WF 0x00
#define SQR_WF 0x28
#define TRI_WF 0x02
#define SLEEP1 0x80 

#define RESET_WD 0x0100
#define START_WD 0x2000
#define SET_F_WD 0x2100
#define PHSE_MSK 0xC000

#define FSEL0 0x0000
#define FSEL1 0x0800

#define PSEL0 0x0000
#define PSEL1 0x0400


#define FSEL_MSK 0xF7FF
#define PSEL_MSK 0xFBFF
#define RST_MSK 0xFEFF
#define SLEP_MSK 0xFF7F
#define WF_MSK 0xFFD5

#define START_F0 START_WD|FSEL0
#define START_F1 START_WD|FSEL1
#define START_MSK 0xD6FF

/*  Constants dels Registres de Frequencies
    ---------------------------------------
  Per seleccionar en quin registre (0 o 1) volem programar les paraules de 
14 bits (LSB o MSB).
*/
#define FREQ0 0x4000
#define FREQ1 0x8000

/*  Constants dels Registres de Fase
    --------------------------------
  Per seleccionar en quin registre (0 o 1) volem programar les paraules de 
12 bits per la fase.
*/
#define PHASE0 0xC000
#define PHASE1 0xE000


uint16_t control_reg=START_WD;
uint32_t freq0_reg;
uint32_t freq1_reg;
uint16_t fase0_reg;
uint16_t fase1_reg;



/*   CAPÇALERES FUNCIONS PRIVADES   
     ----------------------------           */
static int8_t DDS_SetDirectFrequency(uint32_t frequency, const uint16_t freq_reg);
static int8_t DDS_SetFrequency(uint32_t frequency, const uint16_t freq_reg);
static int8_t DDS_SetPhase(const uint16_t phase, const uint16_t phase_reg);
static int8_t DDS_ChgControlReg(const uint16_t reg_mask, const uint16_t reg_value);



/*     FUNCIONS PÚBLIQUES
       ------------------                       */
uint32_t DDS_GetFreqR0(void){
  /* Retorna el contingut de la copia del 
     registre de freqüència del DDS  */
  return freq0_reg;
}

uint32_t DDS_GetFreqR1(void){
  /* Retorna el contingut de la copia del 
     registre de freqüència 1 del DDS  */
  return freq1_reg;
}

uint16_t DDS_GetPhaseR0(void){
  /* Retorna el contingut de la copia del 
     registre de fase 0 del DDS  */
  return fase0_reg;
}

uint16_t DDS_GetPhaseR1(void){
  /* Retorna el contingut de la copia del 
     registre de fase 1 del DDS  */
  return fase1_reg;
}

uint16_t DDS_GetCtlReg(void){
  /* Retorna el contingut de la copia del 
     registre de control del DDS  */
  return control_reg;
}

void DDS_Reset(void){
  /* Resets output (not registers)
     output is held in middle value */
  DDS_ChgControlReg(RST_MSK, RESET_WD);
}

void DDS_Start(void){
  /* Posa en marxa la sortida mantenint 
     els bits PSEL FSEL sense alterar */
  DDS_ChgControlReg(RST_MSK, START_WD);
}


void DDS_SetCtlReg(const uint16_t reg){
  /* Estableix el valor de registre de 
     control i de la seva copia */
  DDS_ChgControlReg(0x0000, reg);
}

void DDS_Init(void){
  /* Prepara Master Device 
     i atura DDS */ 
  SPI_MasterInit();
  SPI_Active();
  DDS_Reset();
  SPI_Idle();
}

void DDS_Start_0(void){
  /* Enables DDS Output */
  DDS_ChgControlReg(START_MSK, START_F0);
}

void DDS_Start_1(void){
  /* Enables DDS Output */
  DDS_ChgControlReg(START_MSK, START_F1);
}


void DDS_Freq_0(void){
  /* Select FR0 */
  DDS_ChgControlReg(FSEL_MSK, FSEL0);
}

void DDS_Freq_1(void){
  /* Select FR1 */
  DDS_ChgControlReg(FSEL_MSK, FSEL1);
}

void DDS_Phase_0(void){   
  DDS_ChgControlReg(PSEL_MSK, PSEL0);
}

void DDS_Phase_1(void){
  DDS_ChgControlReg(PSEL_MSK, PSEL1);
}

void DDS_Sine(void){ 
  DDS_ChgControlReg(WF_MSK, SIN_WF);
}

void DDS_Square(void){
  DDS_ChgControlReg(WF_MSK, SQR_WF);
}

void DDS_Triangle(void){
  DDS_ChgControlReg(WF_MSK, TRI_WF);
}


int8_t DDS_SetPhase_0(const uint16_t phase){
  /* Estableix el valor de fase del registre de fase 0 */
  int8_t value=DDS_SetPhase(phase, PHASE0);
  if (!value) fase0_reg=phase;
  return value;
}

int8_t DDS_SetPhase_1(const uint16_t phase){
  /* Estableix el valor de fase del registre de fase 1 */
  int8_t value=DDS_SetPhase(phase, PHASE1);
  if (!value) fase1_reg=phase;
  return value;
}

int8_t DDS_SetFreq_0(uint32_t frequency){
  /* Estableix el valor de frequencia del registre de frequencia 0 */
  int8_t value=DDS_SetFrequency(frequency, FREQ0);
  if (!value) freq0_reg=frequency;
  return value;
}

int8_t DDS_SetFreq_1(uint32_t frequency){
  /* Estableix el valor de frequencia del registre de frequencia 1 */
  int8_t value=DDS_SetFrequency(frequency, FREQ1);
  if (!value) freq1_reg=frequency;
  return value;
}


/*     FUNCIONS PRIVADES
       -----------------                       */
int8_t DDS_SetDirectFrequency(uint32_t frequency, const uint16_t freq_reg){
  /* Sets a new frequency without 
     reseting the output */
  if(frequency<=12500000){
    uint32_t divisor=frequency*FREQ_MUL;
    SPI_Master16bTransmit((divisor & 0x3fff)| freq_reg);
    SPI_Master16bTransmit((divisor >> 14) | freq_reg);
    return 0;
  }
  else{
    return -3;
  }
}

int8_t DDS_SetFrequency(uint32_t frequency, const uint16_t freq_reg){
  /* Sets a new frequency reseting 
     previously the output */
    SPI_Master16bTransmit(SET_F_WD);
    return DDS_SetDirectFrequency(frequency,freq_reg);
}

int8_t  DDS_SetPhase(const uint16_t phase, const uint16_t phase_reg){
  /* Sets a new phase */
  if(phase<=DDS_PH_MAX){
    SPI_Master16bTransmit((uint16_t)(phase*PHSE_MUL) | phase_reg);
    return 0;
  }
  else
    return -3;
}

static int8_t DDS_ChgControlReg(const uint16_t reg_mask, const uint16_t reg_value){
  /* Funció que s'encarrega de modificar el registre de control 
     del DDS i guardar una copia */
  control_reg = (control_reg & reg_mask) | reg_value;
  SPI_Master16bTransmit(control_reg);
  return 0;
}
