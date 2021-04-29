/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技CH32V103R8T6核心板
【编    写】chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.0 版权所有，单位使用请先联系授权
【最后更新】2020年12月24日，持续更新，请关注最新版！
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【IDE】CH32V103R8T6:MounRiver Studio及以上版本
【Target 】 CH32V103R8T6
【SYS PLL】 72-96-112MHz 频率太高可能无法启动system_ch32v10x.c
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
=================================================================
CH32V103第一核心板：
-----------------------------------------------------------------
默认电机接口
使用TIM3块，四个通道可产生4路PWM
双路接口2
PC9          TIM3CH4    龙邱双核母板MOTOR3_P
PC12         GPIO       龙邱双核母板MOTOR3_N
PC7          TIM3CH2    龙邱双核母板MOTOR4_P
PC13         GPIO       龙邱双核母板MOTOR4_N
双路接口1
PC8          TIM3CH3    龙邱双核母板MOTOR1_P
PC10         GPIO       龙邱双核母板MOTOR1_N
PC6          TIM3CH1    龙邱双核母板MOTOR2_P
PC11         GPIO       龙邱双核母板MOTOR2_N
-----------------------------------------------------------------
默认屏幕显示接口，用户可以使用TFT或者OLED模块
TFTSPI_CS     PB13      龙邱双核母板 CS管脚 默认拉低，可以不用
TFTSPI_SCK    PB15      龙邱双核母板 SPI SCK管脚
TFTSPI_SDI    PB14      龙邱双核母板 SPI MOSI管脚
TFTSPI_DC     PB12      龙邱双核母板 D/C管脚
TFTSPI_RST    PB11      龙邱双核母板 RESET管脚
-----------------------------------------------------------------
OLED_CK       PB13      龙邱双核母板OLED CK管脚
OLED_DI       PB15      龙邱双核母板OLED DI管脚
OLED_RST      PB14      龙邱双核母板OLED RST管脚
OLED_DC       PB12      龙邱双核母板OLED DC管脚
OLED_CS       PB11      龙邱双核母板OLED CS管脚 默认拉低，可以不用
----------------------------------------------------------------
默认按键接口
KEY0p         PC3       龙邱双核母板上按键0
KEY1p         PC2       龙邱双核母板上按键1
KEY2p         PC1       龙邱双核母板上按键2
DSW0p         PB4       龙邱双核母板上拨码开关0
DSW1p         PB5       龙邱双核母板上拨码开关1
-----------------------------------------------------------------
默认LED接口
LED0p         新版PC4/原始A3    龙邱SPIN27PS核心板上LED0 翠绿
LED1p         PB10      龙邱SPIN27PS核心板上LED0 翠绿
-----------------------------------------------------------------
默认蜂鸣器接口
BEEPp         PD2       龙邱双核母板上蜂鸣器接口
干簧管接口
REEDp         PB3       龙邱双核母板上干簧管接口
-----------------------------------------------------------------
CH32V103第二核心板：：

摄像头接口                  龙邱神眼或者OV7725模块
● 数据端口：PC8-PC15口，共8位，接摄像头的数据端口；
● 时钟像素：外部中断PB1；
● 行信号：  外部中断PC3/如果用DMA+TIMER1:PA12/如果用DMA+TIMER3:PD2
● 场信号：  外部中断PB4
● I2C：     PB8 I2C1-SCL
PB9 I2C1-SDA
-----------------------------------------------------------------
默认舵机接口
使用TIM3块，四个通道可产生4路PWM
PC6        龙邱双核母板舵机
-----------------------------------------------------------------
正交解码增量编码器信号采集，任意选择四路即可；
TIM2:
PA15 PB3   龙邱双核母板编码器1
TIM4
PB7 PB6    龙邱双核母板编码器2
-----------------------------------------------------------------
电感电压采集模块
推荐使用ADC0-1+4-9，共8路ADC，可以满足电磁车电感电压采集；
电源采集用ADC10:  CH32V103:C0 / SPIN27PS：B2;
-----------------------------------------------------------------

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////配置在"LQ_Config.h"文件中，不要两个单片机同时使用屏幕
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef DOUBLE_CORE_HOST
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////双机通信之主机，完成整体计算，插在右边卡槽
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 temp;
u8 num = 0;
u8 function;
u8 rev_data[4];
//extern int16_t ECPULSE3, ECPULSE4;

int main(void)//双机通信之主机，完成整体计算，插在右边卡槽
{

  // 系统及延时函数初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SysTick_Init();
  //LED KEY初始化
  GPIO_LED_Init();
  GPIO_KEY_Init();

  //TFT屏幕初始化
  TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
  TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLACK);//字符串显示
  TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);              //字符串显示
  TFTSPI_CLS(u16BLACK);   //蓝色屏幕
  //串口初始化为
  UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10); //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
  UART_PutStr(USART1," UART1 LongQiu \r\n"); //发送字符串到上位机

  UART_InitConfig(USART2,115200,UART2_TX_A2,UART2_RX_A3); //初始化串口2 波特率 115200 无奇偶校验 1停止位 使用管脚
  UART_PutStr(USART2," UART2 LongQiu \r\n");
  //UART_RX_IRQ_Config(USART2, ENABLE);           //开启串口2中断
  // 程序配套视频地址：https://space.bilibili.com/95313236
  // 以下测试函数，内建死循环，用户可调用所用模块的初始化及读写函数来实现自己的任务
  //________________________________________________________________________________
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
     Test_CAMERA();    //PASS，测试摄像头函数，核心板插在左侧位置，通过TFT1.8屏幕显示摄像头采集画面


  while(1)//不要两个单片机同时使用屏幕
  {

  }
}

#elif defined  DOUBLE_CORE_SLAVE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
u8 num = 0;
u8 function;
u8 rev_data[4];
u8 temp;
int main(void)//双机通信之从机，发送编码器和或摄像头算法结果数据，插在左边卡槽
{

  int16_t tx=0;
  unsigned char sum = 0;
  // 系统及延时函数初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SysTick_Init();
  //LED KEY初始化
  GPIO_LED_Init();
  GPIO_KEY_Init();
  
 //TFT屏幕初始化
 /* TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_Show_Logo(0,37);//显示龙邱LOGO
  TFTSPI_P16x16Str(0,0,(unsigned char*)"北京龙邱智能科技",u16RED,u16BLUE);//字符串显示
  TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);              //字符串显示*/

  //串口初始化为
  //UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10); //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
  //UART_PutStr(USART1," UART1 LongQiu \r\n"); //发送字符串到上位机
  UART_InitConfig(USART2,115200,UART2_TX_A2,UART2_RX_A3); //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
  //UART_PutStr(USART2," UART2 LongQiu \r\n");
  
  //TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  //TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  EncInit();     // 初始化两个编码器
  // 摄像头初始化 //
  //CAMERA_Init(50);
  //Test_CAMERA(); //PASS,测试龙邱神眼摄像头主频升级到96M
  //TIMER_InitConfig(TIMER_1, 10);// 读取PID定时中断10ms
  while(1)//不要两个单片机同时使用屏幕
  {

    //Get_Bin_Image(0);
    // 显示神眼摄像头图像
    //TFTSPI_BinRoad2((130-LCDW)/2, 0, LCDH, LCDW);


    tx = (int16_t)(Read_Encoder(4));//如果分辨率太大，会超过100的话，可以除以10，20...100

    UART_PutChar(USART2,0xAA);      //发数据帧头
    UART_PutChar(USART2,0x04);      //编码器4
    UART_PutChar(USART2,tx);        //低八位
    UART_PutChar(USART2,tx>>8);    //高八位
    UART_PutChar(USART2,0);
    UART_PutChar(USART2,0);
    sum = 0xAA + 0x04 + tx + (tx>>8); //求和校验
    UART_PutChar(USART2, sum);
    sum = 0;
    
    tx = (int16_t)(Read_Encoder(2));//如果分辨率太大，会超过100的话，可以除以10，20...100

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
    delayms(10);
  }
}
#endif
