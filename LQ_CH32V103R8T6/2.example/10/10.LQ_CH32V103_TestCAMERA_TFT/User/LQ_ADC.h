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



