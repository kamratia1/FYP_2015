#include "io.h"

void mirror_drive_en(uint8_t en)
{
	// Set(1) or reset(0) the mirror drive signal
	if(!en)	GPIO_SetBits(MIRROR_CONTROL_PORT, MIRROR_CONTROL_PIN);
	else	GPIO_ResetBits(MIRROR_CONTROL_PORT, MIRROR_CONTROL_PIN);
}

void HV_en(uint8_t en)
{
	//enable(1) or disable(0) the HV Converter
	if(!en)	GPIO_ResetBits(HV_EN_PORT, HV_EN_PIN);
	else	GPIO_SetBits(HV_EN_PORT, HV_EN_PIN);
}

void SYNC_en(uint8_t en)
{
	// set(1) or reset(0) the SYNC signal
	if(!en)	GPIO_ResetBits(SYNC_PORT, SYNC_PIN);
	else	GPIO_SetBits(SYNC_PORT, SYNC_PIN);
}

void IO_init(void)
{
	// Initialise all digital output pins
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(MIRROR_CONTROL_CLK, ENABLE);
	RCC_AHBPeriphClockCmd(HV_EN_CLK, ENABLE);
	RCC_AHBPeriphClockCmd(SYNC_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = MIRROR_CONTROL_PIN;
  GPIO_Init(MIRROR_CONTROL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HV_EN_PIN;
	GPIO_Init(HV_EN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SYNC_PIN;
	GPIO_Init(SYNC_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(MIRROR_CONTROL_PORT, MIRROR_CONTROL_PIN);
	GPIO_ResetBits(HV_EN_PORT, HV_EN_PIN);
	GPIO_ResetBits(SYNC_PORT, SYNC_PIN);
}
