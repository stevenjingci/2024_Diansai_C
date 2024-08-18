#include "main.h"
void init(void)
{
	//system
	HAL_Init();
  SystemClock_Config();
	//core
	KEY_Init();
	MX_USART1_UART_Init();
	MX_DAC_Init();
	dac_value_set();
	//hardware
  Delay_5ms(200);
	AD9959_Init();
	//param init
	Signal_Init();
}	
void signal_setting(void)
{
	//cw setting
	cw_set_frequency();
	cw_set_amptitude();
	//mesetting
	mw_set_frequency();		
	mw_am_param();		
	//phase
	phase_setting();
	//refresh
	IO_Update();	 
}
void screen_Init(void)
{
	printf("page0.n0.val=%d\xff\xff\xff",Signal.cw_rms);
	printf("page0.n1.val=%d\xff\xff\xff",Signal.Ma);
	printf("page0.n2.val=%d\xff\xff\xff",Signal.fc/1000000);
	printf("page0.t2.txt=\"AM\"\xff\xff\xff");
	printf("page1.n0.val=%d\xff\xff\xff",Signal.reduction_dB);
	printf("page1.n1.val=%d\xff\xff\xff",Signal.delay_ns);
	printf("page1.n2.val=%d\xff\xff\xff",Signal.phase_difference);
}
int main(void)
{
  init();
	signal_setting();
	screen_Init();
	HAL_UART_Receive_IT(&huart1,Rx_data,2);

  while (1)
  {
		UART1_Rx_Proc();
		key_proc();
  }
}
