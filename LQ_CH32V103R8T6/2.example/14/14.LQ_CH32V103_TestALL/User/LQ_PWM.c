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
#include "include.h"
/*************************************************************************
*  函数名称：void PWM_PinConfig(PWM_Name_t pch)
*  功能说明：管脚初始化
*  参数说明：pch,PWM通道所对应的的定时器及管脚
            u32 duty占空比为百分之duty*100/PWM_DUTY_MAX
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：Set_TIM3PWM_Duty(1，1000);     // 电机驱动的PWM为12.5Khz，占空比为10%
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
*  函数名称：void PWM_InitConfig(PWM_Name_t pch, u32 freq, u32 duty)
*  功能说明：电机PWM初始化
*  参数说明：pch,PWM通道所对应的的定时器及管脚
            u32 duty占空比为百分之duty*100/PWM_DUTY_MAX
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：PWM_InitConfig(PWM3_CH1_C6, 50, 5000);   //初始化PWM3 通道1 使用引脚C6 输出PWM频率50HZ 占空比为百分之 5000/PWM_DUTY_MAX*100
*************************************************************************/
void PWM_InitConfig(PWM_Name_t pch, u32 freq, u32 duty)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    u16 tmduty;                                               //占空比值
    u16 tmperiod;                                             //周期值
    u16 freq_div = 0;                                         //分频值

    PWM_PinConfig(pch);                                       //PWM引脚初始化
    //pwm_gpio_init(pch);
    if((pch >> 8) == 0x00)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //开启定时器1时钟
    else if((pch >> 8) == 0x01)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //开启定时器2时钟
    else if((pch >> 8) == 0x02)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //开启定时器3时钟
    else if((pch >> 8) == 0x03)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  //开启定时器4时钟

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);      //开启AFIO复用功能模块时钟

    //引脚重映射
    if((pch >> 4) == 0x10)        //PWM1的部分重映射
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
    else if((pch >> 4) == 0x11)   //PWM2的完全重映射
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else if((pch >> 4) == 0x21)   //PWM3的完全重映射
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else if((pch >> 4) == 0x22)   //PWM3的部分重映射
        GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

    //获取系统主频
    SystemCoreClock = 8000000 * (((RCC->CFGR0 >> 18)&0x0F) + 2);

    freq_div = (u16)((SystemCoreClock / freq) >> 16);                           //多少分频
    tmperiod = (u16)(SystemCoreClock/(freq*(freq_div + 1)));                    //周期
    tmduty = tmperiod * duty / PWM_DUTY_MAX;                                //占空比

    //初始化TIM1-4
    TIM_TimeBaseStructure.TIM_Period = tmperiod - 1;                        //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = freq_div;                         //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    //初始化TIM1-4 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                       //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //比较输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = tmduty;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                //输出极性:TIM输出比较极性
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_Low;               //输出极性:TIM输出比较极性
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_TimeBaseInit(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_TimeBaseStructure);  //初始化TIMx的时基

    if((pch & 0x03) == 0x00)                                                //通道选择
    {
        TIM_OC1Init(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_OCInitStructure );                       //定时器通道1初始化
        TIM_OC1PreloadConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OCPreload_Enable);               //定时器预装载配置
        TIM_OC1FastConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OC1FE);                             //比较捕获通道快速使能
    }
    else if((pch & 0x03) == 0x01)
    {
        TIM_OC2Init(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_OCInitStructure );
        TIM_OC2PreloadConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OCPreload_Enable);
        TIM_OC2FastConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OC2FE);                             //比较捕获通道快速使能
    }
    else if((pch & 0x03) == 0x02)
    {
        TIM_OC3Init(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_OCInitStructure );
        TIM_OC3PreloadConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OCPreload_Enable);
        TIM_OC3FastConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OC3FE);                             //比较捕获通道快速使能
    }
    else if((pch & 0x03) == 0x03)
    {
        TIM_OC4Init(((TIM_TypeDef *) TIMERx[pch>>8]), &TIM_OCInitStructure );
        TIM_OC4PreloadConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OCPreload_Enable);
        TIM_OC4FastConfig(((TIM_TypeDef *) TIMERx[pch>>8]), TIM_OC4FE);                             //比较捕获通道快速使能
    }

    TIM_Cmd(((TIM_TypeDef *) TIMERx[pch>>8]), ENABLE);                                              //定时器使能
    TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIMERx[pch>>8]), ENABLE );
    //TIM_ARRPreloadConfig( TIM1, ENABLE );
}

/*************************************************************************
*  函数名称：void Set_TIM3PWM_Duty(u8 ch,u16 duty)
*  功能说明：电机PWM初始化
*  参数说明：pch,PWM通道所对应的的定时器及管脚
            u32 duty占空比为百分之duty*100/PWM_DUTY_MAX
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：PWM_Set_Duty(PWM3_CH1_PC6, 5000);
*************************************************************************/
void PWM_Set_Duty(PWM_Name_t pch, u32 duty)
{
    u16 tmduty;
    u16 tmperiod;

    tmperiod = ((TIM_TypeDef *) TIMERx[pch>>8])->ATRLR;  //获取定时器周期值
    tmduty = tmperiod * duty / PWM_DUTY_MAX;             //计算占空比

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
*  函数名称：void PWM_Set_Freq(PWM_Name_t pch, u32 freq, u32 duty)
*  功能说明：电机PWM初始化
*  参数说明： pch,  一个定时器有四个通道，对应1234 PC6--9
             duty 0--TIM3_PWM_DUTY_MAX 10000对应0--100%
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：PWM_Set_Duty(PWM3_CH1_PC6, 5000);
*************************************************************************/
void PWM_Set_Freq(PWM_Name_t pch, u32 freq, u32 duty)
{
    u16 tmperiod = 0;                                     //周期值
    u16 freq_div = 0;                                        //分频值
    u16 tmduty;

    freq_div = (u16)((SystemCoreClock / freq) >> 16);                //计算多少分频
    tmperiod = (u16)(SystemCoreClock/(freq*(freq_div + 1)));      //计算周期

    ((TIM_TypeDef *) TIMERx[pch>>8])->ATRLR = tmperiod - 1 ;
    ((TIM_TypeDef *) TIMERx[pch>>8])->PSC = freq_div;
    tmduty = tmperiod * duty / PWM_DUTY_MAX;             //占空比
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

