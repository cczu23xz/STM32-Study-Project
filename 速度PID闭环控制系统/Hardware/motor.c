#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "Timer.h"
#include "stdio.h"
#include "Serial.h"
int16_t motorspeed=0;
int Pwm=0;
int Pwm_out=0;
uint16_t Dead_Voltage=520; //540;


void Motor_Init(void)//��������˿�PA2  PA33��ʼ��
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3 | GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void PWM_Init(void)//ʹ��TIM4��PWMģʽ��ʹ��ͨ��1,PB7
{
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		TIM_InternalClockConfig(TIM4);
	
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;   
		TIM_TimeBaseInitStructure.TIM_Period=1000 - 1;                     //arr�Զ���װֵ����7200,PWMƵ��10KHz  
		TIM_TimeBaseInitStructure.TIM_Prescaler=4 - 1;                       //psc����Ƶ
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;        //���˲�
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Down;    //���¼���ģʽ
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);               //��ʼ��
		
		//TIM4 CH2����Ƚ�ͨ������
		 TIM_OCInitTypeDef  TIM_OCInitStructure;
		 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;            //PWMģʽ1
		 TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;  //��PWMʹ��
		 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������
		 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ����Ƚ�ʹ��
		 TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
		 TIM_OC2Init(TIM4, &TIM_OCInitStructure); 	                 //��ʼ��  ʹ��ͨ��2

		 TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
   
		 //TIM4ʹ��	
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

void  Motor_SetSpin(int Pwm)//�������ת����
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
