#include "config.h"
#include "adc.h"

//Task Parameters
extern OS_TID LED_t;
#define LED_STK_SIZE 80
extern U64  LED_stk[LED_STK_SIZE];

__task void LED_task(void);
void LED_Init(void);
