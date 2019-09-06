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
 *  Nom:	DDS
 *
 *  Autor:	Joan Martínez
 *
 *  Descripció:	Mòdul control DDS AD9837/AD9833  
 *
 ****************************************************/


#ifndef DDS_AD9837_h
#define DDS_AD9837_h

/* Aquest include es per passar a les capes superiors els 
defines de DD_SS, DD_SCK, etc  */
#include "spibus.h"


#define DDS_Active()  SPI_Active()
#define DDS_Idle()  SPI_Idle()
#define DDS_Master16bTransmit(data) SPI_Master16bTransmit(data)


void DDS_Init(void);
uint16_t DDS_GetCtlReg(void);
uint32_t DDS_GetFreqR0(void);
uint32_t DDS_GetFreqR1(void);
uint16_t DDS_GetPhaseR0(void);
uint16_t DDS_GetPhaseR1(void);

void DDS_Reset(void);
void DDS_Start(void);
void DDS_Start_0(void);
void DDS_Start_1(void);
void DDS_Phase_0(void);
void DDS_Phase_1(void);
void DDS_Freq_0(void);
void DDS_Freq_1(void);
void DDS_Sine(void);
void DDS_Square(void);
void DDS_Triangle(void);

int8_t DDS_SetPhase_0(const uint16_t phase);
int8_t DDS_SetPhase_1(const uint16_t phase);
int8_t DDS_SetFreq_0(uint32_t frequency);
int8_t DDS_SetFreq_1(uint32_t frequency);
void DDS_SetCtlReg(const uint16_t reg);
#endif
