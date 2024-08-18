#ifndef __TLV5618_H
#define __TLV5618_H
#include "main.h"

#define Channal_A  1
#define Channal_B  2
#define Channal_AB 3

#define TLV5618_CS_H     HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET)
#define TLV5618_CS_L     HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET)
 
#define TLV5618_DIN_H	   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET)
#define TLV5618_DIN_L    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET)
 
#define TLV5618_CLK_H	 	 HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET)
#define TLV5618_CLK_L    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET)

void TLV5618_init(void); 
void Write_A_B(unsigned short Data_A, unsigned short Data_B, unsigned char Channal);

#endif