/*
 * File Name: pcap02a.c
 * PICOCAP 02a Device Driver
 * Created: 09/05/2015 11:34:36
 * Author: Kishan Amratia
 * More details about PICOCAP can be found at http://www.acam.de/products/picocap/ 
 */ 

#include "pcap02a.h"

// start a Capacitance Conversion and read back the result
uint8_t * PCAP_Conv(void)
{	// returns a pointer to an array that holds the result of the conversion 

	SPI_Transmit(0x8C);		// CDC Start Conversion
	_delay_us(CONV_TIME);	// wait some time before reading back result 
	
	// read result
	SPI_Transmit(0x40);
	SPI_Transmit(0x06);		// Read result in register 6,7,8 i.e. Result 2
	SPI_Transmit(0x00);
	SPI_Transmit(0x00);
	
	return SPI_Recieve();	// Returns an 8-bit integer array that is 3 bytes long
}

// Writes the configuration registers
void PICOCAP_Config_Set(void)
{
	uint8_t i = 0;
	
	Clear_RunBit();		// Disable Converter		
	SPI_Transmit(0xC0);
	SPI_Transmit(0x00);

	while(i<80)
	{
		SPI_Transmit(Config[i]);
		i++;
	}
	
	Set_RunBit();		// Enable Converter
} 

void Set_RunBit(void)
{
	// This function Enables the converter
	SPI_Transmit(0xC0);
	SPI_Transmit(0x4D);
	SPI_Transmit(0x01);
}

void Clear_RunBit(void)
{
	// This function Disables the converter
	SPI_Transmit(0xC0);
	SPI_Transmit(0x4D);
	SPI_Transmit(0x00);
}