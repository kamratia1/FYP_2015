#ifndef __adc_h
#define __adc_h

#include "stm32f0xx_adc.h"
#include "config.h"


#ifndef ADC_VREF
	#define ADC_VREF 3.0f
#endif

#define NO_SAMPLES 50
#define NO_CHANNELS 1

//Public Variables
extern volatile uint16_t RegularConvData_Tab[NO_SAMPLES*NO_CHANNELS];

/* Private variables ---------------------------------------------------------*/

/* Public function prototypes -----------------------------------------------*/
void adc_init(void);
uint16_t get_capacitance(void);
uint16_t get_capacitance_value(void);

#endif
