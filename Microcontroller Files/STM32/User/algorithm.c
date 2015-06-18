#include "algorithm.h"

volatile uint16_t val1 = 0;
volatile uint16_t val2 = 0;
volatile uint16_t val3 = 0;
volatile uint16_t result = 0;
signed int d1 = 0;
signed int d2 = 0;
signed int diff = 0;
volatile int drive_flg;

void closed_loop(void)
{
	val3 = val2;
	val2 = val1;
	val1 = get_capacitance();
	
	// compute derivatives
	d1 = val1 - val2;
	d2 = val2 - val3;
	
	diff = d2 - d1;
	
	if(diff>THRESHOLD) // falling derivative
	{ 
		if(d2>=0 && d1<=0){
			//mirror_drive_en(0);
			drive_flg = 1;
			//dead_band(DEAD_BAND);
		}
	}
	else if (diff<THRESHOLD2)// rising derivative
	{
		if(d2<=0 && d1>=0){
			//mirror_drive_en(1);
			drive_flg = 2;
			//dead_band(DEAD_BAND);
		}
	}	
}

void startup_seq(uint8_t cycles, uint8_t freq, uint8_t duty)
{
	volatile int i = 0;
	volatile float time_on = 0;
	volatile float time_off = 0;
	
	float period = 1/((float)freq);
	float duty_f = (float)duty;
	
	time_on = ((duty_f/100.0)*(period)*(1000000.0));
	time_off = ((100.0-duty_f)/100.0)*(period)*(1000000.0);
	
	
	while(i<cycles)
	{
		mirror_drive_en(0);
		delay_us((uint16_t) time_on);
		mirror_drive_en(1);
		delay_us((uint16_t) time_off);
		i++;
		
	}
}

void delay_ms (volatile int ms) {
	while (ms--)
	{
		delay_us(1000);
	}
}

void delay_us (volatile int us) {
	// delays the processor for approximately 1 - 1.5 microseconds
  // at default clock speed (40MHz)
  volatile int count;
  while(us--) 
  {
    count=6;
    while(count--);
  }
}

void dead_band(uint16_t us)
{
	delay_us(us);
}
