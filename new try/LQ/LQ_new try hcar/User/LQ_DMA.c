/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年5月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Hightec4.9.3/Tasking6.3及以上版本
【Target 】 TC264DA
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void DMA1_Channel4_IRQHandler(void)
{
    if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC4))
    {
        DMA_ClearFlag(DMA1_FLAG_TC4);
        //MT9V034_DMA_CH->CFGR &= (u16)(~DMA_CFGR1_EN);   //关闭DMA
        MT9V034_DMA_CLS();
    }
}

/*************************************************************************
  * @brief    摄像头DMA传输初始化
  *
  * @param    srcStartAddr   ：源地址 IO地址
  * @param    dstStartAddr   ：目的地址 摄像头数据缓冲区
  * @param    channel        ：DMA传输通道 0-47      注意 DMA传输通道需要和PIX像素中断优先级保持一致
  * @param    lqsize                数据长度
  * @return   无
  *
  * @note     注意 DMA传输通道需要和PIX像素中断优先级保持一致
  *
  * @see      DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);
  *
  * @date     2020/4/8
*************************************************************************/

void DMA_CameraInitConfig(DMA_Channel_TypeDef* dma_ch,u32 srcStartAddr, u32 dstStartAddr, u32 lqsize)
{
    DMA_InitTypeDef DMA_InitStructure;

    if(DMA1_Channel1 == dma_ch || DMA1_Channel2 == dma_ch || DMA1_Channel3 == dma_ch || DMA1_Channel4 == dma_ch ||
       DMA1_Channel5 == dma_ch || DMA1_Channel6 == dma_ch || DMA1_Channel7 == dma_ch)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1总线初始化
    }
    else if(DMA2_Channel1 == dma_ch || DMA2_Channel2 == dma_ch || DMA2_Channel3 == dma_ch ||  DMA2_Channel4 == dma_ch ||
            DMA2_Channel5 == dma_ch)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//DMA2总线初始化
    }

    DMA_DeInit(dma_ch);

    //MDA配置初始化
    DMA_InitStructure.DMA_PeripheralBaseAddr = srcStartAddr;                    //源地址
    DMA_InitStructure.DMA_MemoryBaseAddr = dstStartAddr;                        //目标地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      //外设作为源
    DMA_InitStructure.DMA_BufferSize = lqsize;                                //传输多少个数据
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设地址不增加
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //内存地址依次+1
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设每次传输一个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //内存每次传输一个字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //非循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 //优先级最高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            //非内存到内存模式
    DMA_Init(dma_ch, &DMA_InitStructure);

    DMA_ITConfig(dma_ch, DMA_IT_TC, ENABLE);                                //配置DMA传输完成中断
    DMA_Cmd(dma_ch, ENABLE);                                                //开启DMA1

}

/*************************************************************************
//  @brief      摄像头GPIO触发定时器初始化
//  @param      timern              定时器通道
//  @param      pin                 引脚号
  * @return   无
  * @see      DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);
  *
  * @date     2020/4/8
  * 修改触发管脚，需要修改相应的额中断配置等。
*************************************************************************/
void DMA_CameraTriggerTimerInit(TIMER_Name_t timern, GPIO_Name_t pin)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    if(TIMER_1 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    else if(TIMER_2 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(TIMER_3 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(TIMER_4 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    PIN_InitConfig(pin, GPI, 0, GPIO_Mode_IN_FLOATING);

    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit((TIM_TypeDef*)(TIMERx[timern]), &TIM_TimeBaseStructure);

    TIM_ETRClockMode1Config((TIM_TypeDef*)(TIMERx[timern]), TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x00);
    TIM_SelectInputTrigger((TIM_TypeDef*)(TIMERx[timern]), TIM_TS_ETRF);
    TIM_SelectSlaveMode((TIM_TypeDef*)(TIMERx[timern]), TIM_SlaveMode_Reset);  //TIM从模式：触发信号的上升沿重新初始化计数器和触发寄存器的更新事件
    TIM_SelectMasterSlaveMode((TIM_TypeDef*)(TIMERx[timern]), TIM_MasterSlaveMode_Enable);//启动定时器的被动触发

    TIM_Cmd((TIM_TypeDef*)(TIMERx[timern]), ENABLE);
    TIM_DMACmd((TIM_TypeDef*)(TIMERx[timern]), TIM_DMA_Trigger, ENABLE);//使能TIM_DMA
}
