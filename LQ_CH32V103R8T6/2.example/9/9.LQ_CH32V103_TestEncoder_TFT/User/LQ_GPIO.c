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

extern unsigned char Image_Data[LCDH+2][LCDW];
const u32 PORTX[4] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE};

void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI9_5_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI15_10_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
// 干簧管外部中断
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
  { /*清除EXTI12线路挂起位*/
    EXTI_ClearITPendingBit(EXTI_Line0);
    //LED_Ctrl(LED1,RVS);
  }
}
void EXTI1_IRQHandler(void)
{
  //清除EXTI1线路挂起位//
  EXTI_ClearFlag(EXTI_Line1);    //摄像头用B1
}
// 干簧管外部中断
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
  { /*清除EXTI12线路挂起位*/
    EXTI_ClearITPendingBit(EXTI_Line2);  //干簧管用D2,
    LED_Ctrl(LED1,RVS);
  }
}
#ifdef USETIMER1DMA
void EXTI4_IRQHandler(void)
{
  //清除EXTI4线路挂起位//
  EXTI_ClearFlag(EXTI_Line4); //摄像头场中断用B4

  MT9V034_DMA_START();
  //MT9V034_DMA_CH->CNTR = LCDH*MT9V034_IMAGEW;             //设置当前DMA传输的剩余数量，向下递减，该寄存器只能在DMA不工作时更改。
  //MT9V034_DMA_CH->CFGR |= DMA_CFGR1_EN;
  //LED_Ctrl(LED1,RVS);
}

#else
// 干簧管和摄像头都要用到，
void EXTI4_IRQHandler(void)
{
  //清除EXTI4线路挂起位//
  EXTI_ClearFlag(EXTI_Line4); //摄像头场中断用B4

  hang= 0;
  hangcnt= 0;
  Camera_Flag = 0;
  //LED_Ctrl(LED1,RVS);
}
#endif
void EXTI3_IRQHandler(void)
{    //清除EXTI3线路挂起位//
  EXTI_ClearFlag(EXTI_Line3);    //摄像头行用C3
  if(hang%2==0)
  {
    for(lie=0;lie<LCDW;lie++)
    {
      Image_Data[hangcnt][lie]=(u8)((GPIOC->INDR)>>8);
    }
    if(++hangcnt>LCDH)
    {
      hangcnt=LCDH;
      Camera_Flag=1;
    }
  }
  ++hang;
  LED_Ctrl(LED1,RVS);
}

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
  { /*清除EXTI12线路挂起位*/
    EXTI_ClearITPendingBit(EXTI_Line5);
    //LED_Ctrl(LED1,RVS);
  }
}
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
  { /*清除EXTI12线路挂起位*/
    EXTI_ClearITPendingBit(EXTI_Line12);
    LED_Ctrl(LED1,RVS);
  }
}
/*************************************************************************
*  函数名称：void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
*  功能说明：GPIO初始化函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            dir     ： GPIO 模式   lq_gpio.h中的宏定义中选择
*            output   ： 输出模式时，输出电平 1：高电平  0：低电平
*            u32 pinconf :管脚配置,参考ch32v10x_gpio.h中的枚举GPIO_Mode_AF_PP等
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例： PIN_InitConfig(PA3, GPO, 1,GPIO_Mode_Out_PP);  //PA3初始化推挽输出 高电平
*************************************************************************/
void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode dir, unsigned char output,u32 pinconf)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    u8 port_index = pin >> 4;

    //GPIO总线时钟使能
         if(0 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(1 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(2 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(3 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(4 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    else if(5 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
    else if(6 == port_index) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

    //设置管脚索引
    GPIO_InitStructure.GPIO_Pin = (u16)(1 << (pin & 0x0F));

    if(GPO == dir)
    {  //设置GPIO速度
        if((u16)pinconf >> 8 == 0) GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)(GPIO_Speed_50MHz);  //防止没有填写这个参数,默认设置50Mhz
        else GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)((u16)pinconf >> 8);                       //已经填写了该参数。根据值来设置
    }

    //设置模式 ：输入、输出、上拉、复用、悬空、AD等
    GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)((u16)pinconf & 0xff);
    GPIO_Init((GPIO_TypeDef *)(PORTX[port_index]), &GPIO_InitStructure);

    if(GPO == dir)//设置输出高低电平，高电平默认，作为输入口不设置
    {
        GPIO_WriteBit((GPIO_TypeDef *)(PORTX[port_index]), (u16)(1 << (pin & 0x0F)), output);
    }
}

/*************************************************************************
*  函数名称：unsigned char PIN_Read(GPIO_Name_t pin)
*  功能说明：读取GPIO电平函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：uint8_t pinState = PIN_Read(P00_0);  //读取P00_0 的电平状态
*************************************************************************/
unsigned char PIN_Read(GPIO_Name_t pin)
{
    if((((GPIO_TypeDef *)(PORTX[pin>>4]))->INDR & (1<<(pin & 0x0F))) != 0)
        {
            return 1;
        }
        return 0;
}

/*************************************************************************
*  函数名称：void PIN_Write(GPIO_Name_t pin, unsigned char output)
*  功能说明：设置GPIO输出状态
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*            output   ： 输出模式时，输出电平 1：高电平  0：低电平
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Write(P00_0, 1);//P00_0置为高电平
*************************************************************************/
void PIN_Write(GPIO_Name_t pin, unsigned char output)
{

    if (output == 1)
             ((GPIO_TypeDef *)(PORTX[pin>>4]))->BSHR = (1<<(pin & 0x0F));
        else
             ((GPIO_TypeDef *)(PORTX[pin>>4]))->BCR = (1<<(pin & 0x0F));
}

/*************************************************************************
*  函数名称：void PIN_Reverse(GPIO_Name_t pin)
*  功能说明：GPIO电平翻转函数，使用前请先初始化
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Reverse(P00_0); //翻转P00_0管脚的电平状态
*************************************************************************/
void PIN_Reverse(GPIO_Name_t pin)
{
    ((GPIO_TypeDef *)(PORTX[pin>>4]))->OUTDR ^= (u16)(1<<(pin & 0x0F));
}

/*************************************************************************
*  函数名称：void PIN_Exti(GPIO_Name_t pin, EXTITrigger_TypeDef edge,u32 pinconf)
*  功能说明：GPIO外部中断初始化
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            dir     ： GPIO 模式   lq_gpio.h中的宏定义中选择*
*            u32 pinconf :管脚配置,参考ch32v10x_gpio.h中的枚举GPIO_Mode_AF_PP等
*            u8 prior 中断优先级
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Exti(PA12, EXTI_Trigger_Falling,GPIO_Mode_IPU);//配置PA12下降沿触发中断
*  备    注：ch32v103只有几个个固定的GPIO支持外部中断
*************************************************************************/
void PIN_Exti(GPIO_Name_t pin, EXTITrigger_TypeDef edge,u32 pinconf,u8 prior)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    PIN_InitConfig(pin, GPI, 0, pinconf);
    GPIO_EXTILineConfig(pin>>4, pin & 0x0F);  //管脚中断号配置
    EXTI_InitStructure.EXTI_Line = 1<<(pin & 0x0F);         //管脚号配置
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //触发模式
    EXTI_InitStructure.EXTI_Trigger = edge;                 //触发方式
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //使能或者失能
    EXTI_Init(&EXTI_InitStructure);

    if(pin & 0x0F < 5)
    {        //使能按键所在的外部中断通道
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn + pin & 0x0F;
    }
    else if(pin & 0x0F < 10)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    }
    else
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    }
    //子优先级2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prior;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    //使能外部中断通道//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器//
    NVIC_Init(&NVIC_InitStructure);

}
