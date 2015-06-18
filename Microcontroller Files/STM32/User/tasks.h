#include "config.h"
#include "pwm.h"
#include "adc.h"
#include "timer.h"
#include "io.h"
#include "serial.h"

// Public Variables

//Task Parameters
extern OS_TID aux_t;
#define AUX_STK_SIZE 50
extern U64 aux_stk[AUX_STK_SIZE];

extern OS_TID main_t;
#define MAIN_STK_SIZE 50
extern U64 main_stk[MAIN_STK_SIZE];

__task void aux_task(void);
__task void main_task(void);
