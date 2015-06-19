#ifndef __config_h
#define __config_h

#include "stm32f0xx.h"
#include <RTL.h>
#include <stdio.h>

// Value definitions
#define HV_DEFAULT	50			// default value for High Voltage Converter
#define HV_LIMIT		160			// HV Converter Voltage limit
#define VCC					3.3			// Define VCC for determining the duty cycle for the HV Converter reference apprioprately

#define SAMP_FREQ		10000		// Define Sampling Frequency for the control algorithm

/* Pin Definitions */

// GPIOs
#define MIRROR_CONTROL_PIN			GPIO_Pin_8		// PC8
#define MIRROR_CONTROL_PORT			GPIOC
#define MIRROR_CONTROL_CLK			RCC_AHBPeriph_GPIOC

#define HV_EN_PIN								GPIO_Pin_9		// PC9
#define HV_EN_PORT							GPIOC
#define HV_EN_CLK								RCC_AHBPeriph_GPIOC

#define SYNC_PIN								GPIO_Pin_1		// PF1
#define SYNC_PORT								GPIOF
#define SYNC_CLK								RCC_AHBPeriph_GPIOF

// ADC for PICOCAP Capacitance Analogue Values
#define PCAP_PDM_PIN							GPIO_Pin_1		// PA1 
#define PCAP_PDM_PORT							GPIOA
#define PCAP_PDM_CLK							RCC_AHBPeriph_GPIOA
#define PCAP_ADC_SAMPLING_TIME		ADC_SampleTime_13_5Cycles
#define PCAP_PDM_ADC_CHANNEL			ADC_Channel_1 	

// PWM for HV Converter Voltage Adjust Definitions
#define PWM_VADJUST_PIN					GPIO_Pin_8		//PA8
#define PWM_VADJUST_PORT				GPIOA
#define PWM_VADJUST_CLK					RCC_AHBPeriph_GPIOA
#define PWM_VADJUST_AF_PIN			GPIO_PinSource8		
#define PWM_VADJUST_AF					GPIO_AF_2					
#define PWM_TIMER_CLK						RCC_APB2Periph_TIM1		
#define PWM_TIMER								TIM1							

// UART pin definitions can be found in serial.h

//#define DEBUG	// Defines TRACE level

#ifdef DEBUG
	#define USE_FULL_ASSERT
	#warning "Using Debug mode, for production do not use debug mode"
	#define TRACE_LEVEL TRACE_LEVEL_DEBUG
#else
	#define TRACE_LEVEL TRACE_LEVEL_INFO
#endif

#include "trace.h"

#endif
