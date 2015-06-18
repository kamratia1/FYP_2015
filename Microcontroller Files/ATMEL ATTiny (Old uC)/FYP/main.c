/*
 *  main.c
 *  Created: 24/03/2015 17:20:12
 *  Author: Kishan Amratia
 *  Imperial College London
 *  Dept. of Electrical and Electronic Engineering
 *  Final Year Project: Control of a MEMS scanning mirror for display applications
 */ 

#include "setup.h"
#include "uart.h"
#include "peripherals.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void Delay(void);
void clock_8MHz(void);

int main(void)
{
	char str[50];
	uint16_t adcval = 0;
	
	#ifdef CLOCK8MHZ
	// Setup clock pre-scaler to 1 so that system clock is set to 8MHz
	clock_8MHz();		
	#endif
	
	pwm_init();			// initialize timer in PWM mode
	uart_init();		// initialize serial communication
	adc_init();			// initialize ADCs
	sei();				// initialize global interrupts
	io_init();			// initialize I/O pins
	
	//uart0_puts("Kishan Amratia - Final Year Project 2015\n\r");
	
	HV_set(50);			// Set HV converter Voltage
	HV_en(1);			// Switch on the HV converter
	//PWM_COMPARE_REGISTER = 45;
	
	

	//_delay_ms(200);		// wait for HV_converter to startup 
	//mirror_startup();	
	
	//_delay_us(500);		// startup sequence for 500us
	
	//Timer1_init();		// Initialize Timer for calculating duty cycle of capacitance PWM 
	//Timer2_init();		// Timer to set up sampling rate
	
	while(1) // run forever
	{
		//adcval = get_capacitance();
		//sprintf(str, "%d\r\n", result);
		//uart0_puts(str);
		//_delay_ms(300);
		//HV_en(1);
		
		//adcval = adc_read_avg(7,3);	// Read ADC Channel 6 and average it 3 times
		
		/*// For PC debugging
		process_input();
		//mirror_control();
		adcval = adc_read_avg(10,5);
		//send data to PC --> ADC Values (Feedback, HV Estimation)
		sprintf(str, "%d, %d\r\n", adcval, adcval);
		uart0_puts(str);
		_delay_ms(100);*/
			
	}
	return 0;
}

void clock_8MHz(void)
{
	// need to write this signature in order to change protected I/O registers such as CLKPSR
	CCP = 0xD8;				
	CLKPR = 0x00;
}

void Delay(void)	
{ // unconventional loop delay
	volatile unsigned int del = 4000000;
	while(del--);
}
