#ifndef __SERIAL__H
#define __SERIAL__H
#include <stdio.h>
void Serial_Init(void);
void Serial_SendByte(int8_t Byte);
void Serial_SendArray(float *Array, uint16_t Length);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
void USART1_IRQHandler(void);
#endif
