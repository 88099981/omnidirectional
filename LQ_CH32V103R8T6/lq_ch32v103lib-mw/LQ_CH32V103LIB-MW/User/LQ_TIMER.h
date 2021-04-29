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
#ifndef __LQ_TIMER_H_
#define __LQ_TIMER_H_
#include "include.h"

typedef enum    //TIMER可用的管脚
{
    TIMER1_CHA_A8,
    TIMER1_CHB_A9,
    TIMER2_CHA_A0,
    TIMER2_CHB_A1,
    TIMER2_CHA_A15,              //完全映射
    TIMER2_CHB_B3,               //完全映射

    TIMER3_CHA_A6,
    TIMER3_CHB_A7,

    TIMER3_CHA_C6,               //完全映射
    TIMER3_CHB_C7,               //完全映射
    TIMER3_CHA_B4,               //部分映射
    TIMER3_CHB_B5,               //部分映射

    TIMER4_CHA_B6,
    TIMER4_CHB_B7,

}TIMER_PIN_Name_t;

typedef enum
{
    TIMER_1,
    TIMER_2,
    TIMER_3,
    TIMER_4,
}TIMER_Name_t;
extern const u32 TIMERx[] ;
void TIM1_Int_Init(u16 x100us);
void TIMER_InitConfig(TIMER_Name_t timern, u16 ms);
#endif
