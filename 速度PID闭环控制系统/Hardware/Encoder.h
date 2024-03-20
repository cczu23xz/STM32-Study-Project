#ifndef __ENCODER_H
#define __ENCODER_H

void Encoder_Init(void);
int16_t Encoder_Get(void);
int  Motor_Speed(int Speed);
int32_t Encoder_cal(int32_t Speed_want);
int Position_Angle(int Speed);
#endif
