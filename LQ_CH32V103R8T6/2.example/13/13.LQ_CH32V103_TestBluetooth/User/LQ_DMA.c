/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��ZYF/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��5��10��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��Hightec4.9.3/Tasking6.3�����ϰ汾
��Target �� TC264DA
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void DMA1_Channel4_IRQHandler(void)
{
    if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC4))
    {
        DMA_ClearFlag(DMA1_FLAG_TC4);
        //MT9V034_DMA_CH->CFGR &= (u16)(~DMA_CFGR1_EN);   //�ر�DMA
        MT9V034_DMA_CLS();
    }
}

/*************************************************************************
  * @brief    ����ͷDMA�����ʼ��
  *
  * @param    srcStartAddr   ��Դ��ַ IO��ַ
  * @param    dstStartAddr   ��Ŀ�ĵ�ַ ����ͷ���ݻ�����
  * @param    channel        ��DMA����ͨ�� 0-47      ע�� DMA����ͨ����Ҫ��PIX�����ж����ȼ�����һ��
  * @param    lqsize                ���ݳ���
  * @return   ��
  *
  * @note     ע�� DMA����ͨ����Ҫ��PIX�����ж����ȼ�����һ��
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
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1���߳�ʼ��
    }
    else if(DMA2_Channel1 == dma_ch || DMA2_Channel2 == dma_ch || DMA2_Channel3 == dma_ch ||  DMA2_Channel4 == dma_ch ||
            DMA2_Channel5 == dma_ch)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//DMA2���߳�ʼ��
    }

    DMA_DeInit(dma_ch);

    //MDA���ó�ʼ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = srcStartAddr;                    //Դ��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = dstStartAddr;                        //Ŀ���ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      //������ΪԴ
    DMA_InitStructure.DMA_BufferSize = lqsize;                                //������ٸ�����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //�����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�ڴ��ַ����+1
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //����ÿ�δ���һ���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //�ڴ�ÿ�δ���һ���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //��ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 //���ȼ����
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            //���ڴ浽�ڴ�ģʽ
    DMA_Init(dma_ch, &DMA_InitStructure);

    DMA_ITConfig(dma_ch, DMA_IT_TC, ENABLE);                                //����DMA��������ж�
    DMA_Cmd(dma_ch, ENABLE);                                                //����DMA1

}

/*************************************************************************
//  @brief      ����ͷGPIO������ʱ����ʼ��
//  @param      timern              ��ʱ��ͨ��
//  @param      pin                 ���ź�
  * @return   ��
  * @see      DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);
  *
  * @date     2020/4/8
  * �޸Ĵ����ܽţ���Ҫ�޸���Ӧ�Ķ��ж����õȡ�
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
    TIM_SelectSlaveMode((TIM_TypeDef*)(TIMERx[timern]), TIM_SlaveMode_Reset);  //TIM��ģʽ�������źŵ����������³�ʼ���������ʹ����Ĵ����ĸ����¼�
    TIM_SelectMasterSlaveMode((TIM_TypeDef*)(TIMERx[timern]), TIM_MasterSlaveMode_Enable);//������ʱ���ı�������

    TIM_Cmd((TIM_TypeDef*)(TIMERx[timern]), ENABLE);
    TIM_DMACmd((TIM_TypeDef*)(TIMERx[timern]), TIM_DMA_Trigger, ENABLE);//ʹ��TIM_DMA
}
