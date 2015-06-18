#include "tasks.h"
#include "algorithm.h"

//Public Variables
U64 main_stk[MAIN_STK_SIZE];
OS_TID main_t;
uint16_t value;
extern uint32_t Counter;
extern uint32_t avg_counter;
int i = 0;
float freq = 0.0;
#define PHASE_DELAY 	3500  // in microseconds

__task void main_task(void)
{
	TIM2_Config();
	adc_init();
	pwm_init(40000);				// Setup PWM Frequency at 40kHz
	HV_set(HV_DEFAULT);			// Set HV Converter Duty cycle such that it outputs the default voltage set in config.h
	HV_en(1);								// Turn on HV_Converter
	
	os_dly_wait(50);	// wait for HV_Converter to settle
	
	startup_seq(50, 97, 25);	// Startup Sequence runs for 15 cycles, at 100 Hz Frequency and 25% duty cycle
	pwm_init_mirror(100,50);
	//os_dly_wait(50);
	
	TIM3_Config(SAMP_FREQ);	// Setup 10kHz, sampling frequency
	
	freq = 120.0;
	
	while(1)
	{
		/*if(freq < 85)	freq = 120.0;
		
		pwm_init_mirror(freq,50);
		freq = freq -0.2;
		
		os_dly_wait(10);*/
		
		//TRACE_INFO("%d, %f\r\n", value, (value*ADC_VREF)/(0xFFF));
		if(drive_flg>=1)
		{
			TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);
			if(drive_flg == 1)
			{
				Counter = TIM_GetCounter(TIM2);
				TIM_SetCounter(TIM2, 0);
				dead_band(PHASE_DELAY);
				mirror_drive_en(1);
				//dead_band(2500);
				//mirror_drive_en(0);	
			}
			else 
			{	
				dead_band(PHASE_DELAY-400);
				mirror_drive_en(0);
				//mirror_drive_en(1);
				//dead_band(DEAD_BAND);
			}
			TIM3_Config(SAMP_FREQ);
			drive_flg = 0;
		}	
		
	}

}
