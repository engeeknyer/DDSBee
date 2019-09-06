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
 *  Nom:	GESTIRQ
 *
 *  Autor:	Joan Martínez
 *
 *  Descripcio:   Aquest mòdul implementa les funcionalitats 
 *             per gestionar les interrupcions produides per
 *             canvis en el mòdul i que han de treballar sobre
 *             el funcionament del DDS.
 ****************************************************/

#ifndef _GESTIRQ_
#define _GESTIRQ_

#include <stdbool.h>

typedef void (*nexus_t)(void);

void set_INT0(nexus_t fl, nexus_t fh);
void set_INT1(nexus_t fl, nexus_t fh);
void set_PCINT1(nexus_t fl, nexus_t fh);
void gestirq_init(void);
void INT0_enable(bool active);
void INT1_enable(bool active);

#endif
