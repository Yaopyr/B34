#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Servo.h"
#include "Drv_Hw.h"
#include "Motor.h"
#include "Irobstacle.h"

float Angle1=90,Angle2=45,Angle3=120,Angle4=0;			//定义角度变量
uint8_t i;
extern int8_t speed1;
extern int8_t speed2;
extern int8_t speed3;
extern int8_t speed4;

int main(void)

{
	/*模块初始化*/
	Servo_Init();		//舵机初始化
  Irobstacle_Init();   // 红外避障传感器初始化
	Drv_Hw_Init();
	Motor_Init();
	

	Servo_SetAngle(1,90);
	Servo_SetAngle(2,45);
	Servo_SetAngle(3,120);
	Servo_SetAngle(4,0);



	while (1)
	{
			Motor_SetSpeed1(speed1);
			Motor_SetSpeed2(speed2);
			Motor_SetSpeed3(speed3);
			Motor_SetSpeed4(speed4);

//舵机1
		if (Angle1 > 180)
			{
				Servo_SetAngle(1,180);
			}
		if(Angle1<0)
			{
				Servo_SetAngle(1,0);
			}
//舵机2
		if (Angle2 > 180)
			{
				Servo_SetAngle(2,180);
			}
		if(Angle2<30)
			{
				Servo_SetAngle(2,30);
			}
//舵机3
		if (Angle3 > 180)
			{
				Servo_SetAngle(3,180);
			}
		if(Angle3<120)
			{
				Servo_SetAngle(3,120);
			}
//舵机4
		if (Angle4 > 30)
			{
				Servo_SetAngle(4,30);
			}
		if(Angle4<0)
			{
				Servo_SetAngle(4,0);
			}

//红外控制
if (hw_jsbz == 1) 
{
	u8 Key = Med_Hw_ReadKeyValue();
	hw_jsbz = 0;
//舵机1
		if (Key == 1)
		{
			Angle1 += 15;
			Servo_SetAngle(1,Angle1);

		}
		if (Key == 4)
		{
			Angle1-=15;
			Servo_SetAngle(1,Angle1);

		}


//舵机2
		if (Key == 2)
		{
			Angle2 += 15;
			Servo_SetAngle(2,Angle2);

		}
		if (Key== 5)
		{
			Angle2 -=15;
			Servo_SetAngle(2,Angle2);
		}


//舵机3

		if (Key == 3)
		{
			Angle3 += 15;
			Servo_SetAngle(3,Angle3);

		}
		if (Key == 6)
		{
			Angle3-=15;
			Servo_SetAngle(3,Angle3);

		}


//舵机4

		if (Key == 8)
		{
			Angle4 += 15;
			Servo_SetAngle(4,Angle4);

		}
		if (Key == 0)
		{
			Angle4 -=15;
			Servo_SetAngle(4,Angle4);

		}

      if(Key==11)//自动巡游
			{
					while( Med_Hw_ReadKeyValue() !=10)
					{
					
							Motor_SetSpeed1(40);
							Motor_SetSpeed2(40);
							Motor_SetSpeed3(40);
							Motor_SetSpeed4(40);
						
							while(Left_Irobstacle_Get() == 1 && Right_Irobstacle_Get() == 0) //右边检测到红外信号
							{

								Motor_SetSpeed1(-50);
								Motor_SetSpeed2(-50);
								Motor_SetSpeed3(50);
								Motor_SetSpeed4(50);
							}
							while(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 1) // 左边检测到红外信号
							{

								Motor_SetSpeed1(50);
								Motor_SetSpeed2(50);
								Motor_SetSpeed3(-50);
								Motor_SetSpeed4(-50);
							}			
							while(Left_Irobstacle_Get() == 0 && Right_Irobstacle_Get() == 0) // 两边传感器同时检测到红外
							{

								Motor_SetSpeed1(-50);
								Motor_SetSpeed2(-50);
								Motor_SetSpeed3(50);
								Motor_SetSpeed4(50);

								Delay_ms(1500);

								Motor_SetSpeed1(40);
								Motor_SetSpeed2(40);
								Motor_SetSpeed3(40);
								Motor_SetSpeed4(40);

							}
             
					}
						speed1=speed2=speed3=speed4=0;
			}
}
}
}

