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

/*******************************************************************************
* @name   : RCC_ConfigInit()
* @brief  : RCC configure
* @param  : void
* @retval : void
*******************************************************************************/
void RCC_ConfigInit(void)
{  
    RCC_APB2PeriphClockCmd(0x1FF,ENABLE);// 开启GPIOA~F口全部的时钟
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
* Input          : n：Microsecond number.
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
* Input          : n：Millisecond number.
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
*  函数名称：void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
*  功能说明：中断初始化
*  参数说明：//  @param      irqn              中断号，可以查看CH32V10x.h文件中的IRQn_Type枚举体定义
            //  @param      pre_prior         抢占优先级,值越小优先级越高
            //  @param      sub_prior         次优先级,值越小优先级越高
            //  @param      status            使能或者禁止
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：    nvic_init(EXTI0_IRQn,1, 0, ENABLE); //外部中断1使能,抢占优先级次高，次优先级最高。
*************************************************************************/
void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irqn;                        //中断号
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=pre_prior;   //抢占优先级,值越小优先级越高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prior;        //次优先级,值越小优先级越高
    NVIC_InitStructure.NVIC_IRQChannelCmd = status;                   //使能
    NVIC_Init(&NVIC_InitStructure);
}
