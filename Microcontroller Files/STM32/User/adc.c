#include "adc.h"

#define ADC1_DR_Address 0x40012440

volatile uint16_t RegularConvData_Tab[NO_SAMPLES * NO_CHANNELS];

uint16_t get_capacitance(void)
{
	int i;
	uint32_t sum = 0;
	
	/* Test DMA1 TC flag */
	while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET ); 
	
	/* Clear DMA TC flag */
	DMA_ClearFlag(DMA1_FLAG_TC1);

	sum = 0;
	for (i=0; i<NO_SAMPLES; i++)
	{
		// average the values in the conversion buffer
		sum += RegularConvData_Tab[(i*NO_CHANNELS)+1];
	}
	
	sum = sum/NO_SAMPLES;
				
	return sum;
}

uint16_t get_capacitance_value(void)
{
	/* Test DMA1 TC flag */
	while((DMA_GetFlagStatus(DMA1_FLAG_TC1)) == RESET ); 
	
	/* Clear DMA TC flag */
	DMA_ClearFlag(DMA1_FLAG_TC1);
	
	return RegularConvData_Tab[(NO_SAMPLES*NO_CHANNELS)-1];

}

void adc_init( void )
{
	
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
		
  RCC_AHBPeriphClockCmd(PCAP_PDM_CLK, ENABLE);
  
	GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = PCAP_PDM_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(PCAP_PDM_PORT, &GPIO_InitStructure);
  
  // ADC1 DeInit    
  ADC_DeInit(ADC1);
  
  // ADC1 Periph clock enable  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  // DMA1 clock enable  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
  
  // DMA1 Channel1 Config  
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = (NO_SAMPLES*NO_CHANNELS);
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  // DMA1 Channel1 enable  
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  // ADC DMA request in circular mode  
  ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
  
  // Enable ADC_DMA  
  ADC_DMACmd(ADC1, ENABLE);  
  
  // Initialize ADC structure  
  ADC_StructInit(&ADC_InitStructure);
  
  // Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits   
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
	
	//ADC_JitterCmd(ADC1, ADC_JitterOff_PCLKDiv4, ENABLE);
 
	//ADC Frequency set as 12MHz
	
  // Convert the PCAP_PDM analog input with PCAP_ADC_SAMPLING_TIME (Defined in config.h) ADC Cycles as sampling time      
  ADC_ChannelConfig(ADC1, PCAP_PDM_ADC_CHANNEL , PCAP_ADC_SAMPLING_TIME);
  
  // ADC Calibration  
  ADC_GetCalibrationFactor(ADC1);
  
  // Enable ADC1  
  ADC_Cmd(ADC1, ENABLE);     
  
  // Wait the ADCEN flag  
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 

  // ADC1 regular Software Start Conv   
  ADC_StartOfConversion(ADC1);
}
