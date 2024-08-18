#ifndef __UART_H
#define __UART_H

#include "main.h"

typedef struct 
{
	uint32_t cw_rms;
	uint32_t fc;
	uint8_t  Ma;
	uint8_t reduction_dB;
	uint16_t delay_ns;
	uint8_t phase_sd;
	uint8_t phase_difference;
}Param;
extern Param Signal;


extern UART_HandleTypeDef huart1;
void MX_USART1_UART_Init(void);
void Signal_Init(void);



#endif 