#ifndef _FLASHTXT_
#define _FLASHTXT_

#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>



/* Funcions Macro   */

#define HELP_FRMT &(pgmstr_lst[0])
#define CTREG_FRMT &(pgmstr_lst[1])
#define DUMMY_MSG &(pgmstr_lst[2])
#define PRESETBOOT_MSG &(pgmstr_lst[3])
#define DEFAULTBOOT_MSG &(pgmstr_lst[4])


#define DSOUT &(cmnd_lst[0])
#define ENFR0 &(cmnd_lst[1])
#define ENFR1 &(cmnd_lst[2])
#define ENPHR0 &(cmnd_lst[3])
#define ENPHR1 &(cmnd_lst[4])
#define SRFR0 &(cmnd_lst[5])
#define SRFR1 &(cmnd_lst[6])
/***********/
#define SFR0 &(cmnd_lst[7])
#define SFR1 &(cmnd_lst[8])
#define SPHR0 &(cmnd_lst[9])
#define SPHR1 &(cmnd_lst[10])
#define SINE &(cmnd_lst[11])
#define SQR &(cmnd_lst[12])
#define TRI &(cmnd_lst[13])
/*****/
#define EEOC &(cmnd_lst[14])
#define DEOC &(cmnd_lst[15])
#define SFSC &(cmnd_lst[16])
#define SPSC &(cmnd_lst[17])
#define SFSK &(cmnd_lst[18])
#define SPSK &(cmnd_lst[19])
#define SASK &(cmnd_lst[20])
#define SQAM &(cmnd_lst[21])
#define B202 &(cmnd_lst[22])
#define B103 &(cmnd_lst[23])
/**********/
#define HELP &(cmnd_lst[24])
#define STORE &(cmnd_lst[25])
#define LOAD &(cmnd_lst[26])
#define RBOOT &(cmnd_lst[27])
#define PBOOT &(cmnd_lst[28])
#define SBOOT &(cmnd_lst[29])
/******/
#define TEST1 &(cmnd_lst[30])
#define TEST2 &(cmnd_lst[31])
#define ALIEN &(cmnd_lst[32])
#define CTRL &(cmnd_lst[33])
#define RAW &(cmnd_lst[34])


   /* * *   Funcions MACRO per accedir a les diferents estructures de memòria FLASH  * * */

             /* Accés als missatges i formats genèrics */
#define GET_FLASH(punter,flashm)  strcpy_P(punter,(PGM_P)pgm_read_word(flashm))

             /* Accés i comparació als nom de comandes */
#define GET_F_CMND(punter,flashm)  strcpy_P(punter,(PGM_P)pgm_read_word(&(cmnd_lst[flashm])))
#define CMP_F_CMND(sram, flashm)  strcmp_PF(sram,(PGM_P)pgm_read_word(&(cmnd_lst[flashm])))

             /* Accés al text d'ajuda de les comandes */
#define GET_F_HELP(punter,flashm)  strcpy_P(punter,(PGM_P)pgm_read_word(&(htxt_lst[flashm])))







/********************************/

     /*   Noms de comanda   */
const char cmnd1[] PROGMEM="DSOUT";
const char htxt1[] PROGMEM="Disables Output";
const char cmnd2[] PROGMEM="ENFR0";
const char htxt2[] PROGMEM="Drive freq.reg. 0 to output";
const char cmnd3[] PROGMEM="ENFR1";
const char htxt3[] PROGMEM="Drive freq.reg. 1 to output";
const char cmnd4[] PROGMEM="ENPHR0";
const char htxt4[] PROGMEM="Drive phase reg. 0 to output";
const char cmnd5[] PROGMEM="ENPHR1";
const char htxt5[] PROGMEM="Drive phase reg. 1 to output";
const char cmnd6[] PROGMEM="SRFR0";
const char htxt6[] PROGMEM="Set and run frequency register 0";
const char cmnd7[] PROGMEM="SRFR1";
const char htxt7[] PROGMEM="Set and run frequency register 1";
/***********************/
const char cmnd8[] PROGMEM="SFR0";
const char htxt8[] PROGMEM="Set frequency register 0";
const char cmnd9[] PROGMEM="SFR1";
const char htxt9[] PROGMEM="Set frequency register 1";
const char cmnd10[] PROGMEM="SPHR0";
const char htxt10[] PROGMEM="Set phase register 0";
const char cmnd11[] PROGMEM="SPHR1";
const char htxt11[] PROGMEM="Set phase register 1";
const char cmnd12[] PROGMEM="SINE";
const char htxt12[] PROGMEM="Select sine waveform"; 
const char cmnd13[] PROGMEM="SQR";
const char htxt13[] PROGMEM="Select square waveform (3.3V)"; 
const char cmnd14[] PROGMEM="TRI";
const char htxt14[] PROGMEM="Select triangle waveform"; 
/*****************************/
const char cmnd15[] PROGMEM="EEOC";
const char htxt15[] PROGMEM="Enable external output control"; 
const char cmnd16[] PROGMEM="DEOC";
const char htxt16[] PROGMEM="Disable external output control"; 
const char cmnd17[] PROGMEM="SFSC";
const char htxt17[] PROGMEM="Set frequency shift control"; 
const char cmnd18[] PROGMEM="SPSC";
const char htxt18[] PROGMEM="Set pahse shift control"; 
const char cmnd19[] PROGMEM="SFSK";
const char htxt19[] PROGMEM="Set FSK mode"; 
const char cmnd20[] PROGMEM="SPSK";
const char htxt20[] PROGMEM="Set PSK mode"; 
const char cmnd21[] PROGMEM="SASK";
const char htxt21[] PROGMEM="Set ASK mode"; 
const char cmnd22[] PROGMEM="SQAM";
const char htxt22[] PROGMEM=""; 
const char cmnd23[] PROGMEM="B202";
const char htxt23[] PROGMEM="Set Bell-202 mode"; 
const char cmnd24[] PROGMEM="B103";
const char htxt24[] PROGMEM="Set Bell-103 mode"; 
/********************/
const char cmnd25[] PROGMEM="HELP";
const char htxt25[] PROGMEM="Shows help text"; 
const char cmnd26[] PROGMEM="STORE";
const char htxt26[] PROGMEM="Saves actual status"; 
const char cmnd27[] PROGMEM="LOAD";
const char htxt27[] PROGMEM="Load a preset"; 
const char cmnd28[] PROGMEM="RBOOT";
const char htxt28[] PROGMEM="Reset device on Boot (defaul)"; 
const char cmnd29[] PROGMEM="PBOOT";
const char htxt29[] PROGMEM="Load preset on Boot"; 
const char cmnd30[] PROGMEM="SBOOT";
const char htxt30[] PROGMEM="Gets the Boot status"; 
/*******************/
const char cmnd31[] PROGMEM="TEST1";
const char htxt31[] PROGMEM=""; 
const char cmnd32[] PROGMEM="TEST2";
const char htxt32[] PROGMEM=""; 
const char cmnd33[] PROGMEM="ALIEN";
const char htxt33[] PROGMEM=""; 
const char cmnd34[] PROGMEM="CTRL";
const char htxt34[] PROGMEM=""; 
const char cmnd35[] PROGMEM="RAW";
const char htxt35[] PROGMEM=""; 

#define NUM_CMNDS 35

const char frmt_hlp[] PROGMEM="%8s:   %s";
const char frmt_ctrlreg[] PROGMEM="0x%04X";
const char dummy_txt[] PROGMEM="DuMmY FuNcTiOn.";
const char boot_stxt[] PROGMEM="Boot: preset #%d";
const char boot_dtxt[] PROGMEM="Boot: default";





PGM_P const cmnd_lst[] PROGMEM =
{
  cmnd1,  cmnd2,  cmnd3,  cmnd4,
  cmnd5,  cmnd6,  cmnd7,  cmnd8,
  cmnd9,  cmnd10,  cmnd11,  cmnd12,
  cmnd13,  cmnd14,  cmnd15,  cmnd16,
  cmnd17,  cmnd18,  cmnd19,  cmnd20,
  cmnd21,  cmnd22,  cmnd23,  cmnd24,
  cmnd25,  cmnd26,  cmnd27,  cmnd28,
  cmnd29,  cmnd30,  cmnd31,  cmnd32,
  cmnd33,  cmnd34, cmnd35
};

PGM_P const htxt_lst[] PROGMEM =
{
  htxt1,  htxt2,  htxt3,  htxt4,
  htxt5,  htxt6,  htxt7,  htxt8,
  htxt9,  htxt10,  htxt11,  htxt12,
  htxt13,  htxt14,  htxt15,  htxt16,
  htxt17,  htxt18,  htxt19,  htxt20,
  htxt21,  htxt22,  htxt23,  htxt24,
  htxt25,  htxt26,  htxt27,  htxt28,
  htxt29,  htxt30,  htxt31,  htxt32,
  htxt33,  htxt34, htxt35
};


PGM_P const pgmstr_lst[] PROGMEM =
  {
    frmt_hlp, frmt_ctrlreg, dummy_txt,
    boot_stxt, boot_dtxt
  };


#endif
