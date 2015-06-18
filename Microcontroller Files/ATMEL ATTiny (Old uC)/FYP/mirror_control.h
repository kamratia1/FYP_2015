#include "setup.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void Timer1_init(void);
uint16_t get_capacitance(void);
void Timer2_init(void);


void mirror_startup(void);