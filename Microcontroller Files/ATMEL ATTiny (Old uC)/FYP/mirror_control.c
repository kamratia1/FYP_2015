/*
 *  mirror_control.c
 *  Created: 23/05/2015 17:29:38
 *  Author: Kishan
 */ 

#include "mirror_control.h"
#include "peripherals.h"
#include "uart.h"

volatile uint16_t count1 = 0;
volatile uint16_t count2 = 0;
volatile uint16_t val1 = 0;
volatile uint16_t val2 = 0;
volatile uint16_t val3 = 0;
volatile uint16_t result = 0;
uint16_t d1 = 0;
uint16_t d2 = 0;

char str[50];

void mirror_startup(void)
{
	// set up timer2 for 100Hz 25% duty cycle
	TCCR2B |= (1<<CS20);	// Set clock pre-scaler to 1
	
}

void Timer2_init(void)
{
	OCR2A = 1600;			// Sets up a 5KHz Sampling Time
	TCNT2 = 0;
	TCCR2A |= (1<<COM2A1);	// Clear Timer on compare match
	TCCR2B |= (1<<WGM22);	
	TCCR2B |= (1<<CS20);	// Set clock pre-scaler to 1
	TIMSK2 |= (1<<OCIE2A);	// Interrupt Enable on Output compare A match
}

void Timer1_init(void)
{
	TCCR1B |= (1<<ICNC2);		// Input Capture Noise Canceler
	TCCR1B |= (1<<CS20);		// Timer 1 clock source --> No prescaling
	TCCR1B |= (1<<PCAP_ICP);	// capture rising edge of pulse
	
	PCAP_DIRECTION_PORT &= ~(1<<PCAP_PIN);		// Set PCAP pin to Input
}

uint16_t get_capacitance(void)
{
	TCNT1 = 0;						// clear counter
	TCCR1B |= (1<<PCAP_ICP);		// capture rising edge of pulse
	while (!(TIFR1 & (1<<ICF1)));   // wait for capture
	count1 = TCNT1;
	
	TIFR1 |= (1<<ICF1);				// clear the capture flag
	TCCR1B &= ~(1<<PCAP_ICP);		// capture falling edge of pulse
	while (!(TIFR1 & (1<<ICF1)));	// wait for capture
	count2 = TCNT1;
	result = count2 - count1;
	TIFR1 |= (1<<ICF1);				// clear the capture flag
				
	return result;
}

ISR(TIMER2_COMPA_vect) 
{
	val3 = val2;
	val2 = val1;
	val1 = get_capacitance();
	
	// compute derivatives
	d1 = val1 - val2;
	d2 = val2 - val3;
	
	if(d2>d1) // falling derivative
	{ 
		if(d2>0 && d1<0)
		{
			mirror_drive_off();
		}
	}
	else // rising derivative
	{
		if(d2<0 && d1>0)
		{
			mirror_drive_on();
		}
	}
	TCNT2 = 0;
}
