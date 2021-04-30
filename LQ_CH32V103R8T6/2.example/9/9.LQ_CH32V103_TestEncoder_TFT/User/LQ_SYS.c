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

/*******************************************************************************
* @name   : RCC_ConfigInit()
* @brief  : RCC configure
* @param  : void
* @retval : void
*******************************************************************************/
void RCC_ConfigInit(void)
{  
    RCC_APB2PeriphClockCmd(0x1FF,ENABLE);// ����GPIOA~F��ȫ����ʱ��
}
/*******************************************************************************
* @name   : InitSystick
* @brief  : Init Systick
* @param  : void
* @retval : void
*******************************************************************************/
static uint8_t  p_us=0;
static uint16_t p_ms=0;
u32 SystemCoreClock ;
/*******************************************************************************
* Function Name  : Delay_Init
* Description    : Initializes Delay Funcation.
* Input          : None
* Return         : None
*******************************************************************************/
void SysTick_Init(void)
{
    //SystemCoreClock = 8000000 * (((RCC->CFGR0 >> 18)&0x0F) + 2);
    //SystemCoreClock =SystemCoreClock;
    p_us=SystemCoreClock/8000000;
    p_ms=(uint16_t)p_us*1000;
    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CTLR = 1;
}

/*******************************************************************************
* Function Name  : Delay_Us
* Description    : Microsecond Delay Time.
* Input          : n��Microsecond number.
* Return         : None
*******************************************************************************/
void Delay_Us(uint32_t n)
{
    uint32_t i;

    SysTick->CTLR = 0;
    i = SystemCoreClock/8000000;

    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CTLR = 1;

  while((*(__IO uint32_t*)0xE000F004) <= i);

}

/*******************************************************************************
* Function Name  : Delay_Ms
* Description    : Millisecond Delay Time.
* Input          : n��Millisecond number.
* Return         : None
*******************************************************************************/
void Delay_Ms (uint32_t n)
{
    uint32_t i;

    SysTick->CTLR = 0;
    i = (uint32_t)n*p_ms;

    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CTLR = 1;

  while((*(__IO uint32_t*)0xE000F004) <= i);
}

void delayms (uint32_t n)
{
    uint32_t i;

    SysTick->CTLR = 0;
    i = (uint32_t)n*p_ms;

    SysTick->CNTL0 = 0;
    SysTick->CNTL1 = 0;
    SysTick->CNTL2 = 0;
    SysTick->CNTL3 = 0;
    SysTick->CTLR = 1;

  while((*(__IO uint32_t*)0xE000F004) <= i);
}
/*************************************************************************
*  �������ƣ�void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
*  ����˵�����жϳ�ʼ��
*  ����˵����//  @param      irqn              �жϺţ����Բ鿴CH32V10x.h�ļ��е�IRQn_Typeö���嶨��
            //  @param      pre_prior         ��ռ���ȼ�,ֵԽС���ȼ�Խ��
            //  @param      sub_prior         �����ȼ�,ֵԽС���ȼ�Խ��
            //  @param      status            ʹ�ܻ��߽�ֹ
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��    nvic_init(EXTI0_IRQn,1, 0, ENABLE); //�ⲿ�ж�1ʹ��,��ռ���ȼ��θߣ������ȼ���ߡ�
*************************************************************************/
void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irqn;                        //�жϺ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=pre_prior;   //��ռ���ȼ�,ֵԽС���ȼ�Խ��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prior;        //�����ȼ�,ֵԽС���ȼ�Խ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = status;                   //ʹ��
    NVIC_Init(&NVIC_InitStructure);
}
