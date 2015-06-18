#include "setup.h"
#include <avr/io.h>

#define DDR_SPI	DDRB
#define DD_MOSI PA6
#define DD_SCK PA4
#define DD_SSN PA7
#define SPI_RX_RESULT_SIZE  3

void SPI_MasterInit(void);
void SPI_Transmit(uint8_t);
//void SPI_TransmitBlock(uint8_t *);
uint8_t * SPI_Recieve(void);

uint8_t Result[SPI_RX_RESULT_SIZE];
