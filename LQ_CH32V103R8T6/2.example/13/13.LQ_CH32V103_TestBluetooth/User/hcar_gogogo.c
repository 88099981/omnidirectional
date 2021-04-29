/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦全向三队 编写                   00**
**00             队员 ：陈诏栋 刘坤龙 石柳苗                                               00 **
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  go                           00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/
#include"include.h"
#ifdef DOUBLE_CORE_HOST
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////双机通信之主机，完成整体计算，插在右边卡槽
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 temp;
u8 num = 0;
u8 function;
u8 rev_data[4];
extern int16_t ECPULSE3, ECPULSE4;

void hcar_gogogo(void)//双机通信之主机，完成整体计算，插在右边卡槽
{

  // 系统及延时函数初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SysTick_Init();
  hcar_init();
  MotorInit(12500);

  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_P8X16Str(0,0,(unsigned char*)"Welcome, my lord",u16WHITE,u16BLACK);//字符串显示
  Delay_Ms(100);
  TFTSPI_CLS(u16BLUE);
  //TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);              //字符串显示
  //TFTSPI_CLS(u16BLACK);   //蓝色屏幕
  UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10); //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
  UART_PutStr(USART1," UART1 LongQiu \r\n"); //发送字符串到上位机

  TIMER_InitConfig(TIMER_1, 10);// 读取PID定时中断10ms
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  while(1)//不要两个单片机同时使用屏幕
  {
     encoder_print();
     // UART_PutChar(USART1,ECPULSE3);
     //UART_PutStr(USART1," UART1 LongQiu \r\n"); //发送字符串到上位机
  }
}

#elif  defined DOUBLE_CORE_SLAVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 num = 0;
u8 function;
u8 rev_data[4];
u8 temp;
void hcar_gogogo(void)//双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
{


  // 系统及延时函数初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SysTick_Init();
  hcar_init();
  //串口初始化为
  //UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10); //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
  //UART_PutStr(USART1," UART1 LongQiu \r\n"); //发送字符串到上位机


 //TFT屏幕初始化
 /* TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
  TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);//字符串显示
  TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);              //字符串显示*/



  //TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  //TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  // 摄像头初始化 //
  //CAMERA_Init(50);
  //Test_CAMERA(); //PASS,测试龙邱神眼摄像头主频升级到96M

  TIMER_InitConfig(TIMER_1, 10);// 读取PID定时中断10ms
  while(1)//不要两个单片机同时使用屏幕
  {

    //Get_Bin_Image(0);
    // 显示神眼摄像头图像
    //TFTSPI_BinRoad2((130-LCDW)/2, 0, LCDH, LCDW);
      int16_t tx=0;
      unsigned char sum = 0;

      tx = (int16_t)(ECPULSE4);//如果分辨率太大，会超过100的话，可以除以10，20...100

      UART_PutChar(USART2,0xAA);      //发数据帧头
      UART_PutChar(USART2,0x04);      //编码器4
      UART_PutChar(USART2,tx);        //低八位
      UART_PutChar(USART2,tx>>8);    //高八位
      UART_PutChar(USART2,0);
      UART_PutChar(USART2,0);
      sum = 0xAA + 0x04 + tx + (tx>>8); //求和校验
      UART_PutChar(USART2, sum);
      sum = 0;

      tx = (int16_t)(ECPULSE3);//如果分辨率太大，会超过100的话，可以除以10，20...100

      UART_PutChar(USART2,0xAA);      //发数据帧头
      UART_PutChar(USART2,0x02);      //编码器2
      UART_PutChar(USART2,tx);    //低八位
      UART_PutChar(USART2,tx>>8);    //高八位
      UART_PutChar(USART2,0);
      UART_PutChar(USART2,0);
      sum = 0xAA + 0x02 + tx + (tx>>8); //求和校验
      UART_PutChar(USART2, sum);
      sum = 0;

      LED_Ctrl(LEDALL,RVS);     //LED翻转闪烁


  }
}
#endif
