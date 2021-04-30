/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 ��ƽ    ̨�������������ܿƼ�SPIN27PS/CH32V103R8T6���İ�
 ����    д��chiusir
 ��E-mail  ��chiusir@163.com
 ������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
 �������¡�2020��10��28��
 �������Ϣ�ο����е�ַ��
 ����    վ��http://www.lqist.cn
 ���Ա����̡�http://longqiu.taobao.com
 ------------------------------------------------
 ��IDE��CH32V103R8T6:MounRiver Studio�����ϰ汾
 ��IDE��MM32SPIN27PS:IAR7.8/MDK5.2�����ϰ汾  
 ��Target �� SPIN27PS/CH32V103R8T6
 ��SYS PLL�� 80/96MHz
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_
#define SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_

#include "include.h"

#define  Kbat       2    // ��ص�ѹ�Ե��ռ�ձȵ�Ӱ��ϵ��
#define  Kencoder   5    // �������ٶȶԵ��ռ�ձȵ�Ӱ��ϵ��
#define  Koffset    1    // �������ʶԵ��ռ�ձȵ�Ӱ��ϵ��

#define TURN_LEFT    0   // ��ת
#define TURN_RIGHT   1   // ��ת
#define IN_GARAGE    1   // ���
#define OUT_GARAGE   0   // ����

#define Servo_Delta 150            //�������ת���Ĳ�ֵ�������ͺţ����˺Ͷ���۳��й�
#define Servo_Center_Mid 1440      //���ֱ����ֵ��
#define Servo_Left_Max   (Servo_Center_Mid+Servo_Delta)      //�����ת����ֵ
#define Servo_Right_Min  (Servo_Center_Mid-Servo_Delta)      //�����ת����ֵ����ֵ��������÷�ʽ�йأ���ʽ

#define MtTargetDuty  2100  //���ռ�ձ�ֵ�����ֵ��ʵ����Ҫ��ȥ��ص�ѹ�����������������ʣ��ﲻ����ֵ

#define MTGPIO1p      GPIOC,GPIO_Pin_10 
#define MTGPIO2p      GPIOC,GPIO_Pin_11   
#define MTGPIO3p      GPIOC,GPIO_Pin_12   
#define MTGPIO4p      GPIOC,GPIO_Pin_13  

extern int16_t ServoDuty;
extern int16_t MotorDuty1;      // �������ռ�ձ���ֵ
extern int16_t MotorDuty2;      // �������ռ�ձ���ֵ

extern uint16_t BatVolt;         // ��ص�ѹ�ɼ�
extern unsigned char Camera_Flag; // ����ʶ
extern uint8_t CircleNumber;// �뻷������0������Ĭ��1�� ;���ĸ���һ���ڱ���ǰ��������ʱ��֪����
extern int16_t OFFSET0;    // ��Զ������������ֵ�ۺ�ƫ����
extern int16_t OFFSET1;    // �ڶ���
extern int16_t OFFSET2;    // �����������

void EncInit(void);
void MotorInit(u16 freq);
void MotorCtrl(int16_t motor1, int16_t motor2);
void MotorCtrl4w(int16_t motor1, int16_t motor2,int16_t motor3, int16_t motor4);
void TestMotor(void);
void ServoInit(u16 freq);
void ServoCtrl(uint16_t duty);
void TestServo(void);
void SpeedCtrl(signed short x, signed short y, signed short z);
#endif /* SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_ */
