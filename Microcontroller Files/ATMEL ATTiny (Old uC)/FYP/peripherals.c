/*
 * peripherals.c
 *
 * Created: 23/05/2015 16:08:22
 *  Author: Kishan Amratia
 */ 

#include "peripherals.h"
#include "uart.h"

void io_init(void)
{
	// Setup HV_Enable Pin
	HV_ON_DIRECTION_PORT |= (1<<HV_ON);	
	HV_ON_PORT &= ~(1<<HV_ON);
	
	// Setup Mirror Control Pin
	//MIRROR_CONTROL_DIRECTION_PORT |= (1<<MIRROR_CONTROL);	
	//MIRROR_CONTROL_PORT &= ~(1<<MIRROR_CONTROL);	
	//MIRROR_CONTROL_DIRECTION_PORT &= ~(1<<MIRROR_CONTROL);	
	
	// Setup the Capacitance Input Pin
}

void mirror_control(void)
{
	MIRROR_CONTROL_PORT |= (1<<MIRROR_CONTROL);
	_delay_ms(200);
	MIRROR_CONTROL_PORT &= ~(1<<MIRROR_CONTROL);
}

void HV_en(uint8_t en)
{
	if (!en){
		HV_ON_PORT &= ~(1<<HV_ON);}
	else{
		HV_ON_PORT |= (1<<HV_ON);	
	}
}

void mirror_drive_on(void)
{
	MIRROR_CONTROL_PORT |= (1<<MIRROR_CONTROL);	
}

void mirror_drive_off(void)
{
	MIRROR_CONTROL_PORT &= ~(1<<MIRROR_CONTROL);	
}

void pwm_init(void)
{
	// Timer/Counter Compare Pin MUX Channel Output Enable
	TOCPMCOE = (1<<PWM_OUTPUT_COMPARE);	// Select the output pin of the PWM
	
	// initialize TCCR0 (Timer/Counter0) to Fast PWM mode 
	TCCR0A |= (1<<WGM00)|(1<<COM0B1)|(1<<WGM01);
	TCCR0B |= (1<<CS00);	// No prescaler on clock
	
	#ifdef CLOCK8MHZ
	TCCR0B |= (1<<CS01);	// Clock Pre-scaler to 8
	#endif
	
	PWM_DIRECTION_PORT |= (1<<PWM_ADJUST); // set direction of PWM_ADJUST as output
}

void HV_set(uint8_t voltage)	// set the voltage wanted at the HV converter output
{
	float val = 0;
	
	val = (voltage - 1.0517)/1.3455;
	
	if(voltage > HV_LIMIT)
	{
		val = (HV_LIMIT- 1.0517)/1.3455;
	}
	
	PWM_COMPARE_REGISTER = (uint8_t) val;	
}

void adc_init(void)
{
	ADMUXB = 0x40;							// Internal 2.2V reference
	//ADMUXB |= (1<<REFS1)|(1<<REFS0);		// VCC Reference
	
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);		// Set Pre-scaler for ADC Clock to 8
	//ADCSRA |= (1<<ADPS0);					// Set Pre-scaler for ADC Clock to 2
	//ADCSRA |= (1<<ADPS2)|(1<<ADPS0);		// Set Pre-scaler for ADC Clock to 32
	#ifdef CLOCK8MHZ
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1);		// Set Pre-scaler for ADC Clock to 64
	#endif
	
	ADCSRA |= (1<<ADEN);					// Enable the ADC
}

uint16_t adc_read(uint8_t channel)
{
	ADMUXA = (ADMUXA &~(0x1F)) | (channel & 0x1F);	// select ADC conversion channel
	ADCSRA |= (1<<ADSC);			// start conversion
	while(ADCSRA & (1<<ADSC));		// Wait until conversion is over
	return ADCW;					// return the ADC result word
}

uint16_t adc_read_avg(uint8_t channel, uint8_t nsamples)
{
	uint32_t sum = 0;
	for(uint8_t i = 0; i < nsamples; i++)
	{
		sum+=adc_read(channel);
	}
	return (uint16_t)(sum/nsamples);
}

void process_input(void){
	const int buf_size = 20;
	static char cmd[20] = {0};
	char cmd_ptr = 0;
	char c;
	uint8_t cmd_number = 0;
	static uint8_t val = 0;
	char str[100];

	while(uart0_available())
	{
		c = uart0_getc();
		cmd[cmd_ptr++] = c;
		
		// If a command is entered (determined by the last character)
		if (c == '\n'){
			//cmd[cmd_ptr - 1] = 0;
			
			sscanf(cmd, "%d,%d", &cmd_number, &val);
			switch(cmd_number)
			{
				case 1:	//HV on/off
				if(val == 1)	HV_ON_PORT |= (1<<HV_ON);
				else HV_ON_PORT &= ~(1<<HV_ON);
				
				case 2:	//PWM DAC Set
				PWM_COMPARE_REGISTER = val;
				//case 3:	// Mirror Control
			}
			
			sprintf(str, "ok,cmd=%d val=%d\r\n", cmd_number, val);
			uart0_puts(str);
			cmd_ptr = 0;
			
			for(uint8_t i=0; i<20; i++)
			{
				cmd[i] = 0;
			}
		}
	}
	
}