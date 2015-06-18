#ifndef __io_h
#define __io_h

#include "config.h"

void IO_init(void);							// Function that initializes all GPIO pins
void mirror_drive_en(uint8_t);	// Apply(1)/Disconnect(0) High Voltage at mirror
void HV_en(uint8_t);						// Enable(1) or Disable(0) the High Voltage Converter
void SYNC_en(uint8_t);					// Pull the SYNC pin High(1) or Low(0) 

#endif
