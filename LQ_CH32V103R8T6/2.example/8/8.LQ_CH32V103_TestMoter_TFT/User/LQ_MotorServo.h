/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技SPIN27PS/CH32V103R8T6核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【IDE】CH32V103R8T6:MounRiver Studio及以上版本
 【IDE】MM32SPIN27PS:IAR7.8/MDK5.2及以上版本  
 【Target 】 SPIN27PS/CH32V103R8T6
 【SYS PLL】 80/96MHz
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_
#define SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_

#include "include.h"

#define  Kbat       2    // 电池电压对电机占空比的影响系数
#define  Kencoder   5    // 编码器速度对电机占空比的影响系数
#define  Koffset    1    // 赛道曲率对电机占空比的影响系数

#define TURN_LEFT    0   // 左转
#define TURN_RIGHT   1   // 右转
#define IN_GARAGE    1   // 入库
#define OUT_GARAGE   0   // 出库

#define Servo_Delta 150            //舵机左右转动的差值，与舵机型号，拉杆和舵机臂长有关
#define Servo_Center_Mid 1440      //舵机直行中值，
#define Servo_Left_Max   (Servo_Center_Mid+Servo_Delta)      //舵机左转极限值
#define Servo_Right_Min  (Servo_Center_Mid-Servo_Delta)      //舵机右转极限值，此值跟舵机放置方式有关，立式

#define MtTargetDuty  2100  //电机占空比值得最大值，实际需要减去电池电压，编码器，赛道曲率，达不到该值

#define MTGPIO1p      GPIOC,GPIO_Pin_10 
#define MTGPIO2p      GPIOC,GPIO_Pin_11   
#define MTGPIO3p      GPIOC,GPIO_Pin_12   
#define MTGPIO4p      GPIOC,GPIO_Pin_13  

extern int16_t ServoDuty;
extern int16_t MotorDuty1;      // 电机驱动占空比数值
extern int16_t MotorDuty2;      // 电机驱动占空比数值

extern uint16_t BatVolt;         // 电池电压采集
extern unsigned char Camera_Flag; // 场标识
extern uint8_t CircleNumber;// 入环次数，0结束；默认1次 ;环的个数一般在比赛前测试赛道时就知道了
extern int16_t OFFSET0;    // 最远处，赛道中心值综合偏移量
extern int16_t OFFSET1;    // 第二格
extern int16_t OFFSET2;    // 最近，第三格

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
