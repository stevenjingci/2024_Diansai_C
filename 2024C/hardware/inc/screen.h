#ifndef __SCREEN_H__
#define __SCREEN_H__
#include "main.h"


extern uint8_t AM_Flag;//1����AM ģʽ��2����CWģʽ
extern uint8_t Rx_data[2];
extern uint8_t RxProc_Flag;//�������ݴ�����ɱ�־��0����δ����1�����Ѵ���


void UART1_Rx_Proc(void);












#endif