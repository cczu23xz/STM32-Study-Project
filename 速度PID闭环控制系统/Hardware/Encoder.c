#include "stm32f10x.h"                  // Device header

float initial = 0;
void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;
}
/**********************************
���ת����Ϣ��ÿ���Ӷ���ת
***********************************/
int  Motor_Speed(int Speed)
{
	int ma_pa=13;//������̣�13��
	float	jian_su=30;//370������ٱȣ�30
	int mai_chong=4;//һ�����ڵ���������4
	int count_time=10;//����ʱ�䣺10ms
	return (Speed/ma_pa/mai_chong/jian_su)*(1000/count_time)*60;
}

/*****************
���λ����Ϣ
*******************/

int Position_Angle(int Speed)
{

	initial += Speed;
	//Speed/ma_pa/mai_chong/jian_su=t=���ת�˶���Ȧ
	//(t*100)%100=m�õ�С�������λ������
	//m*180�͵õ���ת����λ��
	//����Ǹ�����(1-m)*180
	double  a=( initial/1560*100 );
	int b =(int)  ( initial/1560*100 );
	int c= b*360%36000/100;
	//int b =  a*10000;
	//float t= (b%100)/100;
	return c;
}
/*******************************
����ת�ٶ�Ӧ������������
********************************/
int32_t Encoder_cal(int32_t Speed_want)
{
	return (Speed_want*13*30*4)/(60*1000/10);

}
/**************************************************************************
��    ��: ����ת�ٶ�Ӧ������������
��    ��: rpm��ת�٣�ppr����������ratio�����ٱȣ�cnt_time������ʱ��(ms)
�� �� ֵ: ��������� 
**************************************************************************/
long Rpm_Encoder_Cnt(float rpm,uint16_t ppr,uint16_t ratio,uint16_t cnt_time)
{
    return (rpm*ratio*ppr*4)/(60*1000/cnt_time);               
}
