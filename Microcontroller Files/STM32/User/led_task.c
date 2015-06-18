#include "led_task.h"
#include "pwm.h"
#include "timer.h"

//Public Variables
U64 LED_stk[LED_STK_SIZE];
OS_TID LED_t;
uint16_t value =0;

__task void LED_task(void)
{
	LED_Init();
	adc_init();
	TIM3_Config(10000);		//setup frequency of 10kHz
	
	/*GPIO_SetBits(GPIOC, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
	
	os_dly_wait(50);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	*/
	
	while(1)
	{
		
	}
}

void LED_Init(void)
{
	// Initializes pins PC8 and PC9 as outputs to drive the LEDS on the Discovery Board
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
