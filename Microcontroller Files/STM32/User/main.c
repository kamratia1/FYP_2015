#include "config.h"
#include "serial.h"
#include "tasks.h"

__task void init(void)
{	
	// Setup the different tasks for the RTOS
	
	// Main Task: Runs the algorithm for controlling the mirror
	main_t = os_tsk_create_user(main_task, 0, &main_stk, sizeof(main_stk));	
	
	// Auxillary Task: sends and recevies data to/from PC to set variables in the software
	aux_t = os_tsk_create_user(aux_task, 1, &aux_stk, sizeof(aux_stk));
	
	/* os_tsk_delete_self:
		 Function stops and deletes the currently running task. The function doesnt return.
		 The program execution continues with the task with the next highest priority in the ready queue. 
	*/
	os_tsk_delete_self ();
}


int main(void)
{
	Serial.begin(115200);		// Initialise serial interface at 115200 baud rate
	IO_init();							// Initialise all I/O pins
	ADC_init();							// Initialise ADC for capacitance measurement						
	TIM2_Config();					// Setup Timer2 as a counter for time measurement purposes

	os_sys_init(init); 
	
  while(1);
	
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	//
	//TRACE_FATAL("Wrong parameters value: file %s on line %d\n", file, line);

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
