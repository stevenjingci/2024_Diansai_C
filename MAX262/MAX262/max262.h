#ifndef _MAX262_H_ 
#define _MAX262_H_  
#include "main.h"

#define setWr   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)
#define resetWr HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET)

#define PI 3.141592653 
 
extern enum {MODE_1=0,MODE_2,MODE_3,MODE_4} workMode;
extern enum {CH_A=0,CH_B} channel; 


//void MAX262_Init(void);//IO�ڳ�ʼ��
void MAX262_Write(uint8_t add,uint8_t dat2bit); //д�����ص����ݵ�ĳ��ַ 
void Set_Af(uint8_t datF); //����Aͨ��Fֵ      
void Set_AQ(uint8_t datQ); //����Aͨ��Qֵ   
void Set_Bf(uint8_t datF);  // ����Bͨ��Fֵ  
void Set_BQ(uint8_t datQ); // ����Bͨ��Qֵ 
  

float lhp_WorkFclk(float f0,float Q,uint8_t workMode,uint8_t channel);//���õ�ͨ�͸�ͨ�˲���������ȡ��ʱ��Ƶ��fclk ���л�ģ���ϵ�����ñ���ɸ��ĵ�ͨ���ͨ
float bp_WorkFclk(float fh,float fl,uint8_t workMode,uint8_t channel); //���ô�ͨ�˲���������ȡ��ʱ��Ƶ��fclk 
float bs_WorkFclk(float f0,float Q,uint8_t workMode,uint8_t channel);   //�����ݲ��˲���������ȡ��ʱ��Ƶ��fclk 
float ap_WorkFclk(float f0,float Q,uint8_t channel);//����ȫͨ�˲���������ȡ��ʱ��Ƶ��fclk 


#endif



