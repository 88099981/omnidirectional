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
#ifndef __LQ_SYSDELAY_H_
#define __LQ_SYSDELAY_H_
#include "include.h"

#define true 1
#define false 0

//#define USEOLED
#define USETFT   //ʹ��OLED����TFT1.8ģ�飬Ĭ��ΪTFT1.8
//#define USETIMER1DMA
#define USEREAD

#define LQMT9V034   //Ĭ��Ϊ��������ͷ
extern u32 SystemClock;

void SysTick_Init(void);
void Delay_Us (uint32_t n);
void Delay_Ms (uint32_t n);
void delayms (uint32_t n);
void RCC_ConfigInit(void);
void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status);
#endif



