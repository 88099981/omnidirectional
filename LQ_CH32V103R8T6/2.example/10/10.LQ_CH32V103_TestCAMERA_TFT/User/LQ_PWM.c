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
#include "include.h"
/*************************************************************************
*  �������ƣ�void PWM_PinConfig(PWM_Name_t pch)
*  ����˵�����ܽų�ʼ��
*  ����˵����pch,PWMͨ������Ӧ�ĵĶ�ʱ�����ܽ�
            u32 dutyռ�ձ�Ϊ�ٷ�֮duty*100/PWM_DUTY_MAX
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��Set_TIM3PWM_Duty(1��1000);     // ���������PWMΪ12.5Khz��ռ�ձ�Ϊ10%
*************************************************************************/
void PWM_PinConfig(PWM_Name_t pch)
{
    switch(pch)
    {
        case PWM1_CH1_PA8:
            PIN_InitConfig(PA8, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM1_CH2_PA9:
            PIN_InitConfig(PA9, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM1_CH3_PA10:
            PIN_InitConfig(PA10, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM1_CH4_PA11:
            PIN_InitConfig(PA11, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;

        case PWM2_CH1_PA0:
            PIN_InitConfig(PA0, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM2_CH2_PA1:
            PIN_InitConfig(PA1, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM2_CH3_PA2:
            PIN_InitConfig(PA2, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM2_CH4_PA3:
            PIN_InitConfig(PA3, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;

        case PWM2_CH1_PA15:
            PIN_InitConfig(PA15, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM2_CH2_PB3:
            PIN_InitConfig(PB3, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM2_CH3_PB10:
            PIN_InitConfig(PB10, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM2_CH4_PB11:
            PIN_InitConfig(PB11, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;

        case PWM3_CH1_PA6:
            PIN_InitConfig(PA6, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM3_CH2_PA7:
            PIN_InitConfig(PA7, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM3_CH3_PB0:
            PIN_InitConfig(PB0, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM3_CH4_PB1:
            PIN_InitConfig(PB1, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;

        case PWM3_CH1_PC6:
            PIN_InitConfig(PC6, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM3_CH2_PC7:
            PIN_InitConfig(PC7, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM3_CH3_PC8:
            PIN_InitConfig(PC8, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM3_CH4_PC9:
            PIN_InitConfig(PC9, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;

        case PWM3_CH1_PB4:
            PIN_InitConfig(PB4, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM3_CH2_PB5:
            PIN_InitConfig(PB5, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;

        case PWM4_CH1_PB6:
            PIN_InitConfig(PB6, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM4_CH2_PB7:
            PIN_InitConfig(PB7, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM4_CH3_PB8:
            PIN_InitConfig(PB8, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        case PWM4_CH4_PB9:
            PIN_InitConfig(PB9, GPO, 0, GPIO_Speed_50MHz|GPIO_Mode_AF_PP);
            break;
        default:
            break;
    }
}

/*************************************************************************
*  �������ƣ�void PWM_InitConfig(PWM_Name_t pch, u32 freq, u32 duty)
*  ����˵�������PWM��ʼ��
*  ����˵����pch,PWMͨ������Ӧ�ĵĶ�ʱ�����ܽ�
            u32 dutyռ�ձ�Ϊ�ٷ�֮duty*100/PWM_DUTY_MAX
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��PWM_InitConfig(PWM3_CH1_C6, 50, 5000);   //��ʼ��PWM3 ͨ��1 ʹ������C6 ���PWMƵ��50HZ ռ�ձ�Ϊ�ٷ�֮ 5000/PWM_DUTY_MAX*100
*************************************************************************/
void PWM_InitConfig(PWM_Name_t pch, u32 freq, u32 duty)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    u16 tmduty;                                               //ռ�ձ�ֵ
    u16 tmperiod;                                             //����ֵ
    u16 freq_div = 0;                                         //��Ƶֵ

    PWM_PinConfig(pch);                                       //PWM���ų�ʼ��
    //pwm_gpio_init(pch);
    if((pch >> 8) == 0x00)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //������ʱ��1ʱ��
    else if((pch >> 8) == 0x01)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //������ʱ��2ʱ��
    else if((pch >> 8) == 0x02)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //������ʱ��3ʱ��
    else if((pch >> 8) == 0x03)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //������ʱ��4ʱ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);      //����AFIO���ù���ģ��ʱ��

    //������ӳ��
    if((pch >> 4) == 0x10)        //PWM1�Ĳ�����ӳ��
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
    else if((pch >> 4) == 0x11)   //PWM2����ȫ��ӳ��
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else if((pch >> 4) == 0x21)   //PWM3����ȫ��ӳ��
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else if((pch >> 4) == 0x22)   //PWM3�Ĳ�����ӳ��
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

    //��ȡϵͳ��Ƶ
    SystemCoreClock = 8000000 * (((RCC->CFGR0 >> 18)&0x0F) + 2);

    freq_div = (u16)((SystemCoreClock / freq) >> 16);                           //���ٷ�Ƶ
    tmperiod = (u16)(SystemCoreClock/(freq*(freq_div + 1)));                    //����
    tmduty = tmperiod * duty / PWM_DUTY_MAX;                                //ռ�ձ�

    //��ʼ��TIM1-4
    TIM_TimeBaseStructure.TIM_Period = tmperiod - 1;                        //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = freq_div;                         //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    //��ʼ��TIM1-4 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                       //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = tmduty;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                //�������:TIM����Ƚϼ���
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;               //�������:TIM����Ƚϼ���
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_TimeBaseInit(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_TimeBaseStructure);  //��ʼ��TIMx��ʱ��

    if((pch & 0x03) == 0x00)                                                //ͨ��ѡ��
    {
        TIM_OC1Init(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_OCInitStructure );                       //��ʱ��ͨ��1��ʼ��
        TIM_OC1PreloadConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OCPreload_Enable);               //��ʱ��Ԥװ������
        TIM_OC1FastConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OC1FE);                             //�Ƚϲ���ͨ������ʹ��
    }
    else if((pch & 0x03) == 0x01)
    {
        TIM_OC2Init(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_OCInitStructure );
        TIM_OC2PreloadConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OCPreload_Enable);
        TIM_OC2FastConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OC2FE);                             //�Ƚϲ���ͨ������ʹ��
    }
    else if((pch & 0x03) == 0x02)
    {
        TIM_OC3Init(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_OCInitStructure );
        TIM_OC3PreloadConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OCPreload_Enable);
        TIM_OC3FastConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OC3FE);                             //�Ƚϲ���ͨ������ʹ��
    }
    else if((pch & 0x03) == 0x03)
    {
        TIM_OC4Init(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_OCInitStructure );
        TIM_OC4PreloadConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OCPreload_Enable);
        TIM_OC4FastConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OC4FE);                             //�Ƚϲ���ͨ������ʹ��
    }

    TIM_Cmd(((TIM_TypeDef *) TIMERx[pch>>8]), ENABLE);                                              //��ʱ��ʹ��
    TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIMERx[pch>>8]), ENABLE );
    //TIM_ARRPreloadConfig( TIM1, ENABLE );
}

/*************************************************************************
*  �������ƣ�void Set_TIM3PWM_Duty(u8 ch,u16 duty)
*  ����˵�������PWM��ʼ��
*  ����˵����pch,PWMͨ������Ӧ�ĵĶ�ʱ�����ܽ�
            u32 dutyռ�ձ�Ϊ�ٷ�֮duty*100/PWM_DUTY_MAX
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��PWM_Set_Duty(PWM3_CH1_PC6, 5000);
*************************************************************************/
void PWM_Set_Duty(PWM_Name_t pch, u32 duty)
{
    u16 tmduty;
    u16 tmperiod;

    tmperiod = ((TIM_TypeDef *) TIMERx[pch>>8])->ATRLR;  //��ȡ��ʱ������ֵ
    tmduty = tmperiod * duty / PWM_DUTY_MAX;             //����ռ�ձ�

    if((pch >> 8) == 0x00)
    {
        (*(volatile u32*)(0x40012C34 + (pch&0x03) * 0x04)) = tmduty;
    }
    else if((pch >> 8) == 0x01)
    {
        (*(volatile u32*)(0x40000034 + (pch&0x03) * 0x04)) = tmduty;
    }
    else if((pch >> 8) == 0x02)
    {
        (*(volatile u32*)(0x40000434 + (pch&0x03) * 0x04)) = tmduty;
    }
    else if((pch >> 8) == 0x03)
    {
        (*(volatile u32*)(0x40000834 + (pch&0x03) * 0x04)) = tmduty;
    }
}
//-------------------------------------------------------------------------------------------------------------------
/*************************************************************************
*  �������ƣ�void PWM_Set_Freq(PWM_Name_t pch, u32 freq, u32 duty)
*  ����˵�������PWM��ʼ��
*  ����˵���� pch,  һ����ʱ�����ĸ�ͨ������Ӧ1234 PC6--9
             duty 0--TIM3_PWM_DUTY_MAX 10000��Ӧ0--100%
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��PWM_Set_Duty(PWM3_CH1_PC6, 5000);
*************************************************************************/
void PWM_Set_Freq(PWM_Name_t pch, u32 freq, u32 duty)
{
    u16 tmperiod = 0;                                     //����ֵ
    u16 freq_div = 0;                                        //��Ƶֵ
    u16 tmduty;

    freq_div = (u16)((SystemCoreClock / freq) >> 16);                //������ٷ�Ƶ
    tmperiod = (u16)(SystemCoreClock/(freq*(freq_div + 1)));      //��������

    ((TIM_TypeDef *) TIMERx[pch>>8])->ATRLR = tmperiod - 1 ;
    ((TIM_TypeDef *) TIMERx[pch>>8])->PSC = freq_div;
    tmduty = tmperiod * duty / PWM_DUTY_MAX;             //ռ�ձ�
    if((pch >> 8) == 0x00)
    {
        (*(volatile u32*)(0x40012C34 + (pch&0x03) * 0x04)) = tmduty;
    }
    else if((pch >> 8) == 0x01)
    {
        (*(volatile u32*)(0x40000034 + (pch&0x03) * 0x04)) = tmduty;
    }
    else if((pch >> 8) == 0x02)
    {
        (*(volatile u32*)(0x40000434 + (pch&0x03) * 0x04)) = tmduty;
    }
    else if((pch >> 8) == 0x03)
    {
        (*(volatile u32*)(0x40000834 + (pch&0x03) * 0x04)) = tmduty;
    }
}

