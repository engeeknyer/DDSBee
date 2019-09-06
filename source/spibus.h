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
 *  Nom:	SPI BUS
 *
 *  Autor:	Joan Martínez
 *
 *  Descripcio:	Mòdul control BUS SPI  
 *
 ****************************************************/
#include <stdint.h>


#ifndef SPI_BUS_h
#define SPI_BUS_h

/* SPI Pin Config.
 ---------------
 These are the pin configuration for Arduino with ATMEGA328, 
 ATMEGA168. To be sure about what the pins to use check the 
 documentation of your MCU */
#define DD_SS (1<<DDB2)
#define DD_SCK (1<<DDB5)
#define DD_MOSI (1<<DDB3)
#define DD_MISO (1<<DDB4)


//Master
void SPI_MasterInit(void);
uint8_t SPI_MasterTransmit(const uint8_t cData);
void SPI_Master16bTransmit(uint16_t wData);
void SPI_Active(void);
void SPI_Idle(void);

//Slave
void SPI_SlaveInit(void);
uint8_t SPI_SlaveReceive(void);

#endif
