#ifndef __pwm_h_
#define __pwm_h_

#include "config.h"
#include "adc.h"

void set_duty_cycle(uint16_t);
void HV_ref_init(uint16_t);
void TIM_Config(void);
void HV_set(uint16_t);
void pwm_init_mirror(float, uint16_t); 
void set_pwm_mirror(float , uint16_t ); 

#endif
