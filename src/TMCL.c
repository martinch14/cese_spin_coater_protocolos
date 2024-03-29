#include "tmc/TMCL.h"
#include "boards/Board.h"

// these addresses are fixed
#define SERIAL_MODULE_ADDRESS  1
#define SERIAL_HOST_ADDRESS    2

// todo CHECK 2: these are unused - delete? (LH) #11
// tmcl interpreter states
#define TM_IDLE      0
#define TM_RUN       1
#define TM_STEP      2
#define TM_RESET     3 // unused
#define TM_DOWNLOAD  4
#define TM_DEBUG     5 // wie TM_IDLE, es wird jedoch der Akku nicht modifiziert bei GAP etc.

// todo CHECK 2: these are unused - delete? (LH) #12
#define TCS_IDLE           0
#define TCS_CAN7           1
#define TCS_CAN8           2
#define TCS_UART           3
#define TCS_UART_ERROR     4
#define TCS_UART_II        5
#define TCS_UART_II_ERROR  6
#define TCS_USB            7
#define TCS_USB_ERROR      8
#define TCS_MEM            9

// TMCL commands
#define TMCL_ROR                     1
#define TMCL_ROL                     2
#define TMCL_MST                     3
#define TMCL_MVP                     4
#define TMCL_SAP                     5
#define TMCL_GAP                     6
#define TMCL_STAP                    7
#define TMCL_RSAP                    8
#define TMCL_SGP                     9
#define TMCL_GGP                     10
#define TMCL_STGP                    11
#define TMCL_RSGP                    12
#define TMCL_RFS                     13
#define TMCL_SIO                     14
#define TMCL_GIO                     15
#define TMCL_CALC                    19
#define TMCL_COMP                    20
#define TMCL_JC                      21
#define TMCL_JA                      22
#define TMCL_CSUB                    23
#define TMCL_RSUB                    24
#define TMCL_EI                      25
#define TMCL_DI                      26
#define TMCL_WAIT                    27
#define TMCL_STOP                    28
#define TMCL_SAC                     29
#define TMCL_SCO                     30
#define TMCL_GCO                     31
#define TMCL_CCO                     32
#define TMCL_CALCX                   33
#define TMCL_AAP                     34
#define TMCL_AGP                     35
#define TMCL_CLE                     36
#define TMCL_VECT                    37
#define TMCL_RETI                    38
#define TMCL_ACO                     39

#define TMCL_UF0                     64
#define TMCL_UF1                     65
#define TMCL_UF2                     66
#define TMCL_UF3                     67
#define TMCL_UF4                     68
#define TMCL_UF5                     69
#define TMCL_UF6                     70
#define TMCL_UF7                     71

#define TMCL_ApplStop                128
#define TMCL_ApplRun                 129
#define TMCL_ApplStep                130
#define TMCL_ApplReset               131
#define TMCL_DownloadStart           132
#define TMCL_DownloadEnd             133
#define TMCL_ReadMem                 134
#define TMCL_GetStatus               135
#define TMCL_GetVersion              136
#define TMCL_FactoryDefault          137
#define TMCL_SetEvent                138
#define TMCL_SetASCII                139
#define TMCL_SecurityCode            140
#define TMCL_Breakpoint              141
#define TMCL_RamDebug                142
#define TMCL_GetIds                  143
#define TMCL_UF_CH1                  144
#define TMCL_UF_CH2                  145
#define TMCL_writeRegisterChannel_1  146
#define TMCL_writeRegisterChannel_2  147
#define TMCL_readRegisterChannel_1   148
#define TMCL_readRegisterChannel_2   149

#define TMCL_BoardMeasuredSpeed      150
#define TMCL_BoardError              151
#define TMCL_BoardReset              152

#define TMCL_WLAN                    160
#define TMCL_WLAN_CMD                160
#define TMCL_WLAN_IS_RTS             161
#define TMCL_WLAN_CMDMODE_EN         162
#define TMCL_WLAN_IS_CMDMODE         163

#define TMCL_MIN                     170
#define TMCL_MAX                     171

#define TMCL_Boot                    242
#define TMCL_SoftwareReset           255

// Command type variants
#define MVP_ABS  0
#define MVP_REL  1
#define MVP_PRF  2

// GetVersion() Format types
#define VERSION_FORMAT_ASCII      0
#define VERSION_FORMAT_BINARY     1
#define VERSION_BOOTLOADER        2 // todo CHECK 2: implemented this way in IDE - probably means getting the bootloader version. Not implemented in firmware (LH)
#define VERSION_SIGNATURE         3 // todo CHECK 2: implemented under "Signature" in IDE. Not sure what to return for that. Not implemented in firmware (LH)
#define VERSION_BOARD_DETECT_SRC  4 // todo CHECK 2: This doesn't really fit under GetVersion, but its implemented there in the IDE - change or leave this way? (LH)

//Statuscodes
#define REPLY_OK                     100
#define REPLY_CMD_LOADED             101
#define REPLY_CHKERR                 1
#define REPLY_INVALID_CMD            2
#define REPLY_INVALID_TYPE           3
#define REPLY_INVALID_VALUE          4
#define REPLY_EEPROM_LOCKED          5
#define REPLY_CMD_NOT_AVAILABLE      6
#define REPLY_CMD_LOAD_ERROR         7
#define REPLY_WRITE_PROTECTED        8
#define REPLY_MAX_EXCEEDED           9
#define REPLY_DOWNLOAD_NOT_POSSIBLE  10
#define REPLY_CHIP_READ_FAILED       11
#define REPLY_DELAYED                128
#define REPLY_ACTIVE_COMM            129

// TMCL communication status
#define TMCL_RX_ERROR_NONE      0
#define TMCL_RX_ERROR_NODATA    1
#define TMCL_RX_ERROR_CHECKSUM  2

// id detection state definitions
#define ID_STATE_WAIT_LOW   0  // id detection waiting for first edge (currently low)
#define ID_STATE_WAIT_HIGH  1  // id detection waiting for second edge (currently high)
#define ID_STATE_DONE       2  // id detection finished successfully
#define ID_STATE_INVALID    3  // id detection failed - we got an answer, but no corresponding ID (invalid ID pulse length)
#define ID_STATE_NO_ANSWER  4  // id detection failed - board doesn't answer
#define ID_STATE_TIMEOUT    5  // id detection failed - board id pulse went high but not low
#define ID_STATE_NOT_IN_FW  6  // id detection detected a valid id that is not supported in this firmware

#define ID_TMC5130 5
#define ID_TMC4671 13
#if defined(TMC5130_eval)
#define ID_EVALBOARD ID_TMC5130
#elif defined(TMC4671_eval)
#define ID_EVALBOARD ID_TMC4671
#endif

const char *VersionString = MODULE_ID"V306";

// TMCL request
typedef struct
{
	uint8_t   Opcode;
	uint8_t   Type;
	uint8_t   Motor;
	uint32_t  Error;
	union
	{
		uint8_t Byte[4];
		uint32_t UInt32;
		int32_t Int32;
		f32 Float32;
	} Value;
} TMCLCommandTypeDef;

// TMCL reply
typedef struct
{
	uint8_t Status;
	uint8_t Opcode;
	union
	{
		uint8_t Byte[4];
		uint32_t UInt32;
		int32_t Int32;
		f32 Float32;
	} Value;

	uint8_t Special[9];
	uint8_t IsSpecial;  // next transfer will not use the serial address and the checksum bytes - instead the whole datagram is filled with data (used to transmit ASCII version string)
} TMCLReplyTypeDef;

void ExecuteActualCommand();
uint8_t setTMCLStatus(uint8_t evalError);

// Helper functions - used to prevent ExecuteActualCommand() from getting too big.
// No parameters or return value are used.
static void SetGlobalParameter(void);
static void GetGlobalParameter(void);
static void boardsErrors(void);
static void boardsReset(void);
static void boardsMeasuredSpeed(void);
static void setDriversEnable(void);
static void SoftwareReset(void);
static void GetVersion(void);
static void GetInput(void);
static void rx(void);
static void tx(void);
static void boardAssignment(void);

TMCLCommandTypeDef ActualCommand;
TMCLReplyTypeDef ActualReply;
uint32_t resetRequest = 0;

#if defined(Landungsbruecke)
extern uint32_t BLMagic;
#endif

// Sets TMCL status from Evalboard error. Returns the parameter given to allow for compact error handling
uint8_t setTMCLStatus(uint8_t evalError)
{
	if(evalError == TMC_ERROR_NONE)          ActualReply.Status = REPLY_OK;
	else if(evalError & TMC_ERROR_FUNCTION)  ActualReply.Status = REPLY_INVALID_CMD;
	else if(evalError & TMC_ERROR_TYPE)      ActualReply.Status = REPLY_INVALID_TYPE;
	else if(evalError & TMC_ERROR_MOTOR)     ActualReply.Status = REPLY_INVALID_TYPE; // todo CHECK ADD 2: Different errors for Evalboard type/motor errors? (LH) #1
	else if(evalError & TMC_ERROR_VALUE)     ActualReply.Status = REPLY_INVALID_VALUE;
	else if(evalError & TMC_ERROR_NOT_DONE)  ActualReply.Status = REPLY_DELAYED;
	else if(evalError & TMC_ERROR_CHIP)      ActualReply.Status = REPLY_EEPROM_LOCKED;
	return evalError;
}

void ExecuteActualCommand()
{
	ActualReply.Opcode = ActualCommand.Opcode;
	ActualReply.Status = REPLY_OK;
	ActualReply.Value.Int32 = ActualCommand.Value.Int32;

	if(ActualCommand.Error == TMCL_RX_ERROR_CHECKSUM)
	{
		ActualReply.Value.Int32  = 0;
		ActualReply.Status       = REPLY_CHKERR;
		return;
	}

	switch(ActualCommand.Opcode)
	{
	case TMCL_ROR:
		setTMCLStatus(Evalboards.ch1.right(ActualCommand.Motor, ActualCommand.Value.Int32));
		break;
	case TMCL_ROL:
		setTMCLStatus(Evalboards.ch1.left(ActualCommand.Motor, ActualCommand.Value.Int32));
		break;
	case TMCL_MST:
		setTMCLStatus(Evalboards.ch1.stop(ActualCommand.Motor));
		break;
	case TMCL_MVP:
		switch(ActualCommand.Type)
		{
		case MVP_ABS: // move absolute
			setTMCLStatus(Evalboards.ch1.moveTo(ActualCommand.Motor, ActualCommand.Value.Int32));
			break;
		case MVP_REL: // move relative
			setTMCLStatus(Evalboards.ch1.moveBy(ActualCommand.Motor, &ActualCommand.Value.Int32));
			ActualReply.Value.Int32 = ActualCommand.Value.Int32;
			break;
		case MVP_PRF:
			setTMCLStatus(Evalboards.ch1.moveProfile(ActualCommand.Motor, ActualCommand.Value.Int32));
			break;
		default:
			ActualReply.Status = REPLY_INVALID_TYPE;
			break;
		}
		break;
	case TMCL_SAP:
		setTMCLStatus(Evalboards.ch1.SAP(ActualCommand.Type, ActualCommand.Motor, ActualCommand.Value.Int32));
		break;
	case TMCL_GAP:
		setTMCLStatus(Evalboards.ch1.GAP(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32));
		break;
	case TMCL_SGP:
		SetGlobalParameter();
		break;
	case TMCL_GGP:
		GetGlobalParameter();
		break;
	case TMCL_GIO:
		GetInput();
		break;
	case TMCL_UF0:
		setDriversEnable();
		break;
	case TMCL_UF4:
		setTMCLStatus(Evalboards.ch1.getMeasuredSpeed(ActualCommand.Motor, &ActualReply.Value.Int32));
		break;
	case TMCL_UF5:
		// if function doesn't exist for ch1 try ch2 // todo CHECK REM 2: We have TMCL_writeRegisterChannel_1, we dont need this. Make sure it isnt used in IDE (LH) #1
		Evalboards.ch1.writeRegister(ActualCommand.Motor, ActualCommand.Type, ActualCommand.Value.Int32);
		break;
	case TMCL_UF6:
		// if function doesn't exist for ch1 try ch2 // todo CHECK REM 2: We have TMCL_readRegisterChannel_1, we dont need this. Make sure it isnt used in IDE (LH) #2
		Evalboards.ch1.readRegister(ActualCommand.Motor, ActualCommand.Type, &ActualReply.Value.Int32);
		break;
	case TMCL_GetVersion:
		GetVersion();
		break;
	case TMCL_GetIds:
		boardAssignment();
		break;
	case TMCL_UF_CH1:
		// user function for motionController board
		setTMCLStatus(Evalboards.ch1.userFunction(ActualCommand.Type, ActualCommand.Motor, &ActualCommand.Value.Int32));
		ActualReply.Value.Int32 = ActualCommand.Value.Int32;
		break;
	case TMCL_writeRegisterChannel_1:
		Evalboards.ch1.writeRegister(ActualCommand.Motor, ActualCommand.Type, ActualCommand.Value.Int32);
		break;
	case TMCL_readRegisterChannel_1:
		Evalboards.ch1.readRegister(ActualCommand.Motor, ActualCommand.Type, &ActualReply.Value.Int32);
		break;
	case TMCL_BoardMeasuredSpeed:
		// measured speed from motionController board or driver board depending on type
		boardsMeasuredSpeed();
		break;
	case TMCL_BoardError:
		// errors of motionController board or driver board depending on type
		boardsErrors();
		break;
	case TMCL_BoardReset:
		// reset of motionController board or driver board depending on type
		boardsReset();
		break;
	case TMCL_MIN:
		setTMCLStatus(Evalboards.ch1.getMin(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32));
		break;
	case TMCL_MAX:
		setTMCLStatus(Evalboards.ch1.getMax(ActualCommand.Type, ActualCommand.Motor, &ActualReply.Value.Int32));
		break;
	case TMCL_Boot:
		if(ActualCommand.Type           != 0x81)  break;
		if(ActualCommand.Motor          != 0x92)  break;
		if(ActualCommand.Value.Byte[3]  != 0xA3)  break;
		if(ActualCommand.Value.Byte[2]  != 0xB4)  break;
		if(ActualCommand.Value.Byte[1]  != 0xC5)  break;
		if(ActualCommand.Value.Byte[0]  != 0xD6)  break;
//		tmcl_boot();
		break;
	case TMCL_SoftwareReset:
		SoftwareReset();
		break;
	default:
		ActualReply.Status = REPLY_INVALID_CMD;
		break;
	}
}

void tmcl_init()
{
	ActualCommand.Error  = TMCL_RX_ERROR_NODATA;
}

void tmcl_process()
{
	if(ActualCommand.Error != TMCL_RX_ERROR_NODATA)
		tx();

	if(resetRequest)
		reset();

	ActualReply.IsSpecial = 0;

	rx();
	if(ActualCommand.Error != TMCL_RX_ERROR_NODATA)
	{
		ExecuteActualCommand();
		return;
	}
}

static void tx(void)
{
	uint8_t checkSum = 0;

	uint8_t reply[9];

	if(ActualReply.IsSpecial)
	{
		for(int i = 0; i < 9; i++)
			reply[i] = ActualReply.Special[i];
	}
	else
	{
		checkSum += SERIAL_HOST_ADDRESS;
		checkSum += SERIAL_MODULE_ADDRESS;
		checkSum += ActualReply.Status;
		checkSum += ActualReply.Opcode;
		checkSum += ActualReply.Value.Byte[3];
		checkSum += ActualReply.Value.Byte[2];
		checkSum += ActualReply.Value.Byte[1];
		checkSum += ActualReply.Value.Byte[0];

		reply[0] = SERIAL_HOST_ADDRESS;
		reply[1] = SERIAL_MODULE_ADDRESS;
		reply[2] = ActualReply.Status;
		reply[3] = ActualReply.Opcode;
		reply[4] = ActualReply.Value.Byte[3];
		reply[5] = ActualReply.Value.Byte[2];
		reply[6] = ActualReply.Value.Byte[1];
		reply[7] = ActualReply.Value.Byte[0];
		reply[8] = checkSum;
	}

	vcom_write(reply, 9);
}

static void rx(void)
{
	uint8_t checkSum = 0;
	uint8_t cmd[9];

	if(!vcom_bread(cmd, 9))
	{
		ActualCommand.Error = TMCL_RX_ERROR_NODATA;
		return;
	}

	// todo ADD CHECK 2: check for SERIAL_MODULE_ADDRESS byte ( cmd[0] ) ? (LH)

	for(int i = 0; i < 8; i++)
		checkSum += cmd[i];

	if(checkSum != cmd[8])
	{
		ActualCommand.Error	= TMCL_RX_ERROR_CHECKSUM;
		return;
	}

	ActualCommand.Opcode         = cmd[1];
	ActualCommand.Type           = cmd[2];
	ActualCommand.Motor          = cmd[3];
	ActualCommand.Value.Byte[3]  = cmd[4];
	ActualCommand.Value.Byte[2]  = cmd[5];
	ActualCommand.Value.Byte[1]  = cmd[6];
	ActualCommand.Value.Byte[0]  = cmd[7];
	ActualCommand.Error          = TMCL_RX_ERROR_NONE;
}

static void SetGlobalParameter()
{
	switch(ActualCommand.Type)
	{
	case 1:
		break;
	case 2:
		setDriversEnable();
		break;
	case 4:
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void GetGlobalParameter()
{
	switch(ActualCommand.Type)
	{
		case 1:
			ActualReply.Value.Int32 = 73;
			break;
		case 2:
			ActualReply.Value.Int32 = (Evalboards.driverEnable == DRIVER_ENABLE)? 1:0;
			break;
		case 3:
			ActualReply.Value.Int32 = 0;
			break;
		case 4:
			ActualReply.Value.Int32 = (ID_STATE_NOT_IN_FW << 24) | (0 << 16) | (ID_STATE_DONE << 8) | ID_EVALBOARD;
			break;
		case 5: // Get hardware ID
			ActualReply.Value.Int32 = 0xFF;
			break;
		default:
			ActualReply.Status = REPLY_INVALID_TYPE;
			break;
	}
}

static void boardAssignment(void)
{
	switch(ActualCommand.Type) {
	case 3:
		ActualReply.Value.Int32 = (ID_STATE_WAIT_LOW << 24) | (0 << 16) | (ID_STATE_WAIT_LOW << 8) | ID_EVALBOARD;
		break;
	case 4:
		ActualReply.Value.Int32 = (ID_STATE_NOT_IN_FW << 24) | (0 << 16) | (ID_STATE_DONE << 8) | ID_EVALBOARD;
		break;
	default:
		ActualReply.Value.Int32 = (ID_STATE_NOT_IN_FW << 24) | (0 << 16) | (ID_STATE_DONE << 8) | ID_EVALBOARD;
		break;
	}
}

static void boardsErrors(void)
{
	switch(ActualCommand.Type)
	{
	case 0:
		ActualReply.Value.Int32 = Evalboards.ch1.errors;
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void boardsReset(void)
{
	switch(ActualCommand.Type)
	{
	case 0:
		if(!Evalboards.ch1.config->reset())
			ActualReply.Status = REPLY_WRITE_PROTECTED;
		break;
	case 2:
		if(!Evalboards.ch1.config->reset())
			ActualReply.Status = REPLY_WRITE_PROTECTED;
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void boardsMeasuredSpeed(void)
{
	switch(ActualCommand.Type)
	{
	case 0:
		ActualReply.Status = Evalboards.ch1.getMeasuredSpeed(ActualCommand.Motor, &ActualReply.Value.Int32);
		break;
	default:
		ActualReply.Status = REPLY_INVALID_TYPE;
		break;
	}
}

static void setDriversEnable()
{
	Evalboards.driverEnable = (ActualCommand.Value.Int32 == 0) ? DRIVER_DISABLE : DRIVER_ENABLE;
	Evalboards.ch1.enableDriver(DRIVER_USE_GLOBAL_ENABLE);
}

static void SoftwareReset(void)
{
	if(ActualCommand.Value.Int32 == 1234)
		resetRequest = TRUE;
}

static void GetVersion(void)
{
	if(ActualCommand.Type == VERSION_FORMAT_ASCII)
	{
		ActualReply.IsSpecial   = 1;
		ActualReply.Special[0]  = SERIAL_HOST_ADDRESS;

		for(int i = 0; i < 8; i++)
			ActualReply.Special[i+1] = VersionString[i];
	}
	else if(ActualCommand.Type == VERSION_FORMAT_BINARY)
	{
		uint8_t tmpVal;

		// module version high
		tmpVal = (uint8_t) VersionString[0] - '0';	// Ascii digit - '0' = digit value
		tmpVal *= 10;
		tmpVal += (uint8_t) VersionString[1] - '0';
		ActualReply.Value.Byte[3] = tmpVal;

		// module version low
		tmpVal = (uint8_t) VersionString[2] - '0';
		tmpVal *= 10;
		tmpVal += (uint8_t) VersionString[3] - '0';
		ActualReply.Value.Byte[2] = tmpVal;

		// fw version high
		ActualReply.Value.Byte[1] = (uint8_t) VersionString[5] - '0';

		// fw version low
		tmpVal = (uint8_t) VersionString[6] - '0';
		tmpVal *= 10;
		tmpVal += (uint8_t) VersionString[7] - '0';
		ActualReply.Value.Byte[0] = tmpVal;
	}
	//how were the boards detected?	// todo CHECK 2: Doesn't fit into GetVersion. Move somewhere else? Or maybe change GetVersion to GetBoardInfo or something (LH)
	else if(ActualCommand.Type == VERSION_BOARD_DETECT_SRC)
	{
		// unused in CIAA
	}
}

static void GetInput(void)
{
//	switch(ActualCommand.Type)
//	{
//	case 0:
//		ActualReply.Value.Int32 = *HAL.ADCs->AIN0;
//		break;
//	case 1:
//		ActualReply.Value.Int32 = *HAL.ADCs->AIN1;
//		break;
//	case 2:
//		ActualReply.Value.Int32 = *HAL.ADCs->AIN2;
//		break;
//	case 3:
//		ActualReply.Value.Int32 = *HAL.ADCs->DIO4;
//		break;
//	case 4:
//		ActualReply.Value.Int32 = *HAL.ADCs->DIO5;
//		break;
//	case 5:
//		ActualReply.Value.Int32 = VitalSignsMonitor.VM;
//		break;
//	case 6:	// Raw VM ADC value, no scaling calculation done // todo QOL 2: Switch this case with case 5? That way we have the raw Values from 0-5, then 6 for scaled VM value. Requires IDE changes (LH)
//		ActualReply.Value.Int32 = *HAL.ADCs->VM;
//		break;
//	default:
//		ActualReply.Status = REPLY_INVALID_TYPE;
//		break;
//	}
}
