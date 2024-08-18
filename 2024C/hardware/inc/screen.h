#ifndef __SCREEN_H__
#define __SCREEN_H__
#include "main.h"


extern uint8_t AM_Flag;//1代表AM 模式，2代表CW模式
extern uint8_t Rx_data[2];
extern uint8_t RxProc_Flag;//接收数据处理完成标志，0代表未处理，1代表已处理


void UART1_Rx_Proc(void);












#endif