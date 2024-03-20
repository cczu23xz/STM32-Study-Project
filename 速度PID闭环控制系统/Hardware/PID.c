#include "stm32f10x.h"                  // Device header
#include "Motor.h"
float  Velocity_Kp=37,Velocity_Ki= 4,Velocity_Kd=0;//80 5
float  Add_Pwm=0;
//����ʽPID
int Velocity_PID(int reality,int target)
{
	static float Error,Last_Error=0,Pre_Error=0,Pwm;
	
	Error=target-reality;
	
	Add_Pwm=(  Velocity_Kp * (Error-Last_Error)//����
	+Velocity_Ki * Error+Velocity_Kd*(Error-2 * Last_Error + Pre_Error )  );
//	     ���� 	΢��
	if(Add_Pwm>1 || Add_Pwm<-1)
	{
		Pwm += Add_Pwm;
	}//����
	Pre_Error=Last_Error;
	Last_Error=Error;
	Pwm=PWM_xianfu(Pwm);

	return Pwm;
}
