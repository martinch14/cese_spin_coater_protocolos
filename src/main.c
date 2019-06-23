/*=====[Nombre del modulo]=====================================================
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

/*==================[inclusions]=============================================*/
#include "main.h"
#include "motor_control.h"
#include "board.h"
#include "boards/Board.h"
#include "tmc/TMCMotionController.h"
#include "comm/spi.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

#if defined(TMC5130_eval)
extern void TMC5130_init(void);
#elif defined(TMC4671_eval)
extern void TMC4671_init(void);
#endif
#include "tmc/ic/TMC4671/TMC4671.h"
#include "SysTick.h"
#include <stdint.h>


/*==================[internal functions definition]==========================*/

//Inicializacion General de la Placa y cofiguracion inicial del driver TMC4671

void init(void)
{
//	Inicializacion de la placa EDU-CIAA
	boardConfig();

//	Inicializacion del periferico GPIO
	spi_init();
//	Inicializacion de la UART_USB
	uartConfig(UART_USB,9600);


//	Configuracion de las API de TRINAMIC, las mismas  se encargan de cargas a la capa Board.h con todas las funcionalidades del driver TMC4671
	tmcmotioncontroller_init();
#if defined(TMC5130_eval)
	TMC5130_init();
#elif defined(TMC4671_eval)
	TMC4671_init();
#endif
}


/*==================[external functions definition]==========================*/


int main(void) {

	init();
	Motor_Init();

	while (TRUE) {

//  Control general de las configuraciones que se realizaran sobre el driver del motor
	Motor_Control();

	}

	return 0;
}


/*==================[end of file]============================================*/
