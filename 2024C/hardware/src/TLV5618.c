#include "TLV5618.h"

void TLV5618_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOD_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);                
 
	TLV5618_CS_H ;
	TLV5618_CLK_L ;
	TLV5618_DIN_L ;
	
}
 
/*!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!*/
static int 
EX_DAC_Conver(unsigned short Dignum)
{
	unsigned short Dig = 0;
 
	TLV5618_CS_L;	
	TLV5618_CLK_H;
	
	for (int idx = 0; idx < 16; ++idx) {
		
        Dig=Dignum&0x8000;
 
		if (Dig) { TLV5618_DIN_H; }
		else     { TLV5618_DIN_L; }
 
		for (int delayTime = 0; delayTime < 100; ++delayTime );//for delay only
 
		TLV5618_CLK_L;
 
		Dignum<<=1;
 
		for (int delayTime = 0; delayTime < 100; ++delayTime );//for delay only
 
		TLV5618_CLK_H;
	}
 
	TLV5618_CLK_L;
	TLV5618_CS_H;	
 
	for (int delayTime = 0; delayTime < 1000; ++delayTime );//for delay only
 
	return 0;
}
/*!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!*/
void Write_A_B(unsigned short Data_A, unsigned short Data_B, unsigned char Channal)
{
	unsigned short Temp=0x4000;
	switch(Channal)
	{
		case Channal_A:		EX_DAC_Conver(Temp|0x8000|(0x0fff&Data_A));break;
		case Channal_B:		EX_DAC_Conver(Temp|0x0000|(0x0fff&Data_B));break;
		case Channal_AB:	EX_DAC_Conver(Temp|0x1000|(0x0fff&Data_B));EX_DAC_Conver(Temp|0x8000|(0x0fff&Data_A));break;
		default:break;
	}
}