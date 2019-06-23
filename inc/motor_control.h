/*=====[motor_control.h]=====================================================
 * Martin Abel Gambarotta <magambarotta@gmail.com>
 * All rights reserved.
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 * Version: 0.0.3
 * Fecha de creacion: 2019/06/10
*/


/*=====[Evitar inclusion multiple comienzo]==================================*/

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

/*=====[Inclusiones de dependencias de funciones publicas]===================*/
#include "board.h"
#include "boards/Board.h"

/*=====[Macros de definicion de constantes publicas]=========================*/
/*=====[Macros estilo funcion publicas]======================================*/
/*=====[Definiciones de tipos de datos publicos]=============================*/
/*=====[Prototipos de funciones publicas]====================================*/


void 	Motor_Init(void);
void 	Motor_Control(void);

/*=====[Prototipos de funciones publicas de interrupcion]====================*/

/*=====[Evitar inclusion multiple fin]=======================================*/
#endif /* _MOTOR_CONTROL_H_ */
