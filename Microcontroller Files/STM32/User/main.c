#include "config.h"
#include "serial.h"
#include "tasks.h"

uint32_t Counter;
uint32_t avg_counter;

__task void init(void)
{	
	//Maximum of four running tasks, so can only launch three tasks here, any more will be ignored.
	//LED_t = os_tsk_create_user(LED_task, 0, &LED_stk, sizeof(LED_stk));
	
	main_t = os_tsk_create_user(main_task, 0, &main_stk, sizeof(main_stk));
	aux_t = os_tsk_create_user(aux_task, 1, &aux_stk, sizeof(aux_stk));
	
	os_tsk_delete_self ();
}


int main(void)
{
	Serial.begin(115200);		// initialise serial interface at 115200 baud rate
	IO_init();							// initialise all I/O pins

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
