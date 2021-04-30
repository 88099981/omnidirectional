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
#ifndef __LQ_TIMER_H_
#define __LQ_TIMER_H_
#include "include.h"

typedef enum    //TIMER���õĹܽ�
{
    TIMER1_CHA_A8,
    TIMER1_CHB_A9,
    TIMER2_CHA_A0,
    TIMER2_CHB_A1,
    TIMER2_CHA_A15,              //��ȫӳ��
    TIMER2_CHB_B3,               //��ȫӳ��

    TIMER3_CHA_A6,
    TIMER3_CHB_A7,

    TIMER3_CHA_C6,               //��ȫӳ��
    TIMER3_CHB_C7,               //��ȫӳ��
    TIMER3_CHA_B4,               //����ӳ��
    TIMER3_CHB_B5,               //����ӳ��

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
