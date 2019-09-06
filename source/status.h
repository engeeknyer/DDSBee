/*
*    This library is free software; you can redistribute it and/or
*    modify it under the terms of the GNU Lesser General Public
*    License as published by the Free Software Foundation; version
*    2.1 of the License.
*    
*    This library is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    Lesser General Public License for more details.
*    
*    You should have received a copy of the GNU Lesser General Public
*    License along with this library; if not, write to the Free Software
*    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*

 ***************************************************
 *  Nom:	STATUS
 *
 *  Autor:	Joan Martínez
 *
 *  Descripcio:   Aquest mòdul implementa els tipus i
 *             les funcions que determinen l'estat del
 *             DDS
 *             
 ****************************************************/


#ifndef _STAT_DDS_
#define _STAT_DDS_

typedef struct{
  /* Definim un nou tipus de estructura 
   que ha de conservar l'estat del DDS. 
   Amb aquesta estructura es possible 
   realitzar canvis i guardar paràmetres 
   en memoria. 
   */
  uint32_t reg_fr0;      // Valor programat en el registre de freq. 0
  uint32_t reg_fr1;      // Valor programat en el registre de freq. 1
  uint16_t reg_ph0;      // Valor programat en el registre de fase 0
  uint16_t reg_ph1;      // Valor programat en el registre de fase 1
  uint8_t stat_wf;       // Forma d'ona predefinida
  uint8_t last_fr;       // Quin és el darrer reg. de freq. usat.
  uint8_t last_ph;       // Quin és el darrer reg. de fase usat.
  boolean out_en;        // Sortida habilitada?
}stat_dds_t;

#endif
