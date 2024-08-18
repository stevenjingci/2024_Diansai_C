#ifndef __AD9854_H
#define __AD9854_H
#include "main.h"

#define AD9854_RD_Set (HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET))         
#define AD9854_RD_Clr (HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET))

#define AD9854_FDATA_Set (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET))        //fsk/bpsk/hold  
#define AD9854_FDATA_Clr (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET))

#define AD9854_UDCLK_Set (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET))           
#define AD9854_UDCLK_Clr (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET))

#define AD9854_SP_Set (HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET))         
#define AD9854_SP_Clr (HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET))

#define AD9854_WR_Set (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET))         
#define AD9854_WR_Clr (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET))

#define AD9854_OSK_Set (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET))         
#define AD9854_OSK_Clr (HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET))

#define AD9854_RST_Set (HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET))				   //开关电源板
#define AD9854_RST_Clr (HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET))






//**************************以下部分为函数定义********************************

extern uint16_t add;
extern uint16_t step_up;
extern uint16_t amp_buf;
extern uint8_t modu_buf;



//extern void GPIO_AD9854_Configuration(void);						// AD9854_IO口初始化
void AD9854_gpio_init(void);
static void AD9854_WR_Byte(u8 addr,u8 dat);	  
extern void AD9854_Init(void);						  
static void Fre_convert(uint32_t Freq);	         	  
void AD9854_SetSine(u32 Freq,u16 Shape);
static void Freq_double_convert(double Freq);		  
extern void AD9854_SetSine_double(double Freq,u16 Shape);
extern void AD9854_InitFSK(void);				
extern void AD9854_SetFSK(u32 Freq1,u32 Freq2);					  
extern void AD9854_InitBPSK(void);					  
extern void AD9854_SetBPSK(u16 Phase1,u16 Phase2);					
extern void AD9854_InitOSK(void);					 
extern void AD9854_SetOSK(u8 RateShape);					  
extern void AD9854_InitAM(void);					 
extern void AD9854_SetAM(u32 Freq,u16 Shape);					
extern void AD9854_InitRFSK(void);					 
extern void AD9854_SetRFSK(u32 Freq_Low,u32 Freq_High,u32 Freq_Up_Down,u32 FreRate);
extern void AD9854_InitChirp(void);
extern void AD9854_SetChirp(unsigned long Freq_Low,unsigned long Freq_Up_Down,unsigned long FreRate);
extern void am_test (uint32_t fund_fre, uint8_t modulation);


#endif 