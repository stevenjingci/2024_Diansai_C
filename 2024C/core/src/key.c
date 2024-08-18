#include "key.h"

uint32_t tick_key;
uint8_t key_value,key_old,key_down,key_up;
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
}
uint8_t key_scan(void)
{
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)==GPIO_PIN_RESET)return KEY1;
	if(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==GPIO_PIN_RESET)return KEY0;
	return 0;
}
void key_proc(void)
{
	if(uwTick-tick_key<50)return;
	tick_key=uwTick;
	
	key_value=key_scan();
	key_down=key_value&(key_value^key_old);
	key_up=~key_value&(key_value^key_old);
	key_old=key_value;
	
	
	if(key_down==KEY1)
	{
		HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);
		amplification_param2+=0.01;
		AD9959_Set_Amp(CH1, Signal.cw_rms*amplification_param2);  
		IO_Update();	
	}
	if(key_down==KEY0)
	{
		HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_10);
		amplification_param2-=0.01;
		AD9959_Set_Amp(CH1, Signal.cw_rms*amplification_param2);  
		IO_Update();	
	}
	//signal_setting();
}