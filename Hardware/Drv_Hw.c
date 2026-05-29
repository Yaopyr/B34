#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
u32 hw_jsm;
u8 hw_jsbz;
int8_t speed1=0;
int8_t speed2=0;
int8_t speed3=0;
int8_t speed4=0;

int8_t speedtset;
void Drv_Hw_Init (void)
{
	// 结构体定义
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	// 配置GPIO结构体
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;   // 红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;   // 上拉输入模式
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);   // 选择GPIO管脚用作外部中断线路
	EXTI_ClearITPendingBit(EXTI_Line12);
	
	// 配置外部中断
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure); 

	// 配置NVIC结构体
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // 抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   // 响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   // 使能
	NVIC_Init(&NVIC_InitStructure);
}

u8 Drv_Hw_RecordHightTime (void)
{
	u8 t = 0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 1)   // 高电平
	{
		t ++;
		Delay_us(20);
		if(t >= 250)
		{
			return t;   // 超时溢出
		}
	}
	return t;
}

void EXTI15_10_IRQHandler(void)   // 红外遥控外部中断
{
	u8 Tim = 0,Ok = 0,Data,Num = 0;

   while(1)
   {
	   	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 1)
		{
			 Tim = Drv_Hw_RecordHightTime();   //获得此次高电平时间

			 if(Tim>=250)
			 {
				 break;   // 不是有用的信号
			 }

			 if(Tim >= 200 && Tim < 250)
			 {
			 	Ok = 1;   // 收到起始信号
			 }
			 else if(Tim >= 60 && Tim < 90)
			 {
			 	Data = 1;   // 收到数据 1
			 }
			 else if(Tim >= 10 && Tim < 50)
			 {
			 	Data = 0;   // 收到数据 0
			 }
			 
			 if(Ok == 1)
			 {
			 	hw_jsm <<= 1;
				hw_jsm += Data;

				 // 接收完成
				if(Num >= 32)
				{
					hw_jsbz = 1;
				  	break;
				}
			 }
			 Num ++;
		}
   }
	 
	 // 清除中断标志位
   EXTI_ClearITPendingBit(EXTI_Line12);	
}

u8 Med_Hw_ReadKeyValue (void)
{
	if(hw_jsm == 0x00FF9867)
	{
		return 0;
	}
	else if(hw_jsm == 0x00FFA25D)
	{
		return 1;
	}
	else if(hw_jsm == 0x00FF629D)
	{
		return 2;
	}
	else if(hw_jsm == 0x00FFE21D)
	{
		return 3;
	}
	else if(hw_jsm == 0x00FF22DD)
	{
		return 4;
	}
	else if(hw_jsm == 0x00FF02FD)
	{
		return 5;
	}
	else if(hw_jsm == 0x00FFC23D)
	{
		return 6;
	}
	else if(hw_jsm == 0x00FFE01F)//7
	{
		if(speed1>=0)//speed1
		{
			speed1-=10;
			if(speed1<0)
			{
				speed1=0;
			}
		}
		else if(speed1<0)
		{
			speed1+=10;
			if(speed1>0)
			{
				speed1=0;
			}
		}
		if(speed2>=0)//speed2
		{
			speed2-=10;
			if(speed2<0)
			{
				speed2=0;
			}
		}
		else if(speed2<0)
		{
			speed2+=10;
			if(speed2>0)
			{
				speed2=0;
			}
		}
		if(speed3>=0)//speed3
		{
			speed3-=10;
			if(speed3<0)
			{
				speed3=0;
			}
		}
		else if(speed3<0)
		{
			speed3+=10;
			if(speed3>0)
			{
				speed3=0;
			}
		}
		if(speed4>=0)//speed4
		{
			speed4-=10;
			if(speed4<0)
			{
				speed4=0;
			}
		}
		else if(speed4<0)
		{
			speed4+=10;
			if(speed4>0)
			{
				speed4=0;
			}
		}

		return 7;
	}
	else if(hw_jsm == 0x00FFA857)
	{
		return 8;
	}
	else if(hw_jsm == 0x00FF906F)//9
	{
		if(speed1>=0)//speed1
		{
			speed1+=10;
			if(speed1>100)
			{
				speed1=100;
			}
		}
		else if(speed1<0)
		{
			speed1-=10;
			if(speed1<-100)
			{
				speed1=-100;
			}
		}
		if(speed2>=0)//speed2
		{
			speed2+=10;
			if(speed2>100)
			{
				speed2=100;
			}
		}
		else if(speed2<0)
		{
			speed2-=10;
			if(speed2<-100)
			{
				speed2=-100;
			}
		}
		if(speed3>=0)//speed3
		{
			speed3+=10;
			if(speed3>100)
			{
				speed3=100;
			}
		}
		else if(speed3<0)
		{
			speed3-=10;
			if(speed3<-100)
			{
				speed3=-100;
			}
		}
		if(speed4>=0)//speed4
		{
			speed4+=10;
			if(speed4>100)
			{
				speed4=100;
			}
		}
		else if(speed4<0)
		{
			speed4-=10;
			if(speed4<-100)
			{
				speed4=-100;
			}
		}


		return 9;
	}
	else if(hw_jsm == 0x00FF6897)   // 按键*按下
	{
		speed1=speed2=speed3=speed4=0;
		return 10;
	}
	else if(hw_jsm == 0x00FFB04F)   // 按键#按下
	{
		return 11;
	}
	else if(hw_jsm == 0x00FF38C7)   // 按键OK按下
	{
		speed1=speed2=50;
		speed3=speed4=-50;

		return 12;
	}
	
	else if(hw_jsm == 0x00FF18E7)   // 按键"上"按下
	{
		speed1=speed2=speed3=speed4=50;
		return 13;
	}
	else if(hw_jsm == 0x00FF4AB5)   // 按键"下"按下
	{
		speed1=speed2=speed3=speed4=-50;

		return 14;
	}
	else if(hw_jsm == 0x00FF10EF)   // 按键"左"按下
	{
		speed1=speed3=50;
		speed2=speed4=-50;
		return 15;
	}
	else if(hw_jsm == 0x00FF5AA5)   // 按键"右"按下
	{
		speed1=speed3=-50;speed2=speed4=50;
		return 16;
	}
	return 17;
}
