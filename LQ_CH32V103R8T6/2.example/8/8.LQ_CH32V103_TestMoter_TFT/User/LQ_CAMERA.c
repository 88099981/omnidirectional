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
CH32V103R8T6有7个通道的中断：0，1，2，3，4，5-9，10-15
MM32SPIN27PS有3个通道的中断：0-1，2-3，4-15
________________________________________________________________
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
摄像头接口                  龙邱神眼或者OV7725模块
● 数据端口：PC8-PC15口，共8位，接摄像头的数据端口；
● 时钟像素：外部中断PA2；
● 场信号：  外部中断PB4
● I2C：     PB8 I2C1-SCL
PB9 I2C1-SDA
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"


//* 图像原始数据存放 //
unsigned char Image_Data[LCDH+2][LCDW];

//* 二值化后用于OLED显示的数据 //
unsigned char Bin_Image[LCDH+2][LCDW];

unsigned char Camera_Flag=0;
int16_t OFFSET0 = 0;      //最远处，赛道中心值综合偏移量
int16_t TXV = 0;          //梯形的左高度，右高度
u8 hang= 0;
u8 hangcnt= 0;
u8 lie= 0;

/*************************************************************************
*  函数名称：void Test_CAMERA (void)
*  功能说明：摄像头测试例程
*  参数说明：void
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：注意需要使用  带与非门版（白色）转接座，或者新版摄像头模块
*************************************************************************/
void Test_CAMERA (void)
{
#ifdef USEOLED
  OLED_Init();
  OLED_CLS();                   //LCD清屏
#elif defined USETFT
  TFTSPI_Init(1);               //TFT1.8初始化0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);          //清屏
#endif
  
  // 摄像头初始化 //
  CAMERA_Init(50);
  
  while (1)
  {
    //if (Camera_Flag == 1)
    {
      //上报数据给上位机 串口速度比较慢 注意上位机图像宽高设置为120*188
      //CAMERA_Reprot();
      
      // 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据 //
      Camera_Flag = 0;
      
#ifdef USEOLED
      
      // 二值化 //
      Get_Bin_Image(2);
      
      // OLED动态显示摄像头图像
      OLED_Road(LCDH, LCDW, (unsigned char *)Bin_Image);
#elif defined USETFT
#if 0       //显示原始图像
      //TFT1.8动态显示摄像头图像
      TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
      
#else       //显示二值化图像
      
      // 二值化 //
      Get_Bin_Image(0);
      
      // 显示摄像头图像
      //TFTSPI_BinRoad((128-LCDW)/2, 0, LCDH, LCDW, (unsigned char *) Bin_Image);
      TFTSPI_BinRoad2((128-LCDW)/2, 0, LCDH, LCDW);
#endif
#endif
      LED_Ctrl(LED0, RVS);
    }
  }
}
/*************************************************************************
*  函数名称：void CAMERA_Reprot (void)
*  功能说明：串口上报上位机
*  参数说明：void
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：注意上位机的帧头可能有所区别
*************************************************************************/
void CAMERA_Reprot (void)
{
  short j, i;
  
  UART_PutChar(USART1,0xfe);  //帧头
  UART_PutChar(USART1,0xef);  //帧头
  
  for (i = 0; i < LCDH; i++)
  {
    for (j = 0; j < LCDW; j++)
    {
      if (Image_Data[i][j] == 0xfe)  //防止错误发送帧尾
      {
        Image_Data[i][j] = 0xff;
      }
      UART_PutChar(USART1,Image_Data[i][j]); //发送数据
    }
  }
  UART_PutChar(USART1,0xef);  //帧尾
  UART_PutChar(USART1,0xfe);  //帧尾
}

/*************************************************************************
* @brief    摄像头MT9V034场中断
* @date     2020/4/8
*************************************************************************/
void MT9V034_DMA_START(void)
{
  MT9V034_DMA_CH->CNTR = LCDH*MT9V034_IMAGEW;     //DMA待传数量，向下递减，可在DMA停止时改变。
  MT9V034_DMA_CH->CFGR |= DMA_CFGR1_EN;
}
/*************************************************************************
* @brief    MT9V03X摄像头DMA完成中断
* @note
* @see
* @date     2020/4/8
*************************************************************************/
void MT9V034_DMA_CLS(void)
{
  Camera_Flag = 1;//一副图像从采集开始到采集结束耗时3.8MS左右(50FPS、188*120分辨率)
  MT9V034_DMA_CH->CFGR &= (u16)(~DMA_CFGR1_EN);   //关闭DMA1
}
/*************************************************************************
*  函数名称：void CAMERA_Init (unsigned char fps)
*  功能说明：摄像头初始化
*  参数说明：fps:  帧率
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：
*     摄像头的一些参数，在LQ_MT9V034.c中的宏定义中修改
*  CAMERA_Init(50);   //初始化MT9V034  50帧 注意使用白色带与非门版转接座
*************************************************************************/
void CAMERA_Init (unsigned char fps)
{
  // DMA 触发源初始化
  // PIN_Exti(P00_4, PIN_IRQ_MODE_FALLING);
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  // 初始化摄像头 数据IO
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //使能GPIOA,GPIOB,GPIOC D时钟//
  RCC_APB2PeriphClockCmd(0x1FF,ENABLE);// 开启GPIOA~F口全部的时钟
  
  //PC
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  //设置成上拉输入//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  //初始化GPIOD//
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_4;
  //设置成上拉输入//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  //初始化GPIOD//
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  MT9V034_Init(fps);
  
  //GPIOB.4   中断线以及中断初始化配置//
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
  EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //下降沿触发//
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器//
  EXTI_Init(&EXTI_InitStructure);
  
  //使能按键所在的外部中断通道//
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  //子优先级2//
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  //使能外部中断通道//
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器//
  NVIC_Init(&NVIC_InitStructure);
  
  //
#ifdef USETIMER1DMA
  //摄像头DMA初始化
  DMA_CameraInitConfig(MT9V034_DMA_CH,(u32)((GPIOC->INDR)>>8), (u32)Image_Data[0], LCDH*MT9V034_IMAGEW);
  nvic_init(MT9V034_DMA_IRQN, 0, 1, ENABLE); //中断配置
  //GPIO触发定时器初始化,PLCK引脚初始化
  DMA_CameraTriggerTimerInit(TIMER_1, PA12); //GPIO触发TIM引脚禁止随意修改
  //VSYNC初始化 场中断初始化
  PIN_Exti(MT9V034_VSYNC_PIN, EXTI_Trigger_Falling, GPIO_Mode_IPU);
  nvic_init(MT9V034_VSYNC_IRQN, 0, 0, ENABLE);
#else
  //PC3行中断线以及中断初始化配置
  //GPIOC3
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
  
  EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //下降沿触发//
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器//
  EXTI_Init(&EXTI_InitStructure);
  
  //使能按键所在的外部中断通道//
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  //子优先级2//
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  //使能外部中断通道//
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器//
  NVIC_Init(&NVIC_InitStructure);
  
#endif
}

/************************************************************************
*  函数名称：void Get_Bin_Image (unsigned char mode)
*  功能说明：图像二值化到Bin_Image[][]
*  参数说明：mode  ：
*    0：使用大津法阈值
*    1：使用平均阈值
*    2: sobel 算子改进型  手动阈值，同时输出改为提取边沿的图像
*    3：sobel 算子改进型   动态阈值，同时输出改为提取边沿的图像
*  函数返回：无
*  修改时间：2020年10月28日
*  备    注：  Get_Bin_Image(0); //使用大津法二值化
************************************************************************/
void Get_Bin_Image (unsigned char mode)
{
  unsigned short i = 0, j = 0;
  unsigned short Threshold = 0;
  unsigned long tv = 0;
  char txt[16];
  
  if (mode == 0)
  {
    Threshold = GetOSTU(Image_Data);  //大津法阈值
  }
  if (mode == 1)
  {
    //累加
    for (i = 0; i < LCDH; i++)
    {
      for (j = 0; j < LCDW; j++)
      {
        tv += Image_Data[i][j];   //累加
      }
    }
    Threshold =(unsigned short)(tv / LCDH / LCDW);   //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
    Threshold = Threshold + 20;      //此处阈值设置，根据环境的光线来设定
  }
  else if (mode == 2)
  {
    Threshold = 80;                          //手动调节阈值
    lq_sobel(Image_Data, Bin_Image, (unsigned char) Threshold);
    
    return;
    
  }
  else if (mode == 3)
  {
    lq_sobelAutoThreshold(Image_Data, Bin_Image);  //动态调节阈值
    return;
  }
  //
#ifdef USEOLED
  sprintf(txt,"V:%03d",Threshold);//显示阈值
  OLED_P6x8Str(80,0,txt);
#else
  sprintf(txt, "V:%03d", Threshold);  //显示阈值
  TFTSPI_P6X8Str(8,8, txt, u16RED, u16BLUE);
#endif
  //
  // 二值化 //
  for (i = 0; i < LCDH; i++)
  {
    for (j = 0; j < LCDW; j++)
    {
      if (Image_Data[i][j] > Threshold) //数值越大，显示的内容越多，较浅的图像也能显示出来
        Bin_Image[i][j] = 1;
      else
        Bin_Image[i][j] = 0;
    }
  }
}
/************************************************************************
*  函数名称：short GetOSTU (unsigned char tmImage[LCDH][LCDW])
*  功能说明：大津法求阈值大小
*  参数说明：tmImage ： 图像数据
*  函数返回：无
*  修改时间：2011年10月28日
*  备    注：  GetOSTU(Image_Use);//大津法阈值
Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
3) i表示分类的阈值，也即一个灰度级，从0开始迭代 1
4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像
的比例w0，        并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背
景像素)  * 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
6) i++；转到4)，直到i为256时结束迭代
7) 将最大g相应的i值作为图像的全局阈值
缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
************************************************************************/
short GetOSTU (unsigned char tmImage[LCDH][LCDW])
{
  signed short i, j;
  unsigned long Amount = 0;
  unsigned long PixelBack = 0;
  unsigned long PixelshortegralBack = 0;
  unsigned long Pixelshortegral = 0;
  signed long PixelshortegralFore = 0;
  signed long PixelFore = 0;
  float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
  signed short MinValue, MaxValue;
  signed short Threshold = 0;
  unsigned char HistoGram[256];              //
  
  for (j = 0; j < 256; j++)
    HistoGram[j] = 0; //初始化灰度直方图
  
  for (j = 0; j < LCDH; j++)
  {
    for (i = 0; i < LCDW; i++)
    {
      HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
    }
  }
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++);        //获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--); //获取最大灰度的值
  
  if (MaxValue == MinValue)
    return MaxValue;         // 图像中只有一个颜色
  if (MinValue + 1 == MaxValue)
    return MinValue;        // 图像中只有二个颜色
  
  for (j = MinValue; j <= MaxValue; j++)
    Amount += HistoGram[j];        //  像素总数
  
  Pixelshortegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    Pixelshortegral += HistoGram[j] * j;        //灰度值总数
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];     //前景像素点数
    PixelFore = Amount - PixelBack;           //背景像素点数
    OmegaBack = (float) PixelBack / Amount;   //前景像素百分比
    OmegaFore = (float) PixelFore / Amount;   //背景像素百分比
    PixelshortegralBack += HistoGram[j] * j;  //前景灰度值
    PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //背景灰度值
    MicroBack = (float) PixelshortegralBack / PixelBack;   //前景灰度百分比
    MicroFore = (float) PixelshortegralFore / PixelFore;   //背景灰度百分比
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);   //计算类间方差
    if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //返回最佳阈值;
}
/*************************************************************************
*  函数名称：lq_sobel
*  功能说明：基于soble边沿检测算子的一种边沿检测
* @param    imageIn    输入数组
*           imageOut   输出数组      保存的二值化后的边沿信息
*           Threshold  阈值
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：注意需要使用  带与非门版（白色）转接座，或者新版摄像头模块
*************************************************************************/
void lq_sobel (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold)
{
  //* 卷积核大小 //
  short KERNEL_SIZE = 3;
  short xStart = KERNEL_SIZE / 2;
  short xEnd = LCDW - KERNEL_SIZE / 2;
  short yStart = KERNEL_SIZE / 2;
  short yEnd = LCDH - KERNEL_SIZE / 2;
  short i, j, k;
  short temp[4];
  for (i = yStart; i < yEnd; i++)
  {
    for (j = xStart; j < xEnd; j++)
    {
      // 计算不同方向梯度幅值  //
      temp[0] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j + 1]     //{{-1, 0, 1},
        - (short) imageIn[i][j - 1] + (short) imageIn[i][j + 1]        // {-1, 0, 1},
          - (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j + 1];    // {-1, 0, 1}};
      
      temp[1] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j - 1]     //{{-1, -1, -1},
        - (short) imageIn[i - 1][j] + (short) imageIn[i + 1][j]       // { 0,  0,  0},
          - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];    // { 1,  1,  1}};
      
      temp[2] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j - 1]       //  0, -1, -1
        - (short) imageIn[i][j + 1] + (short) imageIn[i + 1][j]       //  1,  0, -1
          - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j - 1];    //  1,  1,  0
      
      temp[3] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j + 1]       // -1, -1,  0
        - (short) imageIn[i][j - 1] + (short) imageIn[i + 1][j]       // -1,  0,  1
          - (short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j + 1];    //  0,  1,  1
      
      temp[0] = abs(temp[0]);
      temp[1] = abs(temp[1]);
      temp[2] = abs(temp[2]);
      temp[3] = abs(temp[3]);
      
      // 找出梯度幅值最大值  //
      for (k = 1; k < 4; k++)
      {
        if (temp[0] < temp[k])
        {
          temp[0] = temp[k];
        }
      }
      
      if (temp[0] > Threshold)
      {
        imageOut[i][j] = 1;
      }
      else
      {
        imageOut[i][j] = 0;
      }
    }
  }
}

/*************************************************************************
*  函数名称：lq_sobel
*  功能说明：基于soble边沿检测算子的一种自动阈值边沿检测
* @param    imageIn    输入数组
*           imageOut   输出数组      保存的二值化后的边沿信息 *
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：注意需要使用  带与非门版（白色）转接座，或者新版摄像头模块
*************************************************************************/
void lq_sobelAutoThreshold (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW])
{
  //* 卷积核大小 //
  short KERNEL_SIZE = 3;
  short xStart = KERNEL_SIZE / 2;
  short xEnd   = LCDW - KERNEL_SIZE / 2;
  short yStart = KERNEL_SIZE / 2;
  short yEnd   = LCDH - KERNEL_SIZE / 2;
  short i, j, k;
  short temp[4];
  for (i = yStart; i < yEnd; i++)
  {
    for (j = xStart; j < xEnd; j++)
    {
      // 计算不同方向梯度幅值  //
      temp[0] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j + 1]     //{{-1, 0, 1},
        - (short) imageIn[i][j - 1] + (short) imageIn[i][j + 1]       // {-1, 0, 1},
          - (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j + 1];    // {-1, 0, 1}};
      
      temp[1] = -(short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j - 1]     //{{-1, -1, -1},
        - (short) imageIn[i - 1][j] + (short) imageIn[i + 1][j]       // { 0,  0,  0},
          - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j + 1];    // { 1,  1,  1}};
      
      temp[2] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j - 1]       //  0, -1, -1
        - (short) imageIn[i][j + 1] + (short) imageIn[i + 1][j]       //  1,  0, -1
          - (short) imageIn[i - 1][j + 1] + (short) imageIn[i + 1][j - 1];    //  1,  1,  0
      
      temp[3] = -(short) imageIn[i - 1][j] + (short) imageIn[i][j + 1]       // -1, -1,  0
        - (short) imageIn[i][j - 1] + (short) imageIn[i + 1][j]       // -1,  0,  1
          - (short) imageIn[i - 1][j - 1] + (short) imageIn[i + 1][j + 1];    //  0,  1,  1
      
      temp[0] = abs(temp[0]);
      temp[1] = abs(temp[1]);
      temp[2] = abs(temp[2]);
      temp[3] = abs(temp[3]);
      
      // 找出梯度幅值最大值  //
      for (k = 1; k < 4; k++)
      {
        if (temp[0] < temp[k])
        {
          temp[0] = temp[k];
        }
      }
      
      // 使用像素点邻域内像素点之和的一定比例    作为阈值  //
      temp[3] = (short) imageIn[i - 1][j - 1] + (short) imageIn[i - 1][j] + (short) imageIn[i - 1][j + 1]
        + (short) imageIn[i][j - 1] + (short) imageIn[i][j] + (short) imageIn[i][j + 1]
          + (short) imageIn[i + 1][j - 1] + (short) imageIn[i + 1][j] + (short) imageIn[i + 1][j + 1];
      
      if (temp[0] > temp[3] / 12.0f)
      {
        imageOut[i][j] = 1;
      }
      else
      {
        imageOut[i][j] = 0;
      }
      
    }
  }
}

/*---------------------------------------------------------------
【函    数】Bin_Image_Filter
【功    能】过滤噪点
【参    数】无
【返 回 值】无
【注意事项】
----------------------------------------------------------------*/
void Bin_Image_Filter (void)
{
  int16_t nr; //行
  int16_t nc; //列
  
  for (nr = 1; nr < LCDH - 1; nr++)
  {
    for (nc = 1; nc < LCDW - 1; nc = nc + 1)
    {
      if ((Bin_Image[nr][nc] == 0)
          && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] > 2))
      {
        Bin_Image[nr][nc] = 1;
      }
      else if ((Bin_Image[nr][nc] == 1)
               && (Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] < 2))
      {
        Bin_Image[nr][nc] = 0;
      }
    }
  }
}

/**************************************************************************
*                                                                          *
*  函数名称：Seek_Road(void)                                           *
*  功能说明：寻找白色区域偏差值                                            *
*  参数说明：无                                                            *
*  函数返回：值的大小                                                      *
*  修改时间：2017-07-16                                                    *
*  备    注：以中间为0，左侧减一，右侧加一，数值代表1的面积                *
*            计算区域从第一行开始到倒数第二行结束。                        *
*            如果面积为负数，数值越大说明越偏左边；                        *
*            如果面积为正数，数值越大说明越偏右边。                        *
**************************************************************************/
void Seek_Road (void)
{
  int16_t nr; //行
  int16_t nc; //列
  int16_t temp = 0; //临时数值
  //for(nr=1; nr<MAX_ROW-1; nr++)
  temp = 0;
  for (nr = 0; nr < 30; nr++)
  {
    for (nc = MAX_COL / 2; nc < MAX_COL; nc = nc + 1)
    {
      if (Bin_Image[nr][nc])
      {
        ++temp;
      }
    }
    for (nc = 0; nc < MAX_COL / 2; nc = nc + 1)
    {
      if (Bin_Image[nr][nc])
      {
        --temp;
      }
    }
  }
  OFFSET0 = temp;
  return;
}
