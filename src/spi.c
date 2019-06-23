#include "comm/spi.h"
#include "board.h"


typedef struct {
	gpioMap_t SPI1_CSN;
} SPI_Pins;

static SPI_Pins pins;


//Configuración Periferico SPI  en Modo 3 , clock  1MHZ advertencia del fabricante, otra velocidad puede corromper el MSB de las tramas de datos en chip TMC4671

void spi_init()
{
	pins.SPI1_CSN = ENET_TXD0;
	gpioConfig(pins.SPI1_CSN, GPIO_OUTPUT);
	spiConfig(SPI0);

	/* SPI configuration */
	Board_SSP_Init(LPC_SSP1);
	Chip_SSP_Init(LPC_SSP1);
	Chip_SSP_SetFormat(LPC_SSP1, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, SSP_CLOCK_CPHA1_CPOL1);
#if defined(TMC5130_eval)
	Chip_SSP_SetBitRate(LPC_SSP1, 2666666);
#elif defined(TMC4671_eval)
	Chip_SSP_SetBitRate(LPC_SSP1, 1000000);
#endif
	Chip_SSP_Enable(LPC_SSP1);
}


uint8_t spi_readWrite(spiMap_t channel, uint8_t writeData, bool lastTransfer)
{
	gpioWrite(pins.SPI1_CSN, OFF);
	uint8_t ret = 0;

	Chip_SSP_DATA_SETUP_T config = {
		.tx_data = &writeData,
		.tx_cnt  = 0,
		.rx_data = &ret,
		.rx_cnt  = 0,
		.length  = 1
	};

	Chip_SSP_RWFrames_Blocking(LPC_SSP1, &config);

	if(lastTransfer)
		gpioWrite(pins.SPI1_CSN, ON);

	return ret;
}

void spi_readWriteArray(spiMap_t channel, uint8_t *data, size_t length)
{
	gpioWrite(pins.SPI1_CSN, OFF);

	Chip_SSP_DATA_SETUP_T config = {
		.tx_data = data,
		.tx_cnt  = 0,
		.rx_data = data,
		.rx_cnt  = 0,
		.length  = length
	};

	Chip_SSP_RWFrames_Blocking(LPC_SSP1, &config);

	gpioWrite(pins.SPI1_CSN, ON);
}
