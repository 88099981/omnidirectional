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
*  函数名称：void LED_Init(void)
*  功能说明：GPIO初始化函数 LED灯所用PB12、PA4、PC13和PC14初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void GPIO_LED_Init(void)
{
  /*
  // 底裤版
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  */
  //封装版
  PIN_InitConfig(PB10, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PA3, GPO, 1,GPIO_Mode_Out_PP);

}

/*************************************************************************
*  函数名称：void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
*  功能说明：LED控制
*  参数说明：LEDn_e LEDno编号,LEDs_e sta状态亮/灭/翻转
*  函数返回：按键状态，0/1
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
  switch(LEDno)
  {
  case LED0:
    if(sta==ON)        GPIO_ResetBits(LED0p);
    else if(sta==OFF)  GPIO_SetBits(LED0p);
    else if(sta==RVS) (GPIO_ReadOutputDataBit(LED0p))?(GPIO_ResetBits(LED0p)):(GPIO_SetBits(LED0p));
    break;
    
  case LED1:
    if(sta==ON)        GPIO_ResetBits(LED1p);
    else if(sta==OFF)  GPIO_SetBits(LED1p);
    else if(sta==RVS) (GPIO_ReadOutputDataBit(LED1p))?(GPIO_ResetBits(LED1p)):(GPIO_SetBits(LED1p));
    break;
    
  case LED2:
    if(sta==ON)        GPIO_ResetBits(LED2p);
    else if(sta==OFF)  GPIO_SetBits(LED2p);
    else if(sta==RVS) (GPIO_ReadOutputDataBit(LED2p))?(GPIO_ResetBits(LED2p)):(GPIO_SetBits(LED2p));
    break;
    
  case LED3:
    if(sta==ON)        GPIO_ResetBits(LED3p);
    else if(sta==OFF)  GPIO_SetBits(LED3p);
    else if(sta==RVS) (GPIO_ReadOutputDataBit(LED3p))?(GPIO_ResetBits(LED3p)):(GPIO_SetBits(LED3p));
    break;
  case LEDALL:
    if(sta==ON)
    {
      GPIO_ResetBits(LED0p);
      GPIO_ResetBits(LED1p);
      GPIO_ResetBits(LED2p);
      GPIO_ResetBits(LED3p);
    }
    else if(sta==OFF)
    {
      GPIO_SetBits(LED0p);
      GPIO_SetBits(LED1p);
      GPIO_SetBits(LED2p);
      GPIO_SetBits(LED3p);
    }
    else if(sta==RVS)
    {
      (GPIO_ReadOutputDataBit(LED0p))?(GPIO_ResetBits(LED0p)):(GPIO_SetBits(LED0p));
      (GPIO_ReadOutputDataBit(LED1p))?(GPIO_ResetBits(LED1p)):(GPIO_SetBits(LED1p));
      (GPIO_ReadOutputDataBit(LED2p))?(GPIO_ResetBits(LED2p)):(GPIO_SetBits(LED2p));
      (GPIO_ReadOutputDataBit(LED3p))?(GPIO_ResetBits(LED3p)):(GPIO_SetBits(LED3p));
    }
    break;
  default:
    break;
  }
}
/*************************************************************************
*  函数名称：void Test_GPIO_LED(void)
*  功能说明：测试程序
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：LED翻转
*************************************************************************/
void Test_GPIO_LED(void)
{
  GPIO_LED_Init();
  while(1)
  {
     LED_Ctrl(LEDALL,RVS);
     delayms(200);
  }
}


