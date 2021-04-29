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
使用说明：
本教学演示程序适用于电磁四轮或者三轮车：
整车资源为：
模块：龙邱TC264DA核心板、配套母板、双路全桥电机驱动、双编码器512带方向、TFT1.8屏幕、单舵机、四路电感模块；
车模：三轮或者四轮均可；
电感分布：
||----------左------------------------------------右--------------||
||---------侧--------------------------------------侧-------------||
||--------第----------------------------------------第------------||
||-------1----左侧第2个电感 -------------右侧第2个电感 -----1-----------||
||------个--------------------------------------------个----------||
||-----电----------------------------------------------电---------||
||----感------------------------------------------------感--------||
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

int16_t TempAngle_F = 0;        // 根据电感偏移量计算出的临时打角值
int16_t TempAngle_B = 0;        // 根据电感偏移量计算出的临时打角值
int16_t LastAngle = 0;        // 记忆冲出赛道前的有效偏移方向
int16_t TempAngle_L = 0;        // 根据电感偏移量计算出的临时打角值
int16_t TempAngle_R = 0;        // 根据电感偏移量计算出的临时打角值

int16_t BackOffset = 0;
int16_t FrontOffset = 0;      // 根据电感偏移量计算出的临时打角值

int16_t MotorDuty_FL=0;
int16_t MotorDuty_FR=0;
int16_t MotorDuty_BL=0;
int16_t MotorDuty_BR=0;

 //Front Back Vertical Parallel
int16_t LleftFP = 0, LleftFV = 0, LrightFV = 0, LrightFP = 0;  // 电感偏移量
int16_t LleftBP = 0, LleftBV = 0, LrightBV = 0, LrightBP = 0;  // 电感偏移量
int16_t LnowADC[8];           // 电感当前ADC数值

int16_t ad_max[8] = {2500, 2300, 999, 999, 2500, 2500, 2500, 2500}; // 新板子放到赛道标定最大值,会被程序刷新
int16_t ad_min[8] = { 120,  120, 120, 120,  120,  120,  120,  120}; // 新板子据需要标定最小值,会被程序刷新

u8 CircleNumber = 1;    // 入环次数，0结束；默认1次 ;环的个数一般在比赛前测试赛道时就知道了
u8 TangentPoint = 1;    // 切点判断   0切点结束；默认1可以入环，读取脉冲为入环准备
u8 EnterCircle = 0;     // 允许进环  默认 0不可进环；1可以进环
u8 OutCircle = 0;       // 允许出环   默认0不可出环；1可以出环
u8 LeftRightCircle = 0; // 左侧环还是右侧环 默认0原始；1左环；2右环

u8 Left_fork=0;
u8 Right_fork=0;
int32_t TangentPointpulse = 0; // 脉冲记忆临时变量1
int32_t EnterCirclePulse = 0;  // 脉冲记忆临时变量2
int32_t OutCirclePulse = 0;    // 脉冲记忆临时变量3
int32_t EnterCircleOKPulse = 0;// 脉冲记忆临时变量4
int16_t LowSpeed = 0;          // 圆环/十字口减速

u16 MagneticField = 0;     // 磁场强度    magnetic field intensity,判断圆环是否出现
int16_t OffsetDelta = 0;
extern pid_param_t pid1;
extern int16_t ECPULSE1, ECPULSE2,ECPULSE3, ECPULSE4;

/*************************************************************************
*  函数名称：void InductorInit (void)
*  功能说明：四个电感ADC初始化函数；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
void InductorInit (void)
{
  ADCInit(ADC_Channel_0);
  ADCInit(ADC_Channel_1);
  ADCInit(ADC_Channel_4);
  ADCInit(ADC_Channel_5);
  ADCInit(ADC_Channel_6);
  ADCInit(ADC_Channel_7);
  ADCInit(ADC_Channel_8);
  ADCInit(ADC_Channel_9);
  ADCInit(ADC_Channel_10);  
}
/*************************************************************************
*  函数名称：void InductorNormal(void)
*  功能说明：采集电感电压并归一化；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：   注意要先标定运放的放大倍数，尽量四个一致或者接近
*************************************************************************/
void InductorNormal (void)
{
  LnowADC[0] = ADC_Read(0);  // 左前侧第1个电感，与赛道平行，采集各个电感的AD值
  LnowADC[1] = ADC_Read(1);  // 左前侧第2个电感，与垂直赛道，
  LnowADC[2] = ADC_Read(4);  // 右前侧第2个电感，与垂直赛道，
  LnowADC[3] = ADC_Read(5);  // 右前侧第1个电感，与赛道平行，
  LnowADC[4] = ADC_Read(6);  // 左后侧第1个电感，与赛道平行，
  LnowADC[5] = ADC_Read(7);  // 左后侧第2个电感，与赛道垂直
  LnowADC[6] = ADC_Read(8);  // 右后侧第2个电感，与赛道垂直，
  LnowADC[7] = ADC_Read(9);  // 右后侧第1个电感，与赛道平行
  BatVolt    = ADC_Read(10);  // 刷新电池电压

  if (LnowADC[0] < ad_min[0])        ad_min[0] = LnowADC[0];     // 刷新最小值
  else if (LnowADC[0] > ad_max[0])   ad_max[0] = LnowADC[0];     // 刷新最大值

  if (LnowADC[1] < ad_min[1])        ad_min[1] = LnowADC[1];
  else if (LnowADC[1] > ad_max[1])   ad_max[1] = LnowADC[1];

  if (LnowADC[2] < ad_min[2])        ad_min[2] = LnowADC[2];
  else if (LnowADC[2] > ad_max[2])   ad_max[2] = LnowADC[2];

  if (LnowADC[3] < ad_min[3])        ad_min[3] = LnowADC[3];
  else if (LnowADC[3] > ad_max[3])   ad_max[3] = LnowADC[3];

  if (LnowADC[4] < ad_min[4])        ad_min[4] = LnowADC[4];
  else if (LnowADC[4] > ad_max[4])   ad_max[4] = LnowADC[4];

  if (LnowADC[5] < ad_min[5])        ad_min[5] = LnowADC[5];
  else if (LnowADC[5] > ad_max[5])   ad_max[5] = LnowADC[5];

  if (LnowADC[6] < ad_min[6])        ad_min[6] = LnowADC[6];
  else if (LnowADC[6] > ad_max[6])   ad_max[6] = LnowADC[6];

  if (LnowADC[7] < ad_min[7])        ad_min[7] = LnowADC[7];
  else if (LnowADC[7] > ad_max[7])   ad_max[7] = LnowADC[7];

  LleftFP  = (LnowADC[0] - ad_min[0]) * 100 / (ad_max[0] - ad_min[0]);     // 各偏移量归一化到0--100以内
  LleftFV  = (LnowADC[1] - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);
  LrightFV = (LnowADC[2] - ad_min[2]) * 100 / (ad_max[2] - ad_min[2]);
  LrightFP = (LnowADC[3] - ad_min[3]) * 100 / (ad_max[3] - ad_min[3]);
  
  LleftBP  = (LnowADC[4] - ad_min[4]) * 100 / (ad_max[4] - ad_min[4]);
  LleftBV  = (LnowADC[5] - ad_min[5]) * 100 / (ad_max[5] - ad_min[5]);
  LrightBV = (LnowADC[6] - ad_min[6]) * 100 / (ad_max[6] - ad_min[6]);
  LrightBP = (LnowADC[7] - ad_min[7]) * 100 / (ad_max[7] - ad_min[7]);
  
  MagneticField = LleftFP + LleftFV + LrightFV + LrightFP+LleftBP + LleftBV + LrightBV + LrightBP;// 磁场整体强度

  ECPULSE1 = -Read_Encoder(2)/10; //右电机 母板上编码器2，小车前进为正值
  ECPULSE2 = -Read_Encoder(4)/10; //左电机 母板上编码器1，小车前进为负值


  if((LleftBP+LleftBV>LleftFP+LleftFV)&&(LleftFV<5||LrightFV<5)&&(LleftFV>LrightFV))
  {
     // if(LeftRightCircle==0)
      //{
          MotorDuty_FR=MtTargetDuty/2;
          MotorDuty_FL=-MtTargetDuty/2;
          MotorDuty_BL=-MtTargetDuty/2;
          MotorDuty_BR=MtTargetDuty/2;
     // }
  }
  else if((LrightBV+LrightBP>LrightFV+LrightFP)&&(LleftFV<5||LrightFV<5)&&(LleftFV<LrightFV))
  {
     // if(LeftRightCircle==0)
      //{
          MotorDuty_FR=-MtTargetDuty/2;
          MotorDuty_FL=MtTargetDuty/2;
          MotorDuty_BL=MtTargetDuty/2;
          MotorDuty_BR=-MtTargetDuty/2;
   //   }
  }
  else if((LeftRightCircle==0)|| EnterCircle==1)
  {
          if((LleftFV < 30)&&(LrightFV < 30))
            {
                 TempAngle_F=(LleftFV-LrightFV)*4+(LleftFV-LrightFV)/5; //前轮差速
                 TempAngle_B=(LleftFV-LrightFV)*4+(LleftFV-LrightFV)/6; //后轮差速
                 MotorDuty_FR=MtTargetDuty+TempAngle_F*7/5-ECPULSE1/12;  //前右电机
                 MotorDuty_FL=MtTargetDuty-TempAngle_F*7/5-ECPULSE2/12;  //前左电机
                 MotorDuty_BL=MtTargetDuty-TempAngle_B*7/5-ECPULSE3/12;  //后左电机
                 MotorDuty_BR=MtTargetDuty+TempAngle_B*7/5-ECPULSE4/12;  //后右电机
            }
         else
            {
                 LowSpeed=800;
                 TempAngle_F=(LleftFV-LrightFV)*11+(LleftFV-LrightFV)/4; //前轮差速
                 TempAngle_B=(LleftFV-LrightFV)*11+(LleftFV-LrightFV)/5; //后轮差速
                 MotorDuty_FR=MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed;  //前右电机
                 MotorDuty_FL=MtTargetDuty-TempAngle_F*7/5-ECPULSE2 /10-LowSpeed;  //前左电机
                 MotorDuty_BL=MtTargetDuty-TempAngle_B*7/5-ECPULSE3 /10-LowSpeed;  //后左电机
                 MotorDuty_BR=MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed;  //后右电机
            }


  }

}

/*************************************************************************
 *  函数名称：void CircleDetect (void)
 *  功能说明：识别并进入圆环的个数；
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：
 *************************************************************************/
void CircleDetect (void)
{

}

/*************************************************************************
*  函数名称：void TFT_Show_EleMag_Info(void)
*  功能说明：显示各种所需信息
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
void TFT_Show_EleMag_Info(void)
{
  char txt[16] = "X:";
  uint16_t bat=0;
  // 调试信息
  sprintf(txt, "%04d %04d %03d ", FrontOffset, BackOffset,(int16_t)(pid1.out)); // 显示舵机角度数值，电机占空比数值，编码器数值
  TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);      // 字符串显示
  //显示各电感归一化后的偏移量  当前各电感电压值 各电感开机后历史最小值 各电感开机后历史最大值
  sprintf(txt, "%04d %04d %05d ", LleftFP, LleftBP,ECPULSE1);
  TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LleftFV, LleftBV,ECPULSE2);
  TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LrightFV, LrightBV,ECPULSE3);
  TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LrightFP, LrightBP,ECPULSE4);
  TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
  
  sprintf(txt, "Ring num: %d ", CircleNumber);
  TFTSPI_P8X16Str(2, 6, txt, u16GREEN, u16BLACK);
  sprintf(txt, "M:%03d Q:%d J:%d ", MagneticField, TangentPoint, EnterCircle);
  TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
  if (LeftRightCircle == 1)
    TFTSPI_P8X16Str(0, 8, "Left Ring ", u16WHITE, u16BLACK);
  else if (LeftRightCircle == 2)
    TFTSPI_P8X16Str(0, 8, "Right Ring", u16WHITE, u16BLACK);
  else
    TFTSPI_P8X16Str(0, 8, "No Ring   ", u16WHITE, u16BLACK);
  
  bat = BatVolt * 11 / 25;  // x/4095*3.3*100*5.7
  sprintf(txt, "B:%d.%02dV ", bat / 100, bat % 100);  // *3.3/4095*3
  TFTSPI_P8X16Str(0, 9, txt, u16PURPLE, u16BLACK);   // 字符串显示
}
/*************************************************************************
*  函数名称：void ElectroMagneticCar(void)
*  功能说明：电磁车双电机差速控制
-->1.入门算法：简单的分段比例控制算法，教学演示控制算法；
---2.进阶算法：PID典型应用控制算法，教学演示控制算法；
---3.高端算法：暂定改进粒子群协同控制算法；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年10月28日
*  备    注：驱动2个电机
*************************************************************************/
void ElectroMagneticMWCar (void)
{

  InductorInit();       // 电感器ADC初始化
  EncInit();            // 初始化两个编码器
  MotorInit(12500);     // 电机驱动的PWM为12.5Khz
  TIMER_InitConfig(TIMER_1, 10);// 读取PID定时中断10ms
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLACK);            // 清屏

  TFTSPI_P8X16Str(0,2,"PRESS K2 to show...",u16WHITE,u16BLACK);              //字符串显示
  while (1)
  {
    InductorNormal();           // 采集电感电压并并归一化；

    if (MagneticField < 20)     // 判断是否冲出赛道
      {
        MotorCtrl4w(0, 0, 0, 0);
      }
    else
    {
        MotorCtrl4w(MotorDuty_FR,MotorDuty_FL,MotorDuty_BL,MotorDuty_BR);
    }


   if (KEY_Read (KEY2)==0)
    {// 连续显示赛道信息
           TFT_Show_EleMag_Info();// 电感相关信息
   } // key

  } // WHILE(1)
} // MAIN()

