#include "AD9854.h"


#define CLK_Set 9
const u32  Freq_mult_ulong  = 1042500;		 
const double Freq_mult_doulle = 1042499.9137431;

u8 FreqWord[6];              //6���ֽ�Ƶ�ʿ�����
uint16_t add = 0;
uint16_t step_up = 0;
uint32_t fund_fre_buf;
	
uint16_t amp_buf;
uint8_t modu_buf;

/**
   * @Brief	us��ʱ����
   * @Param  None
   * @Retval None
   * @Note   None
   */
void delay_1us(uint32_t us) 
{
    uint32_t startval, tickn, delays, wait;
    startval = SysTick->VAL;
    tickn = HAL_GetTick();
    delays = us * 72;
    if (delays > startval) {
        while (HAL_GetTick() == tickn) {
        }
        wait = 72000 + startval - delays;
        while (wait < SysTick->VAL) {
        }
    } else {
        wait = startval - delays;
        while (wait < SysTick->VAL && HAL_GetTick() == tickn) {
        }
    }
}
void delay_us(uint16_t us)
{
    uint32_t delay = us * (SystemCoreClock / 1000000) / 5;
    while (delay--)
    {
        __NOP();
    }
}
void AD9854_gpio_init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
	
	// ʹ��GPIOB,GPIOA,GPIOE,GPIOC
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();	
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();							
		
	
	GPIO_InitStructure.Pin		=		GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStructure.Mode	=	 	GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull =   GPIO_NOPULL;
  GPIO_InitStructure.Speed = 	GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin		=		GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
	GPIO_InitStructure.Mode	=	 	GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = 	GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed =GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin		=		GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10;
	GPIO_InitStructure.Mode	=	 	GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = 	GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);
}
//====================================================================================
// AD9854������
//====================================================================================
//====================================================================================
//��������:void AD9854_WR_Byte(uchar addr,uchar dat)
//��������:AD9854���п�д������
//��ڲ���:addr     6λ��ַ
//         dat      д�������														
//���ڲ���:��
//====================================================================================
void AD9854_WR_Byte(u8 addr,u8 dat)
{
	u16 Tmp;
	
	Tmp =  GPIOE->ODR;
	GPIOE->ODR =  ((u16)(addr&0x3f) | (Tmp & 0xFFC0));	// ADDER  PE0~PE5
	Tmp =  GPIOF->ODR;
	GPIOF->ODR=((u16)(dat) | (Tmp & 0xFF00));		  // DATA PF0~PF7
	AD9854_WR_Clr;
	AD9854_WR_Set;
}
//====================================================================================
//��������:void Freq_convert(long Freq)
//��������:�����ź�Ƶ������ת��
//��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
//���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
//˵����   ���㷨λ���ֽ�����㷨���й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK
//         �õ����㷨������N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
//         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
//====================================================================================
void Fre_convert(uint32_t Freq)   
{	
  u32 FreqBuf;
  u32 Temp=Freq_mult_ulong;   	       

	u8 Array_Freq[4];			     //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�
	Array_Freq[0]=(u8) Freq;
	Array_Freq[1]=(u8)(Freq>>8);
	Array_Freq[2]=(u8)(Freq>>16);
	Array_Freq[3]=(u8)(Freq>>24);

	FreqBuf=Temp*Array_Freq[0];                  
	FreqWord[0]=FreqBuf;    
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[1]);
	FreqWord[1]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[2]);
	FreqWord[2]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[3]);
	FreqWord[3]=FreqBuf;
	FreqBuf>>=8;

	FreqWord[4]=FreqBuf;
	FreqWord[5]=FreqBuf>>8;	
}  

//====================================================================================
//��������:void AD9854_Init(void)
//��������:AD9854��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_Init(void)
{
	uint32_t i;
	AD9854_WR_Set;                 // ������д���ƶ˿���Ϊ��Ч
	AD9854_RD_Set;
	AD9854_SP_Set;
	delay_us(10);
	AD9854_UDCLK_Clr;
 	AD9854_RST_Set;                // ��λAD9854
	AD9854_RST_Clr;
	delay_us(10);
	AD9854_RST_Set;
	for(i=0;i<1000;i++);
	AD9854_RST_Clr;

 	AD9854_WR_Byte(0x1d,0x00);	 	 // �����Ƚ���, ������ſ����������
//	AD9854_WR_Byte(0x1d,0x10);	 // �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	 // ����ϵͳʱ�ӱ�Ƶ            
	AD9854_WR_Byte(0x1f,0x01);	   // ����ϵͳΪģʽ0�����ڲ�����
	AD9854_WR_Byte(0x20,0x60);	   // ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,�ر�SincЧӦ


}
//====================================================================================
//��������:void AD9854_SetSine(ulong Freq,uint Shape)
//��������:AD9854���Ҳ���������
//��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~(1/2)*SYSCLK
//         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) ,ȡֵԽ��,����Խ�� 
//���ڲ���:��
//====================================================================================
void AD9854_SetSine(u32 Freq,u16 Shape)
{
	u8 count;
	u8 Adress;

	Adress = 0x04;                      //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

	Fre_convert(Freq);		              //Ƶ��ת��

	for(count=6;count>0;)	              //д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	
	AD9854_WR_Byte(0x21,(Shape>>8) & 0x00FF);		// ����Iͨ������
	AD9854_WR_Byte(0x22,(Shape&0x00ff));
	
	AD9854_WR_Byte(0x23,(Shape>>8) & 0x00FF);   // ����Qͨ������
	AD9854_WR_Byte(0x24,(Shape&0x00ff));
	
	
	AD9854_UDCLK_Set;                   				// ����AD9854���
	AD9854_UDCLK_Clr;

}
////====================================================================================
////��������:void Freq_doublt_convert(double Freq)
////��������:�����ź�Ƶ������ת��
////��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
////���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
////˵����   �й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK�õ��ú�����
////         ����N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
////         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
////ע�⣺   �ú��������溯��������Ϊ�ú�������ڲ���Ϊdouble����ʹ�źŵ�Ƶ�ʸ���ȷ
////         ���꽨����100HZ�����ñ��������ڸ���100HZ��������ú���void Freq_convert(long Freq)
////====================================================================================
//void Freq_double_convert(double Freq)   
//{
//	u32 Low32;
//	u32 High16;
//  double Temp=Freq_mult_doulle;   	            //23ca99Ϊ2��48�η�����120M
//	Freq*=(double)(Temp);
////	1 0000 0000 0000 0000 0000 0000 0000 0000 = 4294967295
//	High16 = (int)(Freq/4294967295);                  //2^32 = 4294967295
//	Freq -= (double)High16*4294967295;
//	Low32 = (u32)Freq;

//	FreqWord[0]=Low32;	     
//	FreqWord[1]=Low32>>8;
//	FreqWord[2]=Low32>>16;
//	FreqWord[3]=Low32>>24;
//	FreqWord[4]=High16;
//	FreqWord[5]=High16>>8;			
//} 

////====================================================================================
////��������:void AD9854_SetSine_double(double Freq,uint Shape)
////��������:AD9854���Ҳ���������
////��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~1/2*SYSCLK
////         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) 
////���ڲ���:��
////====================================================================================
//void AD9854_SetSine_double(double Freq,u16 Shape)
//{
//	u8 count=0;
//	u8 Adress;

//	Adress=0x04;						     //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

//	Freq_double_convert(Freq);		           //Ƶ��ת��
//	 
//	for(count=6;count>0;)	                    //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		AD9854_WR_Byte(Adress++,FreqWord[--count]);
//  }
//	
//	AD9854_WR_Byte(0x21,Shape>>8);	  //����Iͨ������
//	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
//	
//	AD9854_WR_Byte(0x23,Shape>>8);	  //����Qͨ������
//	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

//	AD9854_UDCLK_Set;                    //����AD9854���
//  AD9854_UDCLK_Clr;
//}

//====================================================================================
//��������:void AD9854_InitFSK(void)
//��������:AD9854��FSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitFSK(void)
{

	AD9854_WR_Byte(0x1d,0x10);	     	// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   	// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x02);	     	// ����ϵͳΪģʽ1�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	    	// ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����


}

//====================================================================================
//��������:void AD9854_SetFSK(ulong Freq1,ulong Freq2)
//��������:AD9854��FSK����
//��ڲ���:Freq1   FSKƵ��1   
//         Freq2   FSKƵ��2
//���ڲ���:��
//====================================================================================
void AD9854_SetFSK(u32 Freq1,u32 Freq2)
{
  u8 count=6;
	u8 Adress1,Adress2;

	const u16 Shape=4000;						// ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)
	
	Adress1=0x04;				 // ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ
	Adress2=0x0a;				 // ѡ��Ƶ�ʿ�����2��ַ�ĳ�ֵ
	
	Fre_convert(Freq1);           	// Ƶ��ת��1
	
	for(count=6;count>0;)	          // д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
  }
	
	Fre_convert(Freq2);						// Ƶ��ת��2

	for(count=6;count>0;)	          // д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress2++,FreqWord[--count]);
  }

	AD9854_WR_Byte(0x21,Shape>>8);	     // ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	     // ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                    // ����AD9854���
  AD9854_UDCLK_Clr;		
}

//====================================================================================
//��������:void AD9854_InitBPSK(void)
//��������:AD9854��BPSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitBPSK(void)
{
	
	AD9854_WR_Byte(0x1d,0x10);				// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   	// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x08);	      // ����ϵͳΪģʽ4�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	      // ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����


}

//====================================================================================
//��������:void AD9854_SetBPSK(uint Phase1,uint Phase2)
//��������:AD9854��BPSK����
//��ڲ���:Phase1   ������λ1
//         Phase2	������λ2
//���ڲ���:��
//˵����   ��λΪ14Bit��ȡֵ��0~16383�����꽨�����ñ�������ʱ��Phase1����Ϊ0��
//         ��Phase1����Ϊ8192��180����λ
//====================================================================================
void AD9854_SetBPSK(u16 Phase1,u16 Phase2)
{
	u8 count;

	const u32 Freq=60000;
  const u16 Shape=4000;

	u8 Adress;
	Adress=0x04;                           		// ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

	AD9854_WR_Byte(0x00,Phase1>>8);	       		// ������λ1
	AD9854_WR_Byte(0x01,(u8)(Phase1&0xff));
	
	AD9854_WR_Byte(0x02,Phase2>>8);	          // ������λ2
	AD9854_WR_Byte(0x03,(u8)(Phase2&0xff));

	Fre_convert(Freq);                     	// Ƶ��ת��

	for(count=6;count>0;)	                 		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }

	AD9854_WR_Byte(0x21,Shape>>8);	        	// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	        	// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                      		// ����AD9854���
  AD9854_UDCLK_Clr;	
}

//====================================================================================
//��������:void AD9854_InitOSK(void)
//��������:AD9854��OSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitOSK(void)
{																				

  AD9854_WR_Byte(0x1d,0x10);	       			// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   				// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x00);	      			// ����ϵͳΪģʽ0�����ⲿ����
	AD9854_WR_Byte(0x20,0x70);	      			// ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,ͨ�������ڲ�����


}

//====================================================================================
//��������:void AD9854_SetOSK(uchar RateShape)
//��������:AD9854��OSK����
//��ڲ���: RateShape    OSKб��,ȡֵΪ4~255��С��4����Ч
//���ڲ���:��
//====================================================================================
void AD9854_SetOSK(u8 RateShape)
{
	u8 count;

	const u32 Freq=60000;				// ������Ƶ
  const u16  Shape=4000;			// ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	u8 Adress;
	Adress=0x04;            		// ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

	Fre_convert(Freq);                       		// Ƶ��ת��

	for(count=6;count>0;)	                    		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }

	AD9854_WR_Byte(0x21,Shape>>8);	            	// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	            	// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));  	 


  AD9854_WR_Byte(0x25,RateShape);				    		// ����OSKб��

	AD9854_UDCLK_Set;                         		// ����AD9854���
  AD9854_UDCLK_Clr;	
}

//====================================================================================
//��������:void AD9854_InitAM(void)
//��������:AD9854��AM��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitAM(void)
{
	
	
	AD9854_WR_Byte(0x1d,0x10);	          			// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	             	// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x01);	              	// ����ϵͳΪģʽ0�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	             		// ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����


}

//====================================================================================
//��������:void AD9854_SetAM(uchar Shape)
//��������:AD9854��AM����
//��ڲ���:Shape   12Bit����,ȡֵ��70~4095   
//���ڲ���:��
//====================================================================================
void AD9854_SetAM(u32 Freq,u16 Shape)
{
	u8 count;

//	 u32 Freq=60000;			 //������Ƶ

	u8  Adress;
	Adress=0x04;      				// ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ
	
		Fre_convert(Freq);                       		// Ƶ��ת��

	for(count=6;count>0;)	                      		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }
	
	AD9854_WR_Byte(0x21,Shape>>8);	             		// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	              	// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                              	// ����AD9854���
  AD9854_UDCLK_Clr;			
}
//====================================================================================
//��������:void am_test (uint32_t fund_fre, uint8_t modulation)
//��������:AD9854��AM���εĵ���Ƶ�ʺ͵��ƶȵ�����
//��ڲ���:fund_fre   ����Ƶ�� ��0~100��
//				 modulation ���ƶ�   ��0~100��
//���ڲ���:��
//====================================================================================
void am_test (uint32_t fund_fre, uint8_t modulation)
{
	modu_buf = modulation;
	fund_fre_buf = fund_fre;	
	step_up = (float)fund_fre_buf * 65535 / 10000;
}


//====================================================================================
//��������:void AD9854_InitRFSK(void)
//��������:AD9854��RFSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitRFSK(void)
{


	AD9854_WR_Byte(0x1d,0x10);	        // �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	      // ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x24);	        // ����ϵͳΪģʽ2�����ⲿ����,ʹ�����ǲ�ɨƵ����
	AD9854_WR_Byte(0x20,0x60);	        // ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,�ر�SincЧӦ	

}

//====================================================================================
//��������:void AD9854_SetRFSK(u32 Freq_Low,u32 Freq_High,u32 Freq_Up_Down,u32 FreRate)
//��������:AD9854��RFSK����
//��ڲ���:Freq_Low          RFSK��Ƶ��	   48Bit
//         Freq_High         RFSK��Ƶ��	   48Bit
//         Freq_Up_Down		 ����Ƶ��	   48Bit
//		     FreRate           б��ʱ�ӿ���  20Bit
//���ڲ���:��
//ע��     ÿ��������֮���ʱ����������ʽ��ʾ��FreRate +1��*��System Clock ����һ������,
//         Ƶ�� ���������½� һ������Ƶ��
//====================================================================================
void AD9854_SetRFSK(u32 Freq_Low,u32 Freq_High,u32 Freq_Up_Down,u32 FreRate)
{
	u8 count=6;
	u8 Adress1,Adress2,Adress3;
  const u16  Shape=3600;			  // ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	Adress1=0x04;		     					// ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ 
	Adress2=0x0a;
	Adress3=0x10;

	Fre_convert(Freq_Low);                   		// Ƶ��1ת��

	for(count=6;count>0;)	                    		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
  }

	Fre_convert(Freq_High);               				// Ƶ��2ת��

	for(count=6;count>0;)	                     		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress2++,FreqWord[--count]);
  }

	Fre_convert(Freq_Up_Down);          					// ����Ƶ��ת��

	for(count=6;count>0;)	                				// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress3++,FreqWord[--count]);
  }

	AD9854_WR_Byte(0x1a,(u8)((FreRate>>16)&0x0f));// ����б������
	AD9854_WR_Byte(0x1b,(u8)(FreRate>>8));
	AD9854_WR_Byte(0x1c,(u8)FreRate);				    

	AD9854_WR_Byte(0x21,Shape>>8);	        			// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	         			// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                           	// ����AD9854���
  AD9854_UDCLK_Clr;	
}
//====================================================================================
//��������:void  AD9854_InitChirp(void)
//��������:AD9854��Chirp��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitChirp(void)
{	
	AD9854_WR_Byte(0x1d,0x10);	  			// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   		// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x26);	        // ����ϵͳΪģʽ2�����ⲿ����,ʹ�����ǲ�ɨƵ����
	AD9854_WR_Byte(0x20,0x60);	        // ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����	
}

//====================================================================================
//��������:void AD9854_SetChirp(unsigned long Freq_Low,unsigned long Freq_Up_Down,unsigned long FreRate)
//��������:AD9854��Chirp����
//��ڲ���:Freq_Low          RFSK��Ƶ��	   48Bit
//         Freq_Up_Down		 ����Ƶ��	   48Bit
//		   FreRate           б��ʱ�ӿ���  20Bit
//���ڲ���:��
//ע��     ÿ��������֮���ʱ����������ʽ��ʾ��FreRate +1��*��System Clock ����һ������,
//         Ƶ�� ���������½� һ������Ƶ��
//====================================================================================
void AD9854_SetChirp(unsigned long Freq_Low,unsigned long Freq_Up_Down,unsigned long FreRate)
{
	u8 count=6;
	u8 Adress1,/*Adress2,*/Adress3;
  const u16  Shape=4000;			   // ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	Adress1=0x04;		               // ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ 

	Adress3=0x10;

	Fre_convert(Freq_Low);                      		// Ƶ��1ת��

	for(count=6;count>0;)	                         	// д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
	}

	Fre_convert(Freq_Up_Down);            					// ����Ƶ��ת��

	for(count=6;count>0;)	                      		// д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress3++,FreqWord[--count]);
	}

	AD9854_WR_Byte(0x1a,(u8)((FreRate>>16)&0x0f));	// ����б������
	AD9854_WR_Byte(0x1b,(u8)(FreRate>>8));
	AD9854_WR_Byte(0x1c,(u8)FreRate);				    

	AD9854_WR_Byte(0x21,Shape>>8);	               	// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	              	// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                            		// ����AD9854���
  AD9854_UDCLK_Clr;
}