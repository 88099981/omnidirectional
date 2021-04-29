/********************************** (C) COPYRIGHT *******************************
* File Name          : system_ch32v10x.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : CH32V10x Device Peripheral Access Layer System Source File.
*********************************************************************************/
#include "ch32v10x.h" 
#include "LQ_Config.h"

//#define SYSCLK_FREQ_72MHz  72000000
//#define SYSCLK_FREQ_88MHz  88000000
//#define SYSCLK_FREQ_96MHz  96000000
//#define SYSCLK_FREQ_104MHz  104000000
//#define SYSCLK_FREQ_112MHz  112000000
//#define SYSCLK_FREQ_120MHz  120000000
//#define SYSCLK_FREQ_128MHz  128000000 //太高，不能运行

#ifdef DOUBLE_CORE_HOST
    #ifdef USELQMT9V034
    uint8_t xxMHZ=96;//双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
    uint32_t SystemCoreClock=96000000;  /* System Clock Frequency (Core Clock) */
    #else
    uint8_t xxMHZ=80;
    uint32_t SystemCoreClock=80000000;  /* System Clock Frequency (Core Clock) */
    #endif
#else
uint8_t xxMHZ=80;
uint32_t SystemCoreClock=80000000;  /* System Clock Frequency (Core Clock) */
#endif
/******************************************************************************************
* Function Name  : SetSysClockTo88
* Description    : Sets System clock frequency to 88MHz and configure HCLK, PCLK2 and PCLK1 prescalers.
* Input          : None
* Return         : None
*******************************************************************************************/
static void SetSysClocktoXX(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
    uint32_t pre=RCC_PLLMULL11;

    if(xxMHZ==72)
        pre=RCC_PLLMULL9;
    else if(xxMHZ==80)
        pre=RCC_PLLMULL10;
    else if(xxMHZ==88)
        pre=RCC_PLLMULL11;
    else if(xxMHZ==96)
        pre=RCC_PLLMULL12;
    else if(xxMHZ==104)
        pre=RCC_PLLMULL13;
    else if(xxMHZ==112)
        pre=RCC_PLLMULL14;//最高不建议超过112M
    else if(xxMHZ==120)
        pre=RCC_PLLMULL15;
    else
        pre=RCC_PLLMULL11;

    RCC->CTLR |= ((uint32_t)RCC_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CTLR & RCC_HSERDY;
        StartUpCounter++;
    } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CTLR & RCC_HSERDY) != RESET)
    {
        HSEStatus = (uint32_t)0x01;
    }
    else
    {
        HSEStatus = (uint32_t)0x00;
    }

    if (HSEStatus == (uint32_t)0x01)
    {
        /* Enable Prefetch Buffer */
        FLASH->ACTLR |= FLASH_ACTLR_PRFTBE;

        /* Flash 2 wait state */
        FLASH->ACTLR &= (uint32_t)((uint32_t)~FLASH_ACTLR_LATENCY);
        FLASH->ACTLR |= (uint32_t)FLASH_ACTLR_LATENCY_2;

        /* HCLK = SYSCLK */
        RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
        if(xxMHZ>88)
        {
            /* PCLK2 = HCLK */
            RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV2;
            /* PCLK1 = HCLK */
            RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;
        }
        else
        {
            /* PCLK2 = HCLK */
            RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
            /* PCLK1 = HCLK */
            RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV1;
        }
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE|RCC_PLLMULL));
        /*  PLL configuration: PLLCLK = HSE * ?  */
        RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSE | pre);
        /* Enable PLL */
        RCC->CTLR |= RCC_PLLON;
        /* Wait till PLL is ready */
        while((RCC->CTLR & RCC_PLLRDY) == 0)
        {
        }
        /* Select PLL as system clock source */
        RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
        RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
        /* Wait till PLL is used as system clock source */
        while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
        {
        }
    }
    else
    {
    /*
     * If HSE fails to start-up, the application will have wrong clock
     * configuration. User can add here some code to deal with this error
     */
    }
    SystemCoreClock= xxMHZ*1000000;
    //SystemCoreClock= CRYSTAL_FREQ_8MHz * (((RCC->CFGR0 >> 18)&0x0F) + 2);
}

/******************************************************************************************
* Function Name  : SystemInit
* Description    : Setup the microcontroller system Initialize the Embedded Flash Interface,
*                  the PLL and update the SystemCoreClock variable.
* Input          : None
* Return         : None
*******************************************************************************************/
void SystemInit(void)
{
  RCC->CTLR |= (uint32_t)0x00000001;
  RCC->CFGR0 &= (uint32_t)0xF8FF0000;
  RCC->CTLR &= (uint32_t)0xFEF6FFFF;
  RCC->CTLR &= (uint32_t)0xFFFBFFFF;
  RCC->CFGR0 &= (uint32_t)0xFF80FFFF;
  RCC->INTR = 0x009F0000;
  SetSysClocktoXX();
}
