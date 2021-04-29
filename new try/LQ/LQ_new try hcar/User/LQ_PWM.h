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
#ifndef __LQ_PWM_H_
#define __LQ_PWM_H_
#include "include.h"

#define PWM_DUTY_MAX  1000//已修改


typedef enum
{
    PWM1_CH1_PA8 = 0x0000,
    PWM1_CH2_PA9,
    PWM1_CH3_PA10,
    PWM1_CH4_PA11,

    PWM2_CH1_PA0 = 0x0100,
    PWM2_CH2_PA1,
    PWM2_CH3_PA2,
    PWM2_CH4_PA3,

    PWM2_CH1_PA15 = 0x0110,     //完全映射
    PWM2_CH2_PB3,               //完全映射
    PWM2_CH3_PB10,              //完全映射
    PWM2_CH4_PB11,              //完全映射

    PWM3_CH1_PA6 = 0x0200,
    PWM3_CH2_PA7,
    PWM3_CH3_PB0,
    PWM3_CH4_PB1,


    PWM3_CH1_PC6 = 0x0210,      //完全映射
    PWM3_CH2_PC7,               //完全映射
    PWM3_CH3_PC8,               //完全映射
    PWM3_CH4_PC9,               //完全映射

    PWM3_CH1_PB4 = 0x0220,      //部分映射
    PWM3_CH2_PB5,               //部分映射

    PWM4_CH1_PB6 = 0x0300,
    PWM4_CH2_PB7,
    PWM4_CH3_PB8,
    PWM4_CH4_PB9
}PWM_Name_t;
void PWM_PinConfig(PWM_Name_t pch);
void PWM_InitConfig(PWM_Name_t pch, u32 freq, u32 duty);
void PWM_Set_Duty(PWM_Name_t pch, u32 duty);
void PWM_Set_Freq(PWM_Name_t pch, u32 freq, u32 duty);
#endif



