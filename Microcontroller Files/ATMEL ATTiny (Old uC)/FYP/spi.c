/*
 * File Name: spi.c
 * ATTiny 841 SPI driver
 * Created: 09/05/2015 11:34:36
 * Author: Kishan Amratia
 */ 

#include "spi.h"

void SPI_MasterInit()
{
	/*Set MOSI, SCK and SSN output, all others input*/
	PRR &= ~(1<<PRSPI);
	DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SSN);
	
	SPSR |= (1<<SPI2X); // Set double speed SPI
	// Set clock speed at fclk_IO/2 by leaving SPR1 and SPR0 as 0 in SPCR
	
	SPCR |= (1<< SPE)|(1<<MSTR)|(1<<CPHA); // Enable SPI, Master
	
	/* 
	*  As per settings for PICOCAP device
	*  Set in SPI Mode 1. CPOL0, CPHA1
	*  DORD0: Bit sequence Order MSB First
	*/
}

void SPI_Transmit(uint8_t cData)
{
	SPDR = cData; // Begin transmission	
	while(!(SPSR & (1<<SPIF))); // wait for transmission complete
}

/*void SPI_TransmitBlock( uint8_t *str )
{
	// transmit more than one byte of information to slave
	while (*str) {
		SPI_Transmit(*str);
		str++;
	}
}*/

uint8_t * SPI_Recieve(void)
{
	uint8_t head = 0;
	
	// PICOCAP has a 3 byte result so every byte is being stored in a 3 length array
	
	while(head < SPI_RX_RESULT_SIZE)
	{
		while(!(SPSR & (1<<SPIF)));	// wait for reception complete
		Result[head] = SPDR;			// put the bytes received in an array
		head++;
	}
	
	return &Result; // Returns pointer to the array
}