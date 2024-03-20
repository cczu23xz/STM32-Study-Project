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
电机转速信息：每分钟多少转
***********************************/
int  Motor_Speed(int Speed)
{
	int ma_pa=13;//电机码盘：13线
	float	jian_su=30;//370电机减速比：30
	int mai_chong=4;//一个周期的脉冲数：4
	int count_time=10;//计数时间：10ms
	return (Speed/ma_pa/mai_chong/jian_su)*(1000/count_time)*60;
}

/*****************
电机位置信息
*******************/

int Position_Angle(int Speed)
{

	initial += Speed;
	//Speed/ma_pa/mai_chong/jian_su=t=电机转了多少圈
	//(t*100)%100=m得到小数点后两位的数字
	//m*180就得到了转动的位置
	//如果是负数就(1-m)*180
	double  a=( initial/1560*100 );
	int b =(int)  ( initial/1560*100 );
	int c= b*360%36000/100;
	//int b =  a*10000;
	//float t= (b%100)/100;
	return c;
}
/*******************************
计算转速对应编码器脉冲数
********************************/
int32_t Encoder_cal(int32_t Speed_want)
{
	return (Speed_want*13*30*4)/(60*1000/10);

}
/**************************************************************************
功    能: 计算转速对应编码器脉冲数
输    入: rpm：转速；ppr：码盘数；ratio：减速比；cnt_time：计数时间(ms)
返 回 值: 电机脉冲数 
**************************************************************************/
long Rpm_Encoder_Cnt(float rpm,uint16_t ppr,uint16_t ratio,uint16_t cnt_time)
{
    return (rpm*ratio*ppr*4)/(60*1000/cnt_time);               
}
