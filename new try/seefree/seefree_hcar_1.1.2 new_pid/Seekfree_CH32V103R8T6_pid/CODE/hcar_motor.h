#ifndef _hcar_motor_h
#define _hcar_motor_h

#include "common.h"

#define MOTOR1_A   C10              //����1�������ת����
#define MOTOR1_B   PWM4_CH1_B6      //����1���PWM����

#define MOTOR2_A   C11               //����2�������ת����
#define MOTOR2_B   PWM4_CH2_B7      //����2���PWM����

#define MOTOR3_A   B12              //����3�������ת����
#define MOTOR3_B   PWM4_CH3_B8      //����3���PWM����

#define MOTOR4_A   A8              //����4�������ת����
#define MOTOR4_B   PWM4_CH4_B9      //����4���PWM����

//int32 speed1_power;
//int32 speed2_power;
//int32 speed3_power;
//int32 speed4_power;
//����
void hcar_motor_init(void);
void hcar_motor_control(int16 motor1, int16 motor2, int16 motor3, int16 motor4);
void hcar_speed_control(int16 x, int16 y, int16 z);

#endif