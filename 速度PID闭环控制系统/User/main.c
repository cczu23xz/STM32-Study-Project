#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"
#include "PID.h"
#include "Serial.h"
#include "Motor.h"
int16_t Speed;
int16_t PWM=0;
int32_t Encoder_Reality=0;
uint32_t Speed_want_cal=0;
int32_t Encoder_Target;
int32_t Speed_want= -100;
int a=0;

int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	Serial_Init();
	Motor_Init();
	PWM_Init();
	while(1)
	{
		printf("%d,%d,%d,%d,%d\n",PWM,Encoder_Reality,Encoder_Target,Position_Angle(Encoder_Reality),a);
	}
}


void TIM2_IRQHandler(void)//定时器2中断进行PID控制,10ms采样一次
{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
		{
			Encoder_Reality = Encoder_Get();
			Encoder_Target=Encoder_cal(Speed_want);
			PWM=Velocity_PID(Encoder_Reality, Encoder_Target);
			Motor_SetSpin(PWM);
			//printf("%d,%d,%d,%d,%d,%d\n",PWM,Encoder_Reality,Encoder_Target,Position_Angle(Encoder_Reality),flag,a);
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		}
}

void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	uint8_t RxData = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		RxData = USART_ReceiveData(USART1);
		if(RxState == 0)
		{
			Speed_want = RxData;
			RxState=1;
		}
		else if(RxState == 1)
		{
			if(RxData == '\n')
			{
				RxState = 0;
			}
		}
		//Speed_want = USART_ReceiveData(USART1);
		//Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
