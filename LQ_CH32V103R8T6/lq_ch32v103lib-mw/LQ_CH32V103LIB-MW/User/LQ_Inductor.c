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
u8 Normal=0;

int32_t TangentPointpulse = 0; // 脉冲记忆临时变量1
int32_t EnterCirclePulse = 0;  // 脉冲记忆临时变量2
int32_t OutCirclePulse = 0;    // 脉冲记忆临时变量3
int32_t EnterCircleOKPulse = 0;// 脉冲记忆临时变量4
int16_t LowSpeed = 0;          // 圆环/十字口减速

u16 MagneticField = 0;     // 磁场强度    magnetic field intensity,判断圆环是否出现
u16 MagneticField_F=0;     // 前四个电感磁场总强度
u16 MagneticField_B=0;     // 四个电感磁场总强度
u16 run=1;
int16_t OffsetDelta = 0;
extern pid_param_t pid1;
extern int16_t ECPULSE1, ECPULSE2,ECPULSE3, ECPULSE4;
int ting=0;
//定义圆环标志位
int16_t Enter_leftcircle_flag=0,Enter_leftcircle=0;
int16_t Out_leftcircle_flag=0,Out_leftcircle=0;
int16_t Enter_Rightcircle_flag=0,Enter_Rightcircle=0;
int16_t Out_Rightcircle_flag=0,Out_Rightcircle=0;
int16_t Right_Noin=0,left_Noin=0;
int16_t left_circle=0,Right_circle=0;
int16_t circleing=0;
//定义三岔路口标志位
int16_t Enter_leftfork_flag=0,Enter_rightfork_flag=0;
int16_t Enter_leftfork=0,Enter_rightfork=0;
int16_t Out_leftfork_flag=0,Out_rightfork_flag=0;
int16_t Out_leftfork=0,Out_rightfork=0;
int forking=0;
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
  LnowADC[1] = ADC_Read(1);  // 左前侧第2个电感，与垂直赛道，采集各个电感的AD值
  LnowADC[2] = ADC_Read(4);  // 右前侧第2个电感，与垂直赛道，采集各个电感的AD值
  LnowADC[3] = ADC_Read(5);  // 右前侧第1个电感，与赛道平行，采集各个电感的AD值
  LnowADC[4] = ADC_Read(6);  // 左后侧第1个电感，与赛道平行，采集各个电感的AD值
  LnowADC[5] = ADC_Read(7);  // 左后侧第2个电感，与赛道垂直，采集各个电感的AD值
  LnowADC[6] = ADC_Read(8);  // 右后侧第2个电感，与赛道垂直，采集各个电感的AD值
  LnowADC[7] = ADC_Read(9);  // 右后侧第1个电感，与赛道平行，采集各个电感的AD值
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
  MagneticField_F=LleftFP + LleftFV + LrightFV + LrightFP; // 前四个电感磁场总强度
  MagneticField_B=LleftBP + LleftBV + LrightBV + LrightBP; // 后四个电感磁场总强度
  ECPULSE1 = -Read_Encoder(2)/10; //右电机 母板上编码器2
  ECPULSE2 = -Read_Encoder(4)/10; //左电机 母板上编码器1
}

/*************************************************************************
 *  函数名称：void determine (void)
 *  功能说明：识别元素标志位
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：识别各种元素标志位
 *************************************************************************/

void determine (void)
{
    if (MagneticField < 20)     // 判断是否冲出赛道
     {
       ting=1;
       run=0;
       Normal=0;
     }

    //正向行驶进入左三叉路口
    if(forking==0&&MagneticField_F<40&&MagneticField_B>80&&Out_leftfork_flag==0&&Enter_leftfork_flag==0)
    {
        Normal=0;
        run=0;
        ting=0;
        Enter_leftfork_flag=1;
     }
    if( Enter_leftfork_flag==1&&MagneticField_F>MagneticField_B&&120<MagneticField<160)
            {
                Enter_leftfork_flag=0;
                Enter_leftfork=1;
                run=0;
                Normal=0;
                ting=0;
            }
    //出三岔路口
    if(Enter_leftfork==1&&MagneticField>160&&MagneticField_B>MagneticField_F)
    {
        run=0;
        Enter_leftfork=0;
        ting=0;
        Out_leftfork_flag=1;
    }
   if(Out_leftfork_flag==1)
    {
        Normal=1;
        run=0;
        ting=0;
        Out_leftfork_flag=0;
    }
 }
/*************************************************************************
 *  函数名称：void control (void)
 *  功能说明：控制小车函数
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年11月18日
 *  备    注：通过识别到的标志位控制小车
 *************************************************************************/
void control (void)
{
    if(ting == 1)               //冲出赛道停车
     {
       MotorCtrl4w(0, 0, 0, 0);
       delayms(200);
     }
    //左岔路处理
    else if(Enter_leftfork_flag==1)                                 //识别岔路口，横着进入左岔路
     {
        // 根据电机差异自行调整四个轮速度
        MotorDuty_FR=3000;   //前右电机
        MotorDuty_FL=-3000;  //前左电机
        MotorDuty_BL=2800;   //后左电机
        MotorDuty_BR=-2800;  //后右电机
     }
    else if(Enter_leftfork==1&&Out_leftfork_flag==0)                //进入岔路口，改变姿态跑
    {

      if((MagneticField_B-MagneticField_F)>50)
       {
         MotorDuty_FR=-2400;
         MotorDuty_FL=-2400;
         MotorDuty_BL=-2400;
         MotorDuty_BR=-2400;

       }
      else if((MagneticField_F-MagneticField_B)>50)
       {
         MotorDuty_FR=2400;
         MotorDuty_FL=2400;
         MotorDuty_BL=2400;
         MotorDuty_BR=2400;
       }
      else
       {
         if(LleftFP>LleftBP)
          {
            TempAngle_F=(LleftBP-LleftFP)*4+(LleftBP-LleftFP)/4; //前轮差速
            TempAngle_B=(LleftBP-LleftFP)*4+(LleftBP-LleftFP)/4; //后轮差速
            MotorDuty_FR=  MtTargetDuty+TempAngle_F*8/5-ECPULSE1 /10+600;   //前右电机
            MotorDuty_FL=-(MtTargetDuty+TempAngle_B*8/5-ECPULSE2 /10+600);  //前左电机
            MotorDuty_BL=  MtTargetDuty-TempAngle_B*7/5-ECPULSE3 /10+300;   //后左电机
            MotorDuty_BR=-(MtTargetDuty-TempAngle_F*7/5-ECPULSE4 /10+300);  //后右电机
          }
         else if(LleftFP<LleftBP)
          {
            TempAngle_F=(LleftBP-LleftFP)*4+(LleftBP-LleftFP)/4; //前轮差速
            TempAngle_B=(LleftBP-LleftFP)*4+(LleftBP-LleftFP)/4; //后轮差速
            MotorDuty_FR=  MtTargetDuty+TempAngle_F*8/5-ECPULSE1 /10+600;   //前右电机
            MotorDuty_FL=-(MtTargetDuty+TempAngle_B*8/5-ECPULSE2 /10+600);  //前左电机
            MotorDuty_BL=  MtTargetDuty-TempAngle_B*7/5-ECPULSE3 /10+300;   //后左电机
            MotorDuty_BR=-(MtTargetDuty-TempAngle_F*7/5-ECPULSE4 /10+300);  //后右电机
          }
       }
    }
    else if(run==1)                                        //正向行驶
     {
        Normal=0;
        if((LleftFV-LrightFV)>50)
        {
            MotorDuty_FR= 2200;
            MotorDuty_FL=-2200;
            MotorDuty_BL=-2200;
            MotorDuty_BR= 2200;
            LastAngle=1;

        }
        else if((LrightFV-LleftFV)>50)
        {
            MotorDuty_FR=-2200;
            MotorDuty_FL= 2200;
            MotorDuty_BL= 2200;
            MotorDuty_BR=-2200;

        }
        else if((LleftFV < 30)&&(LrightFV < 30))
        {
          LowSpeed=0;
          TempAngle_F=(LleftFV-LrightFV)*6+(LleftFV-LrightFV)/6; //前轮差速
          TempAngle_B=(LleftFV-LrightFV)*6+(LleftFV-LrightFV)/6; //后轮差速
          MotorDuty_FR=MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed;  //前右电机
          MotorDuty_FL=MtTargetDuty-TempAngle_F*8/5-ECPULSE2 /10-LowSpeed;  //前左电机
          MotorDuty_BL=MtTargetDuty-TempAngle_B*8/5-ECPULSE3 /10-LowSpeed;  //后左电机
          MotorDuty_BR=MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed;  //后右电机
        }
       else
        {
          LowSpeed=800;
          TempAngle_F=(LleftFV-LrightFV)*10+(LleftFV-LrightFV)/7; //前轮差速
          TempAngle_B=(LleftFV-LrightFV)*10+(LleftFV-LrightFV)/7; //后轮差速
          MotorDuty_FR=MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed;  //前右电机
          MotorDuty_FL=MtTargetDuty-TempAngle_F*8/5-ECPULSE2 /10-LowSpeed;  //前左电机
          MotorDuty_BL=MtTargetDuty-TempAngle_B*8/5-ECPULSE3 /10-LowSpeed;  //后左电机
          MotorDuty_BR=MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed;  //后右电机
       }
    }

    else if(Normal==1)                                     //反向行驶
         {
            run=0;
            Enter_leftfork=0;
            if((LleftBV-LrightBV)>50)
            {
                MotorDuty_FR= 2200;
                MotorDuty_FL=-2200;
                MotorDuty_BL= 2200;
                MotorDuty_BR=-2200;
                LastAngle=1;
            }
            else if((LrightBV-LleftBV)>50)
            {
                MotorDuty_FR=-2200;
                MotorDuty_FL= 2200;
                MotorDuty_BL=-2200;
                MotorDuty_BR= 2200;
            }
            if((LleftBV < 30)&&(LrightBV < 30))
            {
              LowSpeed=0;
              TempAngle_F=(LleftBV-LrightBV)*6+(LleftBV-LrightBV)/6; //前轮差速
              TempAngle_B=(LleftBV-LrightBV)*6+(LleftBV-LrightBV)/6; //后轮差速
              MotorDuty_FR=-(MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed);  //前右电机
              MotorDuty_FL=-(MtTargetDuty-TempAngle_F*8/5-ECPULSE2 /10-LowSpeed);  //前左电机
              MotorDuty_BL=-(MtTargetDuty-TempAngle_B*8/5-ECPULSE3 /10-LowSpeed);  //后左电机
              MotorDuty_BR=-(MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed);  //后右电机
            }
           else
            {
              LowSpeed=800;
              TempAngle_F=(LleftBV-LrightBV)*10+(LleftBV-LrightBV)/7; //前轮差速
              TempAngle_B=(LleftBV-LrightBV)*10+(LleftBV-LrightBV)/7; //后轮差速
              MotorDuty_FR=-(MtTargetDuty+TempAngle_F*7/5-ECPULSE1 /10-LowSpeed);  //前右电机
              MotorDuty_FL=-(MtTargetDuty-TempAngle_F*8/5-ECPULSE2 /10-LowSpeed);  //前左电机
              MotorDuty_BL=-(MtTargetDuty-TempAngle_B*8/5-ECPULSE3 /10-LowSpeed);  //后左电机
              MotorDuty_BR=-(MtTargetDuty+TempAngle_B*7/5-ECPULSE4 /10-LowSpeed);  //后右电机
           }
        }
    else
    {
     ting=0;
    }

}
/*************************************************************************
*  函数名称：void Bluetooth(void)
*  功能说明：蓝牙上位机显示函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年10月28日
*  备    注：通过蓝牙上位机打印车运行状态
*************************************************************************/
void Bluetooth(void)
{
    if(ting == 1)
    {
      UART_PutStr(USART1,"停车 \r\n ");      //发送标志到上位机
      delayms(1000);
    }
    else if(run==1)
    {
      UART_PutStr(USART1,"正向 \r\n ");      //发送标志到上位机
    }
    else if(Normal==1)
    {
      UART_PutStr(USART1,"反向 \r\n ");      //发送标志到上位机
    }
    else if(Enter_leftfork_flag==1)
    {
        UART_PutStr(USART1,"检测左岔路 \r\n ");      //发送标志到上位机
    }
    else if(Enter_leftfork==1)
    {
        UART_PutStr(USART1,"进入左岔路 \r\n ");      //发送标志到上位机
    }
    else if(Out_leftfork_flag==1)
    {
        UART_PutStr(USART1,"判断出岔路 \r\n ");      //发送标志到上位机
    }
    else if(Out_leftfork==1)
    {
        UART_PutStr(USART1,"出岔路 \r\n ");      //发送标志到上位机
    }
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
  // 调试信息
  sprintf(txt, "%04d %04d %05d ", LleftFP, LleftBP,ECPULSE1);          //显示左1前电感值，左1后电感值，编码器1值（右前）
  TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LleftFV, LleftBV,ECPULSE2);          //显示左2前电感值，左2后电感值，编码器1值（左前）
  TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LrightFV, LrightBV,ECPULSE3);        //显示右2前电感值，右2后电感值，编码器1值（左后）
  TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
  sprintf(txt, "%04d %04d %05d ", LrightFP, LrightBP,ECPULSE4);        //显示右1前电感值，右1后电感值，编码器1值（右后）
  TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
  
  sprintf(txt, "M1: %d ", MagneticField_F);                            //显示前4个电感总值
  TFTSPI_P8X16Str(2, 6, txt, u16GREEN, u16BLACK);
  sprintf(txt, "M:%03d M2:%d", MagneticField, MagneticField_B);        //显示前后8个电感总值     显示后4个电感总值
  TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
  if (run == 1)
    TFTSPI_P8X16Str(0, 8, "run ", u16WHITE, u16BLACK);
  else if (Normal == 1)
    TFTSPI_P8X16Str(0, 8, "Normal", u16WHITE, u16BLACK);
  else
    TFTSPI_P8X16Str(0, 8, "ting ", u16WHITE, u16BLACK);
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
  InductorInit();                                               // 电感器ADC初始化
  EncInit();                                                    // 初始化两个编码器
  TIMER_InitConfig(TIMER_1, 10);                                // 读取PID定时中断10ms
  UART_InitConfig(USART1,115200,UART1_TX_A9,UART1_RX_A10);      //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
  TFTSPI_Init(1);                                               //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLACK);                                         // 清屏
  TFTSPI_P8X16Str(0,2,"PRESS K2 to show...",u16WHITE,u16BLACK); //字符串显示
  while (1)
  {
    InductorNormal();                                           // 采集电感电压并归一化；
    determine();                                                //识别元素标志位
    control();                                                  //识别到元素后改变姿态，控制小车函数
    Bluetooth();                                                //蓝牙传递到上位机
    MotorCtrl4w(MotorDuty_FR,MotorDuty_FL,MotorDuty_BL,MotorDuty_BR);//赋值
    //TFT_Show_EleMag_Info();// 电感相关信息
   if (KEY_Read (KEY2)==0)
    {// 连续显示赛道信息
      TFT_Show_EleMag_Info();// 电感相关信息
    }
  }
}

