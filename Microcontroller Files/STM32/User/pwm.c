#include "pwm.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure_PWM;
TIM_OCInitTypeDef  TIM_OCInitStructure_PWM;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure_PWM1;
TIM_OCInitTypeDef  TIM_OCInitStructure_PWM1;
uint16_t TimerPeriod = 0;
uint16_t Pulse;
uint16_t PrescalerV = 0;

void HV_set(uint16_t voltage)
{
	//sets the duty cycle according the HV Converter voltage demanaded
	
	if(voltage >= HV_LIMIT)
		set_duty_cycle((uint16_t)(HV_LIMIT/VCC));
	else
		set_duty_cycle((uint16_t)(voltage/VCC));
}

void set_duty_cycle(uint16_t duty_cycle)
{
	// duty cycle is a value between 0 and 100
	Pulse = (uint16_t) (((uint32_t) (100-duty_cycle)*(TimerPeriod-1))/100);
	TIM_OCInitStructure_PWM.TIM_Pulse = Pulse;	
	TIM_OC3Init(PWM_TIMER, &TIM_OCInitStructure_PWM);
}

void pwm_init(uint16_t freq) 
{
	// configure the GPIO Alternate function pins for the PWM
	GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOB Clocks enable */
  RCC_AHBPeriphClockCmd(PWM_VADJUST_CLK, ENABLE);
  
  /* GPIOB Configuration: Channel 3N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = PWM_VADJUST_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(PWM_VADJUST_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(PWM_VADJUST_PORT, PWM_VADJUST_AF_PIN, PWM_VADJUST_AF);

	/* Compute the value to be set in ARR regiter to generate signal frequency set */
  TimerPeriod = (SystemCoreClock/freq) - 1;
	/* Compute CCR1 value to generate a duty cycle at 0%  for the channel */
	Pulse = (uint16_t) (((uint32_t) 100 * (TimerPeriod - 1)) / 100);

  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(PWM_TIMER_CLK , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure_PWM.TIM_Prescaler = 0;
  TIM_TimeBaseStructure_PWM.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure_PWM.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure_PWM.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure_PWM.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(PWM_TIMER, &TIM_TimeBaseStructure_PWM);

  /* Channel 3 Configuration in PWM mode */
  TIM_OCInitStructure_PWM.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure_PWM.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure_PWM.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure_PWM.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure_PWM.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure_PWM.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure_PWM.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure_PWM.TIM_Pulse = Pulse;
  TIM_OC3Init(PWM_TIMER, &TIM_OCInitStructure_PWM);
	
	 /* TIM1 counter enable */
  TIM_Cmd(PWM_TIMER, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(PWM_TIMER, ENABLE);
}

void pwm_init_mirror(float freq, uint16_t duty_cycle) 
{
	
	// configure the GPIO Alternate function pins for the PWM
	GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA Clocks enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* GPIOA Configuration: Channel 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_0);

	/* Compute the value to be set in ARR regiter to generate signal frequency set */
  TimerPeriod = (SystemCoreClock/(freq*128)) - 1;
	/* Compute CCR1 value to generate a duty cycle at 0%  for the channel */
	Pulse = (uint16_t) (((uint32_t) (100- duty_cycle) * (TimerPeriod - 1)) / 100);

  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15 , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure_PWM1.TIM_Prescaler = 128;
  TIM_TimeBaseStructure_PWM1.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure_PWM1.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure_PWM1.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure_PWM1.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure_PWM1);

  /* Channel 2 Configuration in PWM mode */
  TIM_OCInitStructure_PWM1.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure_PWM1.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure_PWM1.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure_PWM1.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure_PWM1.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure_PWM1.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure_PWM1.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure_PWM1.TIM_Pulse = Pulse;
  TIM_OC2Init(TIM15, &TIM_OCInitStructure_PWM1);
	
	 /* TIM1 counter enable */
  TIM_Cmd(TIM15, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM15, ENABLE);
}


void set_pwm_mirror(float freq, uint16_t duty_cycle) 
{
	TimerPeriod = (SystemCoreClock/(freq*128)) - 1;
	Pulse = (uint16_t) (((uint32_t) (100-duty_cycle)*(TimerPeriod-1))/100);
	TIM_TimeBaseStructure_PWM1.TIM_Period = TimerPeriod;
	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure_PWM1);
	TIM_OCInitStructure_PWM.TIM_Pulse = Pulse;	
	TIM_OC2Init(TIM15, &TIM_OCInitStructure_PWM1);
}
	

