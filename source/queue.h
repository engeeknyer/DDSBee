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
 *  Nom:	QUEUE
 *
 *  Autor:	Joan Martínez
 *
 *  Descripcio:	Implementació de cues
 *
 ****************************************************/



#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <inttypes.h>


#define MIDA_BUFF 30

typedef struct
{
  uint8_t queue[MIDA_BUFF];
  uint8_t writePointer;
  uint8_t readPointer;
  uint8_t cont;       
} queue_t;

void queue_empty(queue_t *const q);
bool queue_is_empty(const queue_t *const q);
bool queue_is_full(const queue_t *const q);
void queue_enqueue(queue_t *const q, uint8_t v);
void queue_dequeue(queue_t *const q);
uint8_t queue_front(const queue_t *const q);

#endif
