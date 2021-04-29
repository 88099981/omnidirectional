#ifndef __LQ_IIC_H_
#define __LQ_IIC_H_
#include "include.h"


void Uart_ConfigInit(u32 bound);
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);
void I2CWrite(u8 sub, u8* ptr, u16 len);
void I2CRead(u8 sub, u8* ptr, u16 len);
void I2C_MasterMode_Init(I2C_TypeDef *I2Cx, u32 uiI2C_speed);
void I2CSetDeviceAddr(I2C_TypeDef *I2Cx, u8 deviceaddr);
void I2C_WaitEEready(void);
void I2C_TXByte(u8 dat);
u8 I2C_SendBytes(u8 sub, u8* ptr, u16 cnt);
u8 I2C_SendPacket(u8 sub, u8* ptr, u16 cnt);
void I2C_RevBytes(void);
void I2C_RcvPacket(u8 sub, u8* ptr, u16 cnt);
void I2C_Check(void);
void I2CInit(void);
void test_I2C(void);

#endif

















