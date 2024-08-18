/**********************************************************
                       康威电子
功能：stm32f103rbt6控制MAX262滤波参数
接口：控制引脚接口请参照max262.h
时间：2023/06/XX
版本：2.0
作者：康威电子
其他：本程序只供学习使用，盗版必究。

					max262	单片机
硬件连接:	A0 ———PC0; 
					A1 ———PC1;
					A2 ———PC2;
					A3 ———PC3; 
					D0 ———PC4;   
					D1 ———PC5; 				
					WR=LE——PC6; 
					CLK———PA6
          GND--GND(0V)  

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/ 
**********************************************************/

#include "max262.h"
#include <math.h> 

/***************************************************************    
*名		称：MAX262_Write(uint8_t add,uint8_t dat2bit) 
*功		能：写两比特的数据到某地址 
*入口参数：A3到A0的地址add,
					D1和D0的2bit数据dat2bit 
*出口参数：无 
*说		明：D1和D0取dat2bit的最低两位。仅适用于PDL口的八位写操作。 
***************************************************************/    
void MAX262_Write(uint8_t add, uint8_t dat2bit) {
    uint16_t data;

    // 读取GPIOC的当前输出值
    data = GPIOC->ODR;  // 读取当前GPIOC输出寄存器值

    // 取4位地址
    data = (data & 0xFFF0) | (add & 0x000F);

    // 取2位数据
    data = (data & 0xFFCF) | ((dat2bit << 4) & 0x0030);

    // 写入新数据到GPIOC
    GPIOC->ODR = data;

    // 拉低和拉高控制引脚
    resetWr;
    HAL_Delay(1);
    setWr;
    HAL_Delay(1);
}

/***************************************************************     
* 名		称：Set_Af（） 
* 功		能：设置A通道F值  fCLK/f0
* 入口参数：A通道F值datF 0-63
* 出口参数：无 
* 说		明：无 
***************************************************************/     
void Set_Af(uint8_t datF)    //6位中心频率f0控制位F0-F5 对应十进制0-63
{    
	MAX262_Write(1,datF);         
	datF = datF>>2;         
	MAX262_Write(2,datF);         
	datF = datF>>2;         
	MAX262_Write(3,datF);    
}     
/*************************************************************** 
* 名		称：Set_AQ（）
* 功		能：设置A通道Q值 
* 入口参数：A通道Q值datQ 1-127
* 出口参数：无 
* 说		明：无 
***************************************************************/       
void Set_AQ(uint8_t datQ)    //7位品质因数Q控制位Q0-Q6 对应十进制0-127
{    
	MAX262_Write(4,datQ);         
	datQ = datQ>>2;         
	MAX262_Write(5,datQ);         
	datQ = datQ>>2;         
	MAX262_Write(6,datQ);         
	datQ = (datQ>>2)&1;  //对地址7只取一位         
	MAX262_Write(7,datQ);    
} 
/***************************************************************    
* 名		称：Set_Bf（） 
* 功		能：设置B通道F值 fCLK/f0
* 入口参数：B通道F值datF 0-63
* 出口参数：无 
* 说		明：无 
***************************************************************/     
void Set_Bf(uint8_t datF)    
{    
  MAX262_Write(9,datF);        
	datF = datF>>2;       
	MAX262_Write(10,datF);      
	datF = datF>>2;       
	MAX262_Write(11,datF);    
} 
    
/***************************************************************
* 名		称：Set_BQ（） 
* 功		能：设置B通道Q值 
* 入口参数：B通道Q值datQ 0-127
* 出口参数：无 
* 说		明：无 
***************************************************************/   
void Set_BQ(uint8_t datQ)    
{    
  MAX262_Write(12,datQ);         
	datQ = datQ>>2;        
	MAX262_Write(13,datQ);         
	datQ = datQ>>2;         
	MAX262_Write(14,datQ);    
  datQ = (datQ>>2)&1;
	MAX262_Write(15,datQ);    
}      

/***************************************************************     
* 名		称：FnFin_config(double f0,uint8_t workMode)
* 功		能：计算fn控制字 
* 入口参数：f0：截止频率
						workMode：工作模式 MODE_1到MODE_4
* 出口参数：fn控制字
* 说		明：找到满足时钟频率不超过 4MHz 时，选择最大的fn，
***************************************************************/ 
uint8_t FnFin_config(double f0,uint8_t workMode)
{
	uint8_t i=0,fn=0;
	if(workMode==MODE_2)//模式2
	{
		for(i=0;i<64;i++)
		{
			if(((26+i)*1.11072)*f0>4000000)
			{
				if(i>0)
					fn=i-1;//找到满足时钟频率不超过 4MHz 时，选择最大的fn，
				break;
			}
			else fn=63;
		}
		//fclk/f0 = (26 + fn)*1.11072;
		//时钟频率/截止频率=(26 + 频率控制字)*1.11072;
		//fclk = (26 + fn)*1.11072*f0;
		//满足时钟频率不超过 4MHz 时，选择最大的fn，	
	}
	else//模式134
	{
		for(i=0;i<64;i++)
		{
			if(((26+i)*PI/2)*f0>4000000)
			{
				if(i>0)
					fn=i-1;
				break;
			}
			else fn=63;
		}
		//fclk/f0 = (26 + fn)*PI/2;
		//时钟频率/截止频率=(26 + 频率控制字)*PI/2;
		//fclk = (26 + fn)*PI/2*f0;
		//满足时钟频率不超过 4MHz 时，选择最大的fn，
	}
	return fn;
}

/***************************************************************     
* 名		称：Qn_config(float Q,uint8_t workMode)
* 功		能：计算Qn控制字 
* 入口参数：Q值
					workMode：工作模式 MODE_1到MODE_4
* 出口参数：Qn控制字
* 说		明：
***************************************************************/ 
uint8_t Qn_config(float Q,uint8_t workMode)
{
	uint8_t Qn=0;
	if(workMode==MODE_2)//模式2
	{
		Qn = (uint8_t)(128-90.51/Q);
	}		
	else    //模式134
	{
		Qn = (uint8_t)(128-64/Q);
	}	
	return Qn;
}

/***************************************************************     
* 名		称：lhp_WorkFclk（） 
* 功		能：设置低通和高通滤波器工作并取得时钟频率fclk 
* 入口参数：	f0截止频率，切换模块上的跳线帽可更改低通或高通
						Q品质因数
						workMode:工作模式  //高通只有工作模式3  
						channel:输出通道CH_A,CH_B
* 出口参数：时钟频率fclk `
* 说		明：需要将时钟给出之后才能正常工作；Q越大，频率曲线波动越大。
* 使用说明：模式二时Q值要大于0.707；仅适用于低通和高通。 
***************************************************************/  
float lhp_WorkFclk(float f0,float Q,uint8_t workMode,uint8_t channel)    
{       
	float fclk ;  
	uint8_t Qn, Fn; 
	
	Qn=Qn_config(Q, workMode);
	Fn = FnFin_config(f0, workMode);
	
  if(channel==CH_A)       
	{ 
    MAX262_Write(0,workMode);   //      
		Set_Af(Fn);  //要想获得大范围可调，开关电容的采样频率参数取最大值.         
		Set_AQ(Qn);       
	}     
	else 
  { 
    MAX262_Write(8,workMode);         
		Set_Bf(Fn);     //要想获得大范围可调，开关电容的采样频率参数取最大值.         
		Set_BQ(Qn);       
	}  
  if(workMode==MODE_2)
	{       
		fclk = (26+Fn)*1.11072*f0;	// 求出CLK时钟, 芯片手册p11
	}
	else
	{		
		fclk = (26+Fn)*PI/2*f0;	// 求出CLK时钟, 芯片手册p11
	} 
	return fclk ;
}


/***************************************************************     
* 名称：bp_WorkFclk（） 
* 功能：设置带通滤波器工作并取得时钟频率fclk ，模块上的跳线帽请切换到带通
* 入口参数：上限截止频率fh、下限截止频率fl、工作模式workMode和输出通道channel 
* 出口参数：时钟频率fclk 
* 说明：需要将时钟给出之后才能正常工作。 
* 使用说明：设置带通频率时需满足以下条件：
						中心频率÷滤波器带宽>0.707 
***************************************************************/ 
float bp_WorkFclk(float fh,float fl,uint8_t workMode,uint8_t channel)    
{   
  float f0,Q,fclk;    
	uint8_t Qn, Fn; 
	
	f0 = sqrt(fh*fl);   
	Q = f0/(fh-fl);   //计算q值 参考芯片手册p18
 
	Qn=Qn_config(Q, workMode);
	
	Fn = FnFin_config(f0, workMode);
	
	if(channel==CH_A) 
	{
		MAX262_Write(0,workMode);         
		Set_Af(Fn);  //要想获得大范围可调，开关电容的采样频率参数取最大值. 
		Set_AQ(Qn); //
	}     
	else       
	{ 
    MAX262_Write(8,workMode);         
		Set_Bf(Fn);     //要想获得大范围可调，开关电容的采样频率参数取最大值.         
		Set_BQ(Qn);       
	}   
  if(workMode==MODE_2)
	{       
		fclk = (26+Fn)*1.11072*f0;	// 求出CLK时钟, 芯片手册p11
	}
	else
	{		
		fclk = (26+Fn)*PI/2*f0;	// 求出CLK时钟, 芯片手册p11
	} 
	return fclk ;
}


/***************************************************************     
* 名称：bs_WorkFclk（） 
* 功能：设置陷波滤波器工作并取得时钟频率fclk ，模块上的跳线帽请切换到带通
* 入口参数：陷波频率fn、品质因数Q、工作模式workMode和输出通道channel 
* 出口参数：时钟频率fclk 
* 说明：需要将时钟给出之后才能正常工作；Q越大，频率曲线波动越大；仅适用于陷波；模式二时Q值要大于0.707。 
* 使用说明：只有模式1才能正常陷波fn=f0;模式2只能陷下去fn=0.725f0。
***************************************************************/ 
float bs_WorkFclk(float f0,float Q,uint8_t workMode,uint8_t channel) 
{ 
	uint8_t Qn,Fn;
	float fclk;
	
	Qn=Qn_config(Q, workMode);	
	Fn = FnFin_config(f0, workMode);
	
	if(channel==CH_A)		
	{ 
    MAX262_Write(0,workMode);         
		Set_Af(Fn);  //要想获得大范围可调，开关电容的采样频率参数取最大值.         
		Set_AQ(Qn);       
	}     
	else      
	{ 
		MAX262_Write(8,workMode);         
		Set_Bf(Fn);     //要想获得大范围可调，开关电容的采样频率参数取最大值.         
		Set_BQ(Qn);       
	}   
  if(workMode==MODE_2)
	{       
		fclk = (26+Fn)*1.11072*f0;	// 求出CLK时钟, 芯片手册p11
	}
	else
	{		
		fclk = (26+Fn)*PI/2*f0;	// 求出CLK时钟, 芯片手册p11
	} 
	return fclk ;
}

/***************************************************************     
* 名称：ap_WorkFclk（） 
* 功能：设置全通滤波器工作并取得时钟频率fclk ，模块上的跳线帽请切换到带通
* 入口参数：中心频率f0、品质因数Q和输出通道channel 
* 出口参数：时钟频率fclk 
* 说明：需要将时钟给出之后才能正常工作；仅适用于全通。 
* 使用说明：Q值不能太大，否则f0附近有起伏。 
***************************************************************/
float ap_WorkFclk(float f0,float Q,uint8_t channel) 
{    
	uint8_t Qn,Fn; 
  Qn = (uint8_t)(128-64/Q);   
  Fn = FnFin_config(f0, MODE_4);

	if(channel==CH_A) 
	{ 
		MAX262_Write(0,MODE_4);         
		Set_Af(Fn);  //要想获得大范围可调，开关电容的采样频率参数取最大值.         
		Set_AQ(Qn);       
	}     
	else 
	{ 
		MAX262_Write(8,MODE_4);         
		Set_Bf(Fn);     //要想获得大范围可调，开关电容的采样频率参数取最大值.         
		Set_BQ(Qn);       
	}   
	return  (26+Fn)*PI/2*f0; 
} 
	














