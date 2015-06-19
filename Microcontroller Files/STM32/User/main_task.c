#include "tasks.h"
#include "algorithm.h"

void drive_transition(void);

//Public Variables
U64 main_stk[MAIN_STK_SIZE];
OS_TID main_t;

uint32_t Counter;

#define PHASE_DELAY 	3500  // in microseconds

__task void main_task(void)
{
	HV_set(HV_DEFAULT);			// Set HV Converter Duty cycle such that it outputs the default voltage set in config.h
	HV_en(1);								// Turn on HV_Converter
	
	os_dly_wait(50);				// wait for HV_Converter to settle
	
	startup_seq(50, 97, 25);	// Startup Sequence runs for 15 cycles, at 100 Hz Frequency and 25% duty cycle
	
	//pwm_init_mirror(100,50);
	//os_dly_wait(50);
	
	TIM3_Config(SAMP_FREQ);	// Setup 10kHz, sampling frequency
	
	while(1)
	{
		drive_transition();
	}

}

void drive_transition(void)
{
	// Function that determines the drive transistion i.e. rising or falling edge based on the capacitance derivative
	if(!drive_flg)
		{
			TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);	// Disable the Interrput timer
			
			if(drive_flg)
			{
				Counter = TIM_GetCounter(TIM2);					// Get counter value from timer2
				TIM_SetCounter(TIM2, 0);								// Reset the counter to zero
				
				dead_band(PHASE_DELAY);
				mirror_drive_en(1);			
			}
			else 
			{	
				dead_band(PHASE_DELAY-400);
				mirror_drive_en(0);
			}
			
			TIM3_Config(SAMP_FREQ);		// Reset the interrupt timer
			drive_flg = 0;						// Reset the flag
		}	
}
