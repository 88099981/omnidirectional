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

const u32 TIMERx[] = {TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM4_BASE};
int time=0;//测试定时器
int16_t ECPULSE1 = 0;          // 速度全局变量
int16_t ECPULSE2 = 0;          // 速度全局变量
int16_t ECPULSE3 = 0;          // 速度全局变量
int16_t ECPULSE4 = 0;          // 速度全局变量
volatile int32_t RAllPulse = 0;// 速度全局变量
extern volatile uint8_t Game_Over;    // 小车完成全部任务，停车
extern volatile int16_t targetSpeed;
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
//定时器1中断服务程序
void TIM1_UP_IRQHandler(void)
{ 	
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
    //用户程序    
    LED_Ctrl(LED1,RVS);//LED翻转闪烁
    //encoder_get();
    //SpeedCtrl(0,90,0);
    /* 根据角度信息  控制小车  */
      /*  if(Game_Over==0)
        {
          if(FrontOffset > 50)
          {
            SpeedCtrl(0, 0, targetSpeed*2/3);  // 顺时针转动
          }
          else if(FrontOffset < -50)
          {
            SpeedCtrl(0, 0, -targetSpeed*2/3); // 逆时针转动
          }
          else
          {
            SpeedCtrl(0, targetSpeed*2, 0);    //前进后退
            //SpeedCtrl((BackOffset+FrontOffset)/3, targetSpeed*2, (BackOffset-FrontOffset)/3);
          }

          //以下部分需要较大的电流，电池要给力才行
          if(((LleftFP + LleftFV +LleftBP + LleftBV)-(LrightFV + LrightFP+ LrightBV + LrightBP))>100)
          {
          if(BackOffset-FrontOffset > 100)
          SpeedCtrl(0, 0, targetSpeed);  // 顺时针转动
                else if(BackOffset-FrontOffset < -100)
          SpeedCtrl(0, 0, -targetSpeed); // 逆时针转动
        }
            else if((BackOffset+FrontOffset) > 100) // 左右偏移太大
          {
          if(BackOffset+FrontOffset > 100)
          SpeedCtrl(-targetSpeed, 0, 0);  // 向右平移
                else if(BackOffset+FrontOffset < -100)
          SpeedCtrl(targetSpeed, 0, 0); // 向左平移
        }
            else
          {
          SpeedCtrl(0, targetSpeed*2, 0); //
          //SpeedCtrl((BackOffset+FrontOffset)/3, targetSpeed*2, (BackOffset-FrontOffset)/3);
        }
        }*/

  }
}

/*************************************************************************
*  函数名称：void TIMER_InitConfig(TIMER_Name_t timern, u16 ms)
*  功能说明：电机PWM初始化
*  参数说明：
//  @param      timern      定时器通道
//  @param      ms          定时周期pch,PWM通道所对应的的定时器及管脚

*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：TIMER_InitConfig(TIMER_1, 5); 使用定时器1作为5ms一次的周期中断
*************************************************************************/
void TIMER_InitConfig(TIMER_Name_t timern, u16 ms)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

         if(TIMER_1 == timern)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);                //时钟使能
    else if(TIMER_2 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    else if(TIMER_3 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    else if(TIMER_4 == timern)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = ms<<1;                               //设置自动重装载寄存器周期
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/2000);           //设置为2Khz计数一次
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                        //重复计数器设置为0
    TIM_TimeBaseInit((TIM_TypeDef *)TIMERx[timern], &TIM_TimeBaseStructure);//初始化TIMx时基

    TIM_ITConfig((TIM_TypeDef *)TIMERx[timern],TIM_IT_Update,ENABLE );      //使能TIM中断

    //中断优先级设置
         if(TIMER_1 == timern)   nvic_init(TIM1_UP_IRQn, 0, 2, ENABLE);
    else if(TIMER_2 == timern)   nvic_init(TIM2_IRQn, 0, 2, ENABLE);
    else if(TIMER_3 == timern)   nvic_init(TIM3_IRQn, 0, 2, ENABLE);
    else if(TIMER_4 == timern)   nvic_init(TIM4_IRQn, 0, 2, ENABLE);

    TIM_Cmd((TIM_TypeDef *)TIMERx[timern], ENABLE);  //使能TIMx

}
