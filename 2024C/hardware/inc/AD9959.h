#ifndef __AD9959_H
#define __AD9959_H

#include "main.h"

#define AD9959_PWR_HIGH	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);	
#define AD9959_PWR_LOW	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);	

#define Reset_HIGH		  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
#define Reset_LOW		    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_RESET);

#define UPDATE_HIGH	    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET);
#define UPDATE_LOW	    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_RESET);

#define CS_HIGH	        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_SET);
#define CS_LOW	        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_RESET);

#define SCLK_HIGH	      HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);
#define SCLK_LOW	      HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);

#define SDIO3_HIGH	    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8,GPIO_PIN_SET);
#define SDIO3_LOW	      HAL_GPIO_WritePin(GPIOG,GPIO_PIN_8,GPIO_PIN_RESET);


#define PS0_HIGH	      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
#define PS0_LOW	        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);

#define PS1_HIGH	      HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_SET);
#define PS1_LOW	        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_11,GPIO_PIN_RESET);

#define PS2_HIGH	      HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
#define PS2_LOW	        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);

#define PS3_HIGH	      HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_SET);
#define PS3_LOW	        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);

#define SDIO0_HIGH	    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_SET);
#define SDIO0_LOW	      HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_RESET);

#define SDIO1_HIGH	    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_SET);
#define SDIO1_LOW	      HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);

#define SDIO2_HIGH	    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
#define SDIO2_LOW	      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);





//AD9959�Ĵ�����ַ����
#define CSR_ADD   0x00   //CSR ͨ��ѡ��Ĵ���
#define FR1_ADD   0x01   //FR1 ���ܼĴ���1
#define FR2_ADD   0x02   //FR2 ���ܼĴ���2
#define CFR_ADD   0x03   //CFR ͨ�����ܼĴ���

#define CFTW0_ADD 0x04   //CTW0 ͨ��Ƶ��ת���ּĴ���
#define CPOW0_ADD 0x05   //CPW0 ͨ����λת���ּĴ���
#define ACR_ADD   0x06   //ACR ���ȿ��ƼĴ���

#define LSRR_ADD  0x07   //LSR ����ɨ��б�ʼĴ���
#define RDW_ADD   0x08   //RDW ����ɨ�������Ĵ���
#define FDW_ADD   0x09   //FDW �½�ɨ�������Ĵ���

#define PROFILE_ADDR_BASE   0x0A   //Profile�Ĵ���,�����ļ��Ĵ�����ʼ��ַ

//CSR[7:4]ͨ��ѡ������λ
#define CH0 0x10
#define CH1 0x20
#define CH2 0x40
#define CH3 0x80

//FR1[9:8] ���Ƶ�ƽѡ��λ
#define LEVEL_MOD_2  	0x00//2��ƽ���� 2�׵���
#define LEVEL_MOD_4		0x01//4��ƽ����	4�׵���
#define LEVEL_MOD_8		0x02//8��ƽ����	8�׵���
#define LEVEL_MOD_16	0x03//16��ƽ����	16�׵���

//CFR[23:22]  ��Ƶ��λ��AFP��ѡ��λ
#define	DISABLE_Mod		0x00	//00	�����ѽ���
#define	ASK 					0x40	//01	������ƣ����Ƽ���
#define	FSK 					0x80	//10	Ƶ�ʵ��ƣ�Ƶ�Ƽ���
#define	PSK 					0xc0	//11	��λ���ƣ����Ƽ���

//��CFR[14]������ɨ������ sweep enable																				
#define	SWEEP_ENABLE	0x40	//1	����
#define	SWEEP_DISABLE	0x00	//0	������
		
void delay1 (uint32_t length);//��ʱ
void IntReset(void);	 			//AD9959��λ
void IO_Update(void); 		  //AD9959��������
void Intserve(void);				//IO�ڵ�ƽ״̬��ʼ��
void AD9959_Init(void);			//IO�ڳ�ʼ��

/***********************AD9959�����Ĵ�����������*****************************************/
void AD9959_WriteData(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData);//��AD9959д����
void Write_CFTW0(uint32_t fre);										//дCFTW0ͨ��Ƶ��ת���ּĴ���
void Write_ACR(uint16_t Ampli);										//дACRͨ������ת���ּĴ���
void Write_CPOW0(uint16_t Phase);									//дCPOW0ͨ����λת���ּĴ���

void Write_LSRR(uint8_t rsrr,uint8_t fsrr);				//дLSRR����ɨ��б�ʼĴ���
void Write_RDW(uint32_t r_delta);									//дRDW���������Ĵ���
void Write_FDW(uint32_t f_delta);									//дFDW�½������Ĵ���

void Write_Profile_Fre(uint8_t profile,uint32_t data);//дProfile�Ĵ���,Ƶ��
void Write_Profile_Ampli(uint8_t profile,uint16_t data);//дProfile�Ĵ���,����
void Write_Profile_Phase(uint8_t profile,uint16_t data);//дProfile�Ĵ���,��λ                                                                                                                                                                               
/********************************************************************************************/


/*****************************��Ƶ��������***********************************/
void AD9959_Set_Fre(uint8_t Channel,uint32_t Freq); //дƵ��
void AD9959_Set_Amp(uint8_t Channel, uint16_t Ampli);//д����
void AD9959_Set_Phase(uint8_t Channel,uint16_t Phase);//д��λ
/****************************************************************************/

/*****************************���Ʋ�������  ***********************************/
void AD9959_Modulation_Init(uint8_t Channel,uint8_t Modulation,uint8_t Sweep_en,uint8_t Nlevel);//���ø���ͨ���ĵ���ģʽ��
void AD9959_SetFSK(uint8_t Channel, uint32_t *data,uint16_t Phase);//����FSK���ƵĲ���
void AD9959_SetASK(uint8_t Channel, uint16_t *data,uint32_t fre,uint16_t Phase);//����ASK���ƵĲ���
void AD9959_SetPSK(uint8_t Channel, uint16_t *data,uint32_t Freq);//����PSK���ƵĲ���

void AD9959_SetFre_Sweep(uint8_t Channel, uint32_t s_data,uint32_t e_data,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint16_t Ampli,uint16_t Phase);//��������ɨƵ�Ĳ���
void AD9959_SetAmp_Sweep(uint8_t Channel, uint32_t s_Ampli,uint16_t e_Ampli,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Phase);//��������ɨ���Ĳ���
void AD9959_SetPhase_Sweep(uint8_t Channel, uint16_t s_data,uint16_t e_data,uint16_t r_delta,uint16_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Ampli);//��������ɨ��Ĳ���


/***************************************2024C************************************************/
extern float amplification_param1,amplification_param2,sm_reduction_param;
void mw_set_frequency(void);
void mw_am_param(void);
void cw_set_frequency(void);
void cw_set_amptitude(void);
void phase_setting(void);
/********************************************************************************************/
/********************************************************************************************/


#endif 