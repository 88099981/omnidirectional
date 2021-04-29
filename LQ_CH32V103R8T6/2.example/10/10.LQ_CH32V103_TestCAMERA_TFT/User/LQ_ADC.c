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

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"
/********************************************************************************************************
**函数信息 ：void ADC1_SingleChannel(uint8_t ADC_Channel_x)      
**功能描述 ：配置ADC1单次转换模式
**输入参数 ：ADC_Channel_x , x为0~11
**输出参数 ：无
**CH32V103 ADC:
**ADC0 : A0
**ADC1 : A1
**ADC2 : A2
**ADC3 : A3
**ADC4 : A4
**ADC5 : A5
**ADC6 : A6
**ADC7 : A7
**ADC8 : B0
**ADC9 : B1
**ADC10: C0
**ADC11: C1
**ADC12: C2
**ADC13: C3
**ADC14: C4
**ADC15: C5
********************************************************************************************************/


extern int16_t LnowADC[8];           // 电感当前ADC数值


void ADCInit(u8 ADC_Channel_x)
{
  GPIO_InitTypeDef GPIO_InitStructure;    
  ADC_InitTypeDef  ADC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  
  switch (ADC_Channel_x)
  {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x;
    GPIO_Init(GPIOA, &GPIO_InitStructure);   
    break;
  case 8:   
  case 9:
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x-8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);    
    break;
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x-10;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    break;    
  default:
    break;      
  } 
  
  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode =   DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode =  DISABLE;//连续转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_x, 1, 1);
  
  if(ADC_Channel_x == ADC_Channel_14)
  {
      ADC_TempSensorVrefintCmd(ENABLE);
  }
  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
}

/********************************************************************************************************
**函数信息 ：ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)       
**功能描述 ：获取ADC1转换数据
**输入参数 ：ADC_Channel_x , x为0~11
**输出参数 ：puiADData为ADC读到的值
********************************************************************************************************/
u16 ADC_Read(uint8_t ch)
{
    //设置指定ADC的规则组通道，一个序列，采样时间
    ADC_RegularChannelConfig(ADC1,ch, 1, ADC_SampleTime_1Cycles5);  //ADC1,ADC通道,采样时间为1.5周期
    ADC1->CTLR2 |= CTLR2_EXTTRIG_SWSTART_Set;                               //使能指定的ADC1的软件转换启动功能
    while((ADC1->STATR & ADC_FLAG_EOC) == (uint8_t)RESET);                  //等待转换结束
    return ((ADC1->RDATAR));     // 分辨率12位
    //return ((ADC1->RDATAR)>>2);// 分辨率10位
    //return ((ADC1->RDATAR)>>4);// 分辨率8位
}

/********************************************************************************************************
**函数信息 ：Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)     
**功能描述 ：获取几次ADC1采样值的平均值
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：puiADData为ADC读到的值
********************************************************************************************************/
u16 ADC_Read_Average(uint8_t ADC_Channel_x,uint8_t times)
{
  u32 temp_val=0;
  u8 t;
  u8 delay;
  for(t=0;t<times;t++)
  {
    temp_val+=ADC_Read(ADC_Channel_x);
    for(delay=0;delay<100;delay++);
  }
  return temp_val/times;
} 



/*************************************************************************
*  函数名称：void TestADC(void)
*  功能说明：ADC采集测试
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：   ADC悬空不为零，随机的
*************************************************************************/
void Test_ADC(void)
{
 // u16 vbat;
  //char txt[16];
  
  ADCInit(ADC_Channel_0);
  ADCInit(ADC_Channel_1);
  ADCInit(ADC_Channel_4);
  ADCInit(ADC_Channel_5);
  ADCInit(ADC_Channel_6);
  ADCInit(ADC_Channel_7);
  ADCInit(ADC_Channel_8);
  ADCInit(ADC_Channel_9);
  //ADCInit(ADC_Channel_10);


  /*TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕	
  TFTSPI_P8X16Str(0,1,"LQ ADC demo",u16WHITE,u16BLACK);     //字符串显示*/


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

  // 各偏移量归一化到0--100以内
  LleftFP  = (LnowADC[0] - ad_min[0]) * 100 / (ad_max[0] - ad_min[0]);  //左前1
  LleftFV  = (LnowADC[1] - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);  //左前2
  LrightFV = (LnowADC[2] - ad_min[2]) * 100 / (ad_max[2] - ad_min[2]);  //右前2
  LrightFP = (LnowADC[3] - ad_min[3]) * 100 / (ad_max[3] - ad_min[3]);  //右前1

  LleftBP = (LnowADC[4] - ad_min[4]) * 100 / (ad_max[4] - ad_min[4]);   //左后1
  LleftBV = (LnowADC[5] - ad_min[5]) * 100 / (ad_max[5] - ad_min[5]);   //左后2
  LrightBV = (LnowADC[6] - ad_min[6]) * 100 / (ad_max[6] - ad_min[6]);  //右后2
  LrightBP = (LnowADC[7] - ad_min[7]) * 100 / (ad_max[7] - ad_min[7]);  //右后1


      //显示各电感归一化后的偏移量  当前各电感电压值 各电感开机后历史最小值 各电感开机后历史最大值


    /*vbat=ADC_Read(ADC_Channel_10);
    vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
    sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3
    UART_PutStr(USART1,txt);
    TFTSPI_P8X16Str(3, 3, txt, u16PURPLE, u16BLACK);   // 字符串显示
    vbat=ADC_Read(ADC_Channel_4);   
    vbat = vbat * 2 / 25;  // x/4095*3.3*100
    sprintf(txt, "ch4:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095
    TFTSPI_P8X16Str(3, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
    UART_PutStr(USART1,txt);
    vbat=ADC_Read(ADC_Channel_5);   
    vbat = vbat * 2 / 25;  // x/4095*3.3*100
    sprintf(txt, "ch5:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095
    TFTSPI_P8X16Str(3, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示
    UART_PutStr(USART1,txt);
    LED_Ctrl(LED0,RVS); 				                        
    delayms(500); */
     /*  sprintf(txt, "0:%04d %04d ", LleftFP, LleftBP);
       TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
       sprintf(txt, "1:%04d %04d  ", LleftFV, LleftBV);
       TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
       sprintf(txt, "2:%04d %04d  ", LrightFV, LrightBV);
       TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
       sprintf(txt, "3:%04d %04d  ", LrightFP, LrightBP);
       TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);*/
  
}



