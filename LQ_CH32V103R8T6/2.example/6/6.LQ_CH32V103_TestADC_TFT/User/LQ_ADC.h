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
=================================================================

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_ADC_H_
#define __LQ_ADC_H_

#include "include.h"
/* ADC Software start mask */
#define CTLR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)
#define CTLR2_EXTTRIG_SWSTART_Reset   ((uint32_t)0xFFAFFFFF)

void ADCInit(u8 ADC_Channel_x);
u16 ADC_Read(uint8_t ADC_Channel_x);
u16 ADC_Read_Average(uint8_t ADC_Channel_x,uint8_t times);
void Test_ADC(void);

#endif
//



