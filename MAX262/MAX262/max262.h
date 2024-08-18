#ifndef _MAX262_H_ 
#define _MAX262_H_  
#include "main.h"

#define setWr   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)
#define resetWr HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET)

#define PI 3.141592653 
 
extern enum {MODE_1=0,MODE_2,MODE_3,MODE_4} workMode;
extern enum {CH_A=0,CH_B} channel; 


//void MAX262_Init(void);//IO口初始化
void MAX262_Write(uint8_t add,uint8_t dat2bit); //写两比特的数据到某地址 
void Set_Af(uint8_t datF); //设置A通道F值      
void Set_AQ(uint8_t datQ); //设置A通道Q值   
void Set_Bf(uint8_t datF);  // 设置B通道F值  
void Set_BQ(uint8_t datQ); // 设置B通道Q值 
  

float lhp_WorkFclk(float f0,float Q,uint8_t workMode,uint8_t channel);//设置低通和高通滤波器工作并取得时钟频率fclk ，切换模块上的跳线帽即可更改低通或高通
float bp_WorkFclk(float fh,float fl,uint8_t workMode,uint8_t channel); //设置带通滤波器工作并取得时钟频率fclk 
float bs_WorkFclk(float f0,float Q,uint8_t workMode,uint8_t channel);   //设置陷波滤波器工作并取得时钟频率fclk 
float ap_WorkFclk(float f0,float Q,uint8_t channel);//设置全通滤波器工作并取得时钟频率fclk 


#endif



