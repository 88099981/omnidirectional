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
=================================================================
 ________________________________________________________________
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
 *  备    注：
 CH32V103R8T6有4个通道的TIM：1，2，3，4
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。
extern int16_t ECPULSE3, ECPULSE4;
/*************************************************************************
*  函数名称：int Read_Encoder(u8 TIMX)
*  功能说明：读取编码器计数
*  参数说明：u8 TIMX定时器
*  函数返回：脉冲数量
*  修改时间：2020年3月10日
*  备    注：ECPULSE1 = Read_Encoder(1); //左电机 母板上编码器1，小车前进为负值
*************************************************************************/
int Read_Encoder(u8 TIMX)
{
  int Encoder_TIM;    
  switch(TIMX)
  {
  case 1:  Encoder_TIM= (short)TIM1 -> CNT;  TIM1 -> CNT=0;break;//默认定时器用
  case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;//默认编码器用
  case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;//默认电机用
  case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;//默认编码器用
  default:  Encoder_TIM=0;
  }
  return Encoder_TIM;
}

/*************************************************************************
*  函数名称：void EncInit (void)
*  功能说明：编码器初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个编码器
*************************************************************************/
void EncInit (void)
{    
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                            //使能AFIO复用功能模块时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE );

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB6,7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//输入，悬空
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PB6,7
  GPIO_Init( GPIOB, &GPIO_InitStructure);

  // GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
  //GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE );
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1;                                        //预分频器
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                      //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //TIM向上计数
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                //重复计数器
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);       //初始化结构体
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI2 , TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式
  TIM_Cmd(TIM4, ENABLE);                                //定时器使能

  GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //PA15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//输入，悬空
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //PB3
  GPIO_Init( GPIOB, &GPIO_InitStructure);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1;                                        //预分频器
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                      //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //TIM向上计数
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                //重复计数器
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);       //初始化结构体
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI2 , TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式
  TIM_Cmd(TIM2, ENABLE);                                //定时器使能
}

/*************************************************************************
*  函数名称：void TestEncoder(void)
*  功能说明：测试程序，OLED显示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：
*************************************************************************/
void TestEncoderOLED(void)
{
  char txt[32];
  OLED_Init();
  OLED_CLS();   //清屏
  OLED_P6x8Str(0, 0, "Test Encoder");      //字符串显示
  EncInit();
  while (1)
  {
    /* 获取编码器值 */
    ECPULSE1 = Read_Encoder(4); //左电机 母板上编码器1，小车前进为负值
    ECPULSE3 = Read_Encoder(2); //右电机 母板上编码器2，小车前进为正值
    
    sprintf(txt, "Enc1: %05d;", ECPULSE1);
    OLED_P6x8Str(0, 2, txt);       //字符串显示
    sprintf(txt, "Enc2: %05d;", ECPULSE3);
    OLED_P6x8Str(0, 3, txt);       //字符串显示
    

    //LED_Ctrl(LEDALL,RVS); //LED翻转闪烁
    //LED_Ctrl(LED0, RVS);        //电平翻转,LED闪烁
    //delayms(200);              //延时等待
  }
}
/*************************************************************************
*  函数名称：void TestEncoder(void)
*  功能说明：测试程序，TFT1.8显示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：
*************************************************************************/
void TestEncoderTFT(void)
{
  char txt[32];
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_P8X16Str(3, 2,"LQ encoder", u16PURPLE, u16BLACK);   // 字符串显示
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  EncInit();
  MotorInit(12500);
  MotorCtrl4w(1200,1200,1200,1200);
  while (1)
  {
    /* 获取编码器值 */

    ECPULSE1 = -Read_Encoder(2)/1; //右电机 母板上编码器2，小车前进为正值
    ECPULSE2 = -Read_Encoder(4)/1; //左电机 母板上编码器1，小车前进为负值
    
    sprintf(txt, "Enc1: %05d;", ECPULSE1);
    TFTSPI_P8X16Str(0, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf(txt, "Enc2: %05d;", ECPULSE2);
    TFTSPI_P8X16Str(0, 3, txt, u16PURPLE, u16BLACK);   // 字符串显示
    
    sprintf(txt, "Enc3: %05d;", ECPULSE3);
    TFTSPI_P8X16Str(0, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf(txt, "Enc4: %05d;", ECPULSE4);
    TFTSPI_P8X16Str(0, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示

   // LED_Ctrl(LED0, RVS);        //电平翻转,LED闪烁
   // delayms(200);              //延时等待
  }
}
