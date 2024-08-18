#include "screen.h"
uint8_t AM_Flag=1;//1代表AM 模式，2代表CW模式
uint8_t Rx_data[2];
uint8_t RxProc_Flag=1;//接收数据处理完成标志，0代表未处理，1代表已处理
/**
  * @Brief	串口重定向
  * @Param	None
  * @Retval	None
  * @Note		勾选微库
  * @Author	Katrina_Lau
**/
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}
/**
  * @Brief	USART1接收中断回调函数
  * @Param	None
  * @Retval	None
  * @Note		
  * @Author	Katrina_Lau
**/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if((huart->Instance==USART1)&&(RxProc_Flag))//接收到信号，且数据处理完成
	{
		//HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_0);
		HAL_UART_Receive_IT(&huart1,Rx_data,2); 
		RxProc_Flag=0;
	}
}
/**
  * @Brief	接收到的数据处理
  * @Param	None
  * @Retval	None
  * @Note		None
  * @Author	Katrina_Lau
**/
void UART1_Rx_Proc(void)
{
	if(!RxProc_Flag)
	{
		//HAL_GPIO_TogglePin(GPIOD,GPIO_PIN_1);
		RxProc_Flag=1;
		switch(Rx_data[0])
		{
			case 0x00://Sm
			{
				switch(Rx_data[1])
				{
					case 0xb0://rms+100,100~1000
					{
						if(Signal.cw_rms+100<=1000) Signal.cw_rms+=100;
						else Signal.cw_rms=1000;
						printf("page0.n0.val=%d\xff\xff\xff",Signal.cw_rms);
						//printf("%d",Signal.rms);
					}break;
					case 0xb1://rms-100
					{
						if(Signal.cw_rms-100>=100) Signal.cw_rms-=100;
						else Signal.cw_rms=100;
						printf("page0.n0.val=%d\xff\xff\xff",Signal.cw_rms);
						//printf("%d",Signal.rms);
					
					}break;
				/*----------------------*/
					
					case 0xb2://Ma+10,30~90
					{	
						if(AM_Flag)//保证在CW模式下调不了调制度
						{
							if(Signal.Ma+10<=90) Signal.Ma+=10;
							else Signal.Ma=90;
							printf("page0.n1.val=%d\xff\xff\xff",Signal.Ma);
							//printf("%d",Signal.Ma);
						}
					}break;
					case 0xb3://Ma-10
					{
						if(AM_Flag)//保证在CW模式下调不了调制度
						{
							if(Signal.Ma-10>=30) Signal.Ma-=10;
							else Signal.Ma=30;
							printf("page0.n1.val=%d\xff\xff\xff",Signal.Ma);
						//printf("%d",Signal.Ma);
						}
					}break;
		
				/*---------------------*/
					case 0xb4://fc+1000000,30M~40M
					{
						if(Signal.fc+1000000<=40000000) Signal.fc+=1000000;
						else Signal.fc=40000000;
						printf("page0.n2.val=%d\xff\xff\xff",Signal.fc/1000000);
						//printf("%d",Signal.fc);
					}break;
					case 0xb5://fc-1000000
					{
						if(Signal.fc-1000000>=30000000) Signal.fc-=1000000;
						else Signal.fc=30000000;
						printf("page0.n2.val=%d\xff\xff\xff",Signal.fc/1000000);
						//printf("%d",Signal.fc);
					}break;
				/*---------------------*/
					case 0xb6://AM
					{
						if(Signal.Ma==0) //处于CW模式下才进行修改
						{
							Signal.Ma=90;
							AM_Flag=1;
							printf("page0.n1.val=%d\xff\xff\xff",Signal.Ma);
							printf("page0.t2.txt=\"AM\"\xff\xff\xff");
						}
					}break;
					case 0xb7://CW
					{
						if(Signal.Ma>0) //处于AM模式下才进行修改
						{
							Signal.Ma=0;
							AM_Flag=0;
							printf("page0.n1.val=%d\xff\xff\xff",Signal.Ma);
							printf("page0.t2.txt=\"CW\"\xff\xff\xff");
						}
//						printf("CW");
//						printf("%d",Signal.Ma);
					}break;
					/*--------------------*/
					default:break;
				}break;
			}
			case 0x01://Sd
			{
				switch(Rx_data[1])
				{
					case 0xb0://dB+2,0~20
					{
						if(Signal.reduction_dB+2<=20) Signal.reduction_dB+=2;
						else Signal.reduction_dB=20;
						printf("page1.n0.val=%d\xff\xff\xff",Signal.reduction_dB);
						//printf("%d",Signal.dB);
					}break;
					case 0xb1://dB-2
					{
						if(Signal.reduction_dB-2>=0) Signal.reduction_dB-=2;
						else Signal.reduction_dB=0;
						printf("page1.n0.val=%d\xff\xff\xff",Signal.reduction_dB);
						//printf("%d",Signal.dB);
					}break;
				/*----------------------*/
					case 0xb2://ns+30,50~200
					{
						if(Signal.delay_ns==0)	Signal.delay_ns+=50;
						else if(Signal.delay_ns+30<=200) Signal.delay_ns+=30;
						else if(Signal.delay_ns+30>200)Signal.delay_ns=200;	
						printf("page1.n1.val=%d\xff\xff\xff",Signal.delay_ns);
						//printf("%d",Signal.ns);
					}break;
					case 0xb3://ns-30
					{
						if(Signal.delay_ns-30>=50) Signal.delay_ns-=30;
						else Signal.delay_ns=0;
						printf("page1.n1.val=%d\xff\xff\xff",Signal.delay_ns);
						//printf("%d",Signal.ns);
					}break;
				/*---------------------*/
					case 0xb4://phase+30,0~180
					{
						if(Signal.phase_difference+30<=180) Signal.phase_difference+=30;
						else Signal.phase_difference=180;
						printf("page1.n2.val=%d\xff\xff\xff",Signal.phase_difference);
						//printf("%d",Signal.phase);
					}break;
					case 0xb5://phase-30
					{
						if(Signal.phase_difference-30>=0) Signal.phase_difference-=30;
						else Signal.phase_difference=0;
						printf("page1.n2.val=%d\xff\xff\xff",Signal.phase_difference);
						//printf("%d",Signal.phase);
					}break;
					/*---------------------*/
					default:break;
				}break;
			}

			default:break;
		}
		signal_setting();
	}

}