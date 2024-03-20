#ifndef __MOTOR_H__
#define __MOTOR_H__
void Motor_Init(void);
void PWM_Init(void);
void  Motor_SetSpin(int Pwm);//电机正反转设置
void Motor_Stop(void);
int num_abs(int num);
int16_t Get_motorspeed(void);
int PWM_xianfu(int Pwm);
#endif
