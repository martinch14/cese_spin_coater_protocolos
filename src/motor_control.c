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

/*=====[Inclusion de su propia cabecera]=====================================*/

#include "motor_control.h"
/*=====[Inclusiones de dependencias de funciones privadas]===================*/
#include "board.h"
#include "boards/Board.h"
#include "tmc/TMCMotionController.h"
#include "tmc/ic/TMC4671/TMC4671.h"
#include "sapi.h"


/*=====[Macros de definicion de constantes privadas]=========================*/
/*=====[Macros estilo funcion privadas]======================================*/
/*=====[Definiciones de tipos de datos privados]=============================*/

typedef enum {
	MODO_DESHABILITADO,
	MODO_OPENLOOP,
	MODO_TORQUE_FLUJO_HALL,
	MODO_TORQUE_FLUJO_ABN_ENCODER

}modoMotor_t;

/*=====[Definiciones de Variables globales publicas externas]================*/
/*=====[Definiciones de Variables globales publicas]=========================*/

static tick_t old_tick = 0;
uint8_t data_uart = '0';

/*=====[Definiciones de Variables globales privadas]=========================*/
/*=====[Prototipos de funciones privadas]====================================*/

void Motor_Loop(void);

//Funciones para la utilizacion del motor en modo OpenLoop , significa que no tiene realimentacion de los encoder, solo se aplica una pequeña corriente a las bobinas, es para hacer las primeras pruebas

//Configuracion de todos los registros para el funcionamiento del motor en modo OPENLOOP
void Motor_Config_OpenLoop(void);
//Configuracion de velocidad para modo OPENLOOP
void Motor_Velocity_OpenLoop(uint32_t value);
//Borra todos los registros del modo OPENLOOP
void Motor_Config_OpenLoop_Delete(void);


//Funciones para la utilizacion del motor en modo Torque Flujo, se realiza un control FOC (Field Oriented Control ),se utiliza la realimentacion del endoder Digital Hall

//Configuracion de todos los registros para el funcionamiento del motor en modo TORQUE_FLUJO_HALL con Encoder Digital Hall
void Motor_Config_Torque(void);
//Configuracion de velocidad para modo TORQUE_FLUJO_HALL con Encoder Digital Hall
void Motor_Velocity_Torque(uint32_t value);
//Borra todos los registros del modo TORQUE_FLUJO_HALL
void Motor_Config_Torque_Delete(void);


//Funciones para la utilizacion del motor en modo Torque Flujo, se realiza un control FOC (Field Oriented Control ),se utiliza la realimentacion con el ABN  Encoder incremental  pensado para mejor funcionamiento en altas velocidades

//Configuracion de todos los registros para el funcionamiento del motor en modo TORQUE_FLUJO_ABN_ENCODER con Encoder Incremental ABN
void Motor_Config_Torque_ABN_Encoder(void);
//Configuracion de velocidad para modo TORQUE_FLUJO_ABN_ENCODER
void Motor_Velocity_Torque_ABN_Encoder(uint32_t value);
//Borra todos los registros del modo TORQUE_FLUJO_ABN_ENCODER
void Motor_Config_Torque_ABN_Encoder_Delete(void);




/*=====[Implementaciones de funciones publicas]==============================*/




void Motor_Init(void){
	Evalboards.driverEnable = DRIVER_ENABLE;
	Evalboards.ch1.enableDriver(DRIVER_USE_GLOBAL_ENABLE);
}


void  Motor_Control( void )
{
	static modoMotor_t modo;

	Motor_Loop();

	if(uartReadByte(UART_USB, &data_uart)){
	if (data_uart == '0'){
			modo = MODO_DESHABILITADO;
			uartWriteString(UART_USB,"0)MODO DESHABILITADO\r\n");
			Motor_Config_OpenLoop_Delete();
			Motor_Config_Torque_Delete();
			Motor_Config_Torque_ABN_Encoder_Delete();
		}
		if (data_uart == '1'){
			modo = MODO_OPENLOOP;
			uartWriteString(UART_USB,"1)MODO OPEN_LOOP\r\n");
			Motor_Config_OpenLoop_Delete();
			Motor_Config_Torque_Delete();
			Motor_Config_Torque_ABN_Encoder_Delete();
		}
		if (data_uart == '2'){
			modo = MODO_TORQUE_FLUJO_HALL;
			uartWriteString(UART_USB,"2)MODO TORQUE_FLUJO_HALL\r\n");
			Motor_Config_OpenLoop_Delete();
			Motor_Config_Torque_Delete();
			Motor_Config_Torque_ABN_Encoder_Delete();
		}
		if (data_uart == '3'){
			modo = MODO_TORQUE_FLUJO_ABN_ENCODER;
			uartWriteString(UART_USB,"3)MODO TORQUE_FLUJO_ABN_ENCODER\r\n");
			Motor_Config_OpenLoop_Delete();
			Motor_Config_Torque_Delete();
			Motor_Config_Torque_ABN_Encoder_Delete();
		}
	}


	if (modo == MODO_OPENLOOP) {

		if (0 == gpioRead(TEC1)){
			Motor_Config_OpenLoop();
		}
		if (0 == gpioRead(TEC2)){
			Motor_Velocity_OpenLoop(0UL);
		}
		if (0 == gpioRead(TEC3)){
			Motor_Velocity_OpenLoop(0x0000003C);
		}
		if (0 == gpioRead(TEC4)){
			Motor_Config_OpenLoop_Delete();
		}

	}

	if (modo == MODO_TORQUE_FLUJO_HALL) {

		if (0 == gpioRead(TEC1)){
			Motor_Config_Torque();
		}
		if (0 == gpioRead(TEC2)){
			Motor_Velocity_Torque(0UL);
		}
		if (0 == gpioRead(TEC3)){
			Motor_Velocity_Torque(0x03E80000);
		}
		if (0 == gpioRead(TEC4)){
			Motor_Config_Torque_Delete();
		}

	}


	if (modo == MODO_TORQUE_FLUJO_ABN_ENCODER) {

		if (0 == gpioRead(TEC1)){
			Motor_Config_Torque_ABN_Encoder();
		}
		if (0 == gpioRead(TEC2)){
			Motor_Velocity_Torque_ABN_Encoder(0UL);
		}
		if (0 == gpioRead(TEC3)){
			Motor_Velocity_Torque_ABN_Encoder(0xFC180000);
		}
		if (0 == gpioRead(TEC4)){
			Motor_Config_Torque_ABN_Encoder_Delete();
		}

	}


	old_tick = tickRead();
	while (old_tick == tickRead());



}


/*=====[Implementaciones de funciones privadas]==============================*/

void Motor_Loop(void){
	Evalboards.ch1.periodicJob(tickRead());
}

void Motor_Config_OpenLoop(void){

	// Motor type &  PWM configuration
	Evalboards.ch1.writeRegister(0, TMC4671_MOTOR_TYPE_N_POLE_PAIRS, 0x00030004);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_POLARITIES, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_MAXCNT, 0x00000F9F);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_BBM_H_BBM_L, 0x00000505);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_SV_CHOP, 0x00000007);

	// ADC configuration
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I_SELECT, 0x18000100);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCFG_B_MCFG_A, 0x00100010);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCLK_A, 0x20000000);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCLK_B, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MDEC_B_MDEC_A, 0x014E014E);
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I0_SCALE_OFFSET, 0x0100825D);
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I1_SCALE_OFFSET, 0x010082CE);

	// Open loop settings
	Evalboards.ch1.writeRegister(0, TMC4671_OPENLOOP_MODE, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_OPENLOOP_ACCELERATION, 0x0000003C);
	Evalboards.ch1.writeRegister(0, TMC4671_OPENLOOP_VELOCITY_TARGET, 0x00000000);

	// Feedback selection
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_SELECTION, 0x00000002);
	Evalboards.ch1.writeRegister(0, TMC4671_UQ_UD_EXT, 0x00000906);

	// Switch to open loop velocity mode
	Evalboards.ch1.writeRegister(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000008);
}

void Motor_Velocity_OpenLoop(uint32_t value){
	Evalboards.ch1.writeRegister(0, TMC4671_OPENLOOP_VELOCITY_TARGET, value);
}
void Motor_Config_OpenLoop_Delete(void){
	// Open loop settings
	Evalboards.ch1.writeRegister(0, TMC4671_OPENLOOP_MODE, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_OPENLOOP_ACCELERATION, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_OPENLOOP_VELOCITY_TARGET, 0x00000000);

	// Feedback selection
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_SELECTION, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_UQ_UD_EXT, 0x00000000);

	// Delete open loop velocity mode
	Evalboards.ch1.writeRegister(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000000);

}

void Motor_Config_Torque(void){

	// Motor type &  PWM configuration
	Evalboards.ch1.writeRegister(0, TMC4671_MOTOR_TYPE_N_POLE_PAIRS, 0x00030004);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_POLARITIES, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_MAXCNT, 0x00000F9F);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_BBM_H_BBM_L, 0x00000505);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_SV_CHOP, 0x00000007);

	// ADC configuration
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I_SELECT, 0x18000100);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCFG_B_MCFG_A, 0x00100010);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCLK_A, 0x20000000);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCLK_B, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MDEC_B_MDEC_A, 0x014E014E);
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I0_SCALE_OFFSET, 0x0100825D);
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I1_SCALE_OFFSET, 0x010082CE);

	// Digital hall settings
	Evalboards.ch1.writeRegister(0, TMC4671_HALL_MODE, 0x00000001);
	Evalboards.ch1.writeRegister(0, TMC4671_HALL_PHI_E_PHI_M_OFFSET, 0x00000000);

	// Feedback selection
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_SELECTION, 0x00000005);
	Evalboards.ch1.writeRegister(0, TMC4671_VELOCITY_SELECTION, 0x0000000C);

	// Limits
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 0x000003E8);

	// PI settings
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_P_TORQUE_I, 0x01000100);
	Evalboards.ch1.writeRegister(0, TMC4671_PID_FLUX_P_FLUX_I, 0x01000100);

	// Switch to torque mode
	Evalboards.ch1.writeRegister(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000001);
}

void Motor_Velocity_Torque(uint32_t value){
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_FLUX_TARGET, value);

}
void Motor_Config_Torque_Delete(){
	// Digital hall settings
	Evalboards.ch1.writeRegister(0, TMC4671_HALL_MODE, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_HALL_PHI_E_PHI_M_OFFSET, 0x00000000);

	// Feedback selection
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_SELECTION, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_VELOCITY_SELECTION, 0x00000000);

	// Limits
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 0x00000000);

	// PI settings
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_P_TORQUE_I, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_PID_FLUX_P_FLUX_I, 0x00000000);

	// Delete torque mode
	Evalboards.ch1.writeRegister(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000000);


}


void Motor_Config_Torque_ABN_Encoder(void){
	// Motor type &  PWM configuration
	Evalboards.ch1.writeRegister(0, TMC4671_MOTOR_TYPE_N_POLE_PAIRS, 0x00030004);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_POLARITIES, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_MAXCNT, 0x00000F9F);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_BBM_H_BBM_L, 0x00000505);
	Evalboards.ch1.writeRegister(0, TMC4671_PWM_SV_CHOP, 0x00000007);

	// ADC configuration
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I_SELECT, 0x18000100);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCFG_B_MCFG_A, 0x00100010);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCLK_A, 0x20000000);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MCLK_B, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_dsADC_MDEC_B_MDEC_A, 0x014E014E);
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I0_SCALE_OFFSET, 0x0100825D);
	Evalboards.ch1.writeRegister(0, TMC4671_ADC_I1_SCALE_OFFSET, 0x010082CE);

	// ABN encoder settings
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_MODE, 0x00001000);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_PPR, 0x00004000);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_COUNT, 0x000017F0);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);

	// Limits
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 0x000003E8);

	// PI settings
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_P_TORQUE_I, 0x01000100);
	Evalboards.ch1.writeRegister(0, TMC4671_PID_FLUX_P_FLUX_I, 0x01000100);


	// Init encoder (mode 0)
	Evalboards.ch1.writeRegister(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000008);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_SELECTION, 0x00000001);
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_EXT, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_UQ_UD_EXT, 0x000007D0);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_COUNT, 0x00000000);

	// Feedback selection
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_SELECTION, 0x00000003);
	Evalboards.ch1.writeRegister(0, TMC4671_VELOCITY_SELECTION, 0x00000009);

	// Switch to torque mode
	Evalboards.ch1.writeRegister(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000001);


}
void Motor_Velocity_Torque_ABN_Encoder(uint32_t value){
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_FLUX_TARGET, value);
}
void Motor_Config_Torque_ABN_Encoder_Delete(void){
	// ABN encoder settings
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_MODE, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_PPR, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_COUNT, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);

	// Limits
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 0x00000000);

	// PI settings
	Evalboards.ch1.writeRegister(0, TMC4671_PID_TORQUE_P_TORQUE_I, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_PID_FLUX_P_FLUX_I, 0x00000000);


	// Init encoder (mode 0)
	Evalboards.ch1.writeRegister(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_PHI_E_PHI_M_OFFSET, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_SELECTION, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_EXT, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_UQ_UD_EXT, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_ABN_DECODER_COUNT, 0x00000000);

	// Feedback selection
	Evalboards.ch1.writeRegister(0, TMC4671_PHI_E_SELECTION, 0x00000000);
	Evalboards.ch1.writeRegister(0, TMC4671_VELOCITY_SELECTION, 0x00000000);

	// Switch to torque mode
	Evalboards.ch1.writeRegister(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000000);

}


