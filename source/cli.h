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
 *  Nom:	CLI
 *
 *  Autor:	Joan Martínez
 *
 *  Descripcio:   Aquest mòdul implementa les comandes, 
 *             tal com s'han de executar.
 *             (COMMAND LINE INTERFACE)
 ****************************************************/

#ifndef _CLI_
#define _CLI_

/* Defineix la posició on s'emmagatzema 
   el procediment de boot del dispositiu */
#define BOOTVAR_BYTE 511

void cli_init(const uint8_t inici);
int8_t exec_command(const uint8_t * command);

#endif
