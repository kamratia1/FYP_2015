#include "setup.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void pwm_init(void);
void set_pwm_voltage(uint8_t);
void adc_init(void);
uint16_t adc_read(uint8_t);
void adc_off(void);
uint16_t adc_read_avg(uint8_t channel, uint8_t nsamples);
void process_input(void);
void mirror_control(void);
void io_init(void);
void HV_en(uint8_t);
void mirror_drive_on(void);
void mirror_drive_off(void);
