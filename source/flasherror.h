#ifndef _FLASHERROR_
#define _FLASHERROR_


  /* Aquestes estructures es deixen en memòria 
     de programa FLASH i no han de  ocupar 
     memòria SRAM de dades*/



#define GET_ERROR(punter,ierror)  strcpy_P(punter,(PGM_P)pgm_read_word(&(error_msg[ierror])))



      /*   Codis d'error    */
const char error_0[] PROGMEM="No error";
const char error_1[] PROGMEM="Unknown command";
const char error_2[] PROGMEM="Wrong parameters";
const char error_3[] PROGMEM="Out of range";
const char error_4[] PROGMEM="Not allowed";

PGM_P const error_msg[]PROGMEM = 
{
    error_0,
    error_1,
    error_2,
    error_3,
    error_4
};




#endif
