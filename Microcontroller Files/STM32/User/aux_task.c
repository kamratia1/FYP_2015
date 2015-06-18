#include "tasks.h"

//Public Variables
U64 aux_stk[AUX_STK_SIZE];
OS_TID aux_t;
extern uint32_t Counter;
uint32_t avg = 0;
int j = 0;
float x = 0.0; 
int k = 0;

void process_input(void);

char str[50];

__task void aux_task(void)
{
	
	//uint16_t value = 400;
	while(1)
	{
		process_input();
		os_dly_wait(5);		// Delay of 4 is the limit
		
		//send data to PC
		sprintf(str, "%d\r\n", get_capacitance_value());
		print_fcn(str);
		
		/*
		avg += Counter;
		j++;
		
		if(j>10)
		{		
			x = 2*48000000.0/(avg/10.0);
			sprintf(str, "%f\r\n", x);
			print_fcn(str);
			j=0;
			avg = 0;
			//if(x>92 && x<95)
			//pwm_init_mirror((x),50.0);
		}*/
		
	}

}

void process_input(void)
{
	static char cmd[10] = {0};
	char cmd_ptr = 0;
	char c;
	int cmd_number = 0;
	static int val = 0;

	while(available_fcn())
	{
		c = read_fcn();
		cmd[cmd_ptr++] = c;
		
		// If a command is entered (determined by the last character)
		if (c == '\n'){
			cmd[cmd_ptr - 1] = 0;
			
			sscanf(cmd, "s,%d,%d", &cmd_number, &val);
			switch(cmd_number)
			{
				case 1:	//HV on/off
					HV_en(val);
					break;
				case 2:	//HV_Voltage Set
					HV_set(val);
					break;
				//case 3:	//Reset_Program i.e. Do a software reset
			}
			
			sprintf(str, "k,cmd=%d val=%d\r\n", cmd_number, val);
			print_fcn(str);
			cmd_ptr = 0;
		}
	}
	
}
