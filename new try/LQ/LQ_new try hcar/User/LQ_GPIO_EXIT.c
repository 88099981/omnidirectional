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
*  函数名称：void Reed_Init(void)
*  功能说明：干簧管GPIO及中断初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月21日
*  备    注：  GPIOA12
*          
*************************************************************************/
void Reed_Init(void)
{
    /*
	//初始化,底裤版
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);// 开启GPIOA口全部的时钟
    //PA12
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
    //设置成上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    //初始化GPIOA12
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //GPIOA12 中断线以及中断初始化配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
    EXTI_InitStructure.EXTI_Line=EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    //下降沿触发
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		                
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    EXTI_Init(&EXTI_InitStructure);						                        
    
    //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    //子优先级2
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    //使能外部中断通道//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器//
    NVIC_Init(&NVIC_InitStructure);
    */
    //封装版
    PIN_Exti(PA12, EXTI_Trigger_Falling,GPIO_Mode_IPU,3);
}

/*************************************************************************
*  函数名称：void Test_GPIO_LED(void)
*  功能说明：测试程序
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：核心板上的LED灯闪烁--四个LED同时闪烁
*************************************************************************/
void Test_GPIO_EXIT_Init(void)
 {
	GPIO_LED_Init();
	Reed_Init();
 	while(1)
 	{
 		LED_Ctrl(LED0,RVS);        //LED同时闪烁
 		delayms(200);                //延时等待
 	}
 }
