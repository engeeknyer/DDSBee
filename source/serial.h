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
 *  Nom:	Serial
 *
 *  Autor:	Joan Martínez
 *
 *  Descripcio:	Mòdul implementació control sèrie
 *
 ****************************************************/



#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

#define CENTINELLA '\n'

void serial_init(void); 
uint8_t serial_get(void); 
void serial_put(const uint8_t c);
bool serial_can_read(void);
uint8_t serial_print(const uint8_t *msg);
uint8_t serial_write(const uint8_t *msg);

#endif
