#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "Timer.h"
#include "stdio.h"
#include "Serial.h"
int16_t motorspeed=0;
int Pwm=0;
int Pwm_out=0;
uint16_t Dead_Voltage=520; //540;


void Motor_Init(void)//电机驱动端口PA2  PA33初始化
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3 | GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void PWM_Init(void)//使用TIM4的PWM模式，使能通道1,PB7
{
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		TIM_InternalClockConfig(TIM4);
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;   
		TIM_TimeBaseInitStructure.TIM_Period=1000 - 1;                     //arr自动重装值计数7200,PWM频率10KHz  
		TIM_TimeBaseInitStructure.TIM_Prescaler=4 - 1;                       //psc不分频
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;        //不滤波
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Down;    //向下计数模式
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);               //初始化
		
		//TIM4 CH2输出比较通道配置
		 TIM_OCInitTypeDef  TIM_OCInitStructure;
		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;            //PWM模式1
		 TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //打开PWM使能
		 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性
		 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出比较使能
		 TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
		 TIM_OC2Init(TIM4, &TIM_OCInitStructure); 	                 //初始化  使用通道2

		 TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
   
		 //TIM4使能	
		 TIM_Cmd(TIM4,ENABLE);
		 
}

void Motor_Stop(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	TIM_SetCompare2(TIM4,0);
}	

int num_abs(int num)
{
	if(num>0)
	return num;
	else
	return -num;
}

int16_t Get_motorspeed(void)
{
	motorspeed=Encoder_Get();
	return motorspeed;
}

int PWM_xianfu(int Pwm)
{
	if(Pwm>5000)
	Pwm=5000;
	if(Pwm<-5000)
	Pwm=-5000;
	return Pwm;
}

void  Motor_SetSpin(int Pwm)//电机正反转设置
{
	if(Pwm>0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		Pwm=PWM_xianfu(Pwm);
		TIM_SetCompare2(TIM4,num_abs(Pwm)+Dead_Voltage);
	}
	else if(Pwm<0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		Pwm=PWM_xianfu(Pwm);
		TIM_SetCompare2(TIM4,num_abs(Pwm)+Dead_Voltage);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		TIM_SetCompare2(TIM4,0);
	}
	
}
