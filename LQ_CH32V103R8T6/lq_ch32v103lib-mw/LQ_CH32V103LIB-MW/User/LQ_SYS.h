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
#ifndef __LQ_SYSDELAY_H_
#define __LQ_SYSDELAY_H_
#include "include.h"

#define true 1
#define false 0

//#define USEOLED
#define USETFT   //使用OLED或者TFT1.8模块，默认为TFT1.8
//#define USETIMER1DMA
#define USEREAD

#define LQMT9V034   //默认为神眼摄像头
extern u32 SystemClock;

void SysTick_Init(void);
void Delay_Us (uint32_t n);
void Delay_Ms (uint32_t n);
void delayms (uint32_t n);
void RCC_ConfigInit(void);
void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status);
#endif



