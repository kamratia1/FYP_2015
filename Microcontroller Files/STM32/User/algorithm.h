#ifndef __algorithm_h
#define __algorithm_h

#include "config.h"
#include "io.h"
#include "adc.h"

#define THRESHOLD		0
#define THRESHOLD2	0

extern volatile int drive_flg;

void closed_loop(void);
void startup_seq(uint8_t, uint8_t, uint8_t);
void delay_ms (volatile int);
void delay_us (volatile int);
void dead_band(uint16_t);

#endif
