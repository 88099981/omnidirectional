/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�SPIN27PS/CH32V103R8T6���İ�
����    д��chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��IDE��CH32V103R8T6:MounRiver Studio�����ϰ汾
��IDE��MM32SPIN27PS:IAR7.8/MDK5.2�����ϰ汾
��Target �� SPIN27PS/CH32V103R8T6
��SYS PLL�� 80/96MHz
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
=================================================================
________________________________________________________________
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
*  ��    ע��
CH32V103R8T6��7��ͨ�����жϣ�0��1��2��3��4��5-9��10-15
MM32SPIN27PS��3��ͨ�����жϣ�0-1��2-3��4-15
________________________________________________________________
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
����ͷ�ӿ�                  �������ۻ���OV7725ģ��
�� ���ݶ˿ڣ�PC8-PC15�ڣ���8λ��������ͷ�����ݶ˿ڣ�
�� ʱ�����أ��ⲿ�ж�PA2��
�� ���źţ�  �ⲿ�ж�PB4
�� I2C��     PB8 I2C1-SCL
PB9 I2C1-SDA
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"


//* ͼ��ԭʼ���ݴ�� //
unsigned char Image_Data[LCDH+2][LCDW];

//* ��ֵ��������OLED��ʾ������ //
unsigned char Bin_Image[LCDH+2][LCDW];

unsigned char Camera_Flag=0;
int16_t OFFSET0 = 0;      //��Զ������������ֵ�ۺ�ƫ����
int16_t TXV = 0;          //���ε���߶ȣ��Ҹ߶�
u8 hang= 0;
u8 hangcnt= 0;
u8 lie= 0;

/*************************************************************************
*  �������ƣ�void Test_CAMERA (void)
*  ����˵��������ͷ��������
*  ����˵����void
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��ע����Ҫʹ��  ������Ű棨��ɫ��ת�����������°�����ͷģ��
*************************************************************************/
void Test_CAMERA (void)
{
#ifdef USEOLED
  OLED_Init();
  OLED_CLS();                   //LCD����
#elif defined USETFT
  TFTSPI_Init(1);               //TFT1.8��ʼ��0:����  1������
  TFTSPI_CLS(u16BLUE);          //����
#endif
  
  // ����ͷ��ʼ�� //
  CAMERA_Init(50);
  
  while (1)
  {
    //if (Camera_Flag == 1)
    {
      //�ϱ����ݸ���λ�� �����ٶȱȽ��� ע����λ��ͼ��������Ϊ120*188
      //CAMERA_Reprot();
      
      // �������ͷ�ɼ���ɱ�־λ  �����������򲻻��ٴβɼ����� //
      Camera_Flag = 0;
      
#ifdef USEOLED
      
      // ��ֵ�� //
      Get_Bin_Image(2);
      
      // OLED��̬��ʾ����ͷͼ��
      OLED_Road(LCDH, LCDW, (unsigned char *)Bin_Image);
#elif defined USETFT
#if 0       //��ʾԭʼͼ��
      //TFT1.8��̬��ʾ����ͷͼ��
      TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
      
#else       //��ʾ��ֵ��ͼ��
      
      // ��ֵ�� //
      Get_Bin_Image(0);
      
      // ��ʾ����ͷͼ��
      //TFTSPI_BinRoad((128-LCDW)/2, 0, LCDH, LCDW, (unsigned char *) Bin_Image);
      TFTSPI_BinRoad2((128-LCDW)/2, 0, LCDH, LCDW);
#endif
#endif
      LED_Ctrl(LED0, RVS);
    }
  }
}
/*************************************************************************
*  �������ƣ�void CAMERA_Reprot (void)
*  ����˵���������ϱ���λ��
*  ����˵����void
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��ע����λ����֡ͷ������������
*************************************************************************/
void CAMERA_Reprot (void)
{
  short j, i;
  
  UART_PutChar(USART1,0xfe);  //֡ͷ
  UART_PutChar(USART1,0xef);  //֡ͷ
  
  for (i = 0; i < LCDH; i++)
  {
    for (j = 0; j < LCDW; j++)
    {
      if (Image_Data[i][j] == 0xfe)  //��ֹ������֡β
      {
        Image_Data[i][j] = 0xff;
      }
      UART_PutChar(USART1,Image_Data[i][j]); //��������
    }
  }
  UART_PutChar(USART1,0xef);  //֡β
  UART_PutChar(USART1,0xfe);  //֡β
}

/*************************************************************************
* @brief    ����ͷMT9V034���ж�
* @date     2020/4/8
*************************************************************************/
void MT9V034_DMA_START(void)
{
  MT9V034_DMA_CH->CNTR = LCDH*MT9V034_IMAGEW;     //DMA�������������µݼ�������DMAֹͣʱ�ı䡣
  MT9V034_DMA_CH->CFGR |= DMA_CFGR1_EN;
}
/*************************************************************************
* @brief    MT9V03X����ͷDMA����ж�
* @note
* @see
* @date     2020/4/8
*************************************************************************/
void MT9V034_DMA_CLS(void)
{
  Camera_Flag = 1;//һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ3.8MS����(50FPS��188*120�ֱ���)
  MT9V034_DMA_CH->CFGR &= (u16)(~DMA_CFGR1_EN);   //�ر�DMA1
}
/*************************************************************************
*  �������ƣ�void CAMERA_Init (unsigned char fps)
*  ����˵��������ͷ��ʼ��
*  ����˵����fps:  ֡��
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*     ����ͷ��һЩ��������LQ_MT9V034.c�еĺ궨�����޸�
*  CAMERA_Init(50);   //��ʼ��MT9V034  50֡ ע��ʹ�ð�ɫ������Ű�ת����
*************************************************************************/
void CAMERA_Init (unsigned char fps)
{
  // DMA ����Դ��ʼ��
  // PIN_Exti(P00_4, PIN_IRQ_MODE_FALLING);
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  // ��ʼ������ͷ ����IO
  GPIO_InitTypeDef GPIO_InitStructure;
  
  //ʹ��GPIOA,GPIOB,GPIOC Dʱ��//
  RCC_APB2PeriphClockCmd(0x1FF,ENABLE);// ����GPIOA~F��ȫ����ʱ��
  
  //PC
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  //���ó���������//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  //��ʼ��GPIOD//
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_4;
  //���ó���������//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  //��ʼ��GPIOD//
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  MT9V034_Init(fps);
  
  //GPIOB.4   �ж����Լ��жϳ�ʼ������//
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
  EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //�½��ش���//
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���//
  EXTI_Init(&EXTI_InitStructure);
  
  //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��//
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  //�����ȼ�2//
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  //ʹ���ⲿ�ж�ͨ��//
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���//
  NVIC_Init(&NVIC_InitStructure);
  
  //
#ifdef USETIMER1DMA
  //����ͷDMA��ʼ��
  DMA_CameraInitConfig(MT9V034_DMA_CH,(u32)((GPIOC->INDR)>>8), (u32)Image_Data[0], LCDH*MT9V034_IMAGEW);
  nvic_init(MT9V034_DMA_IRQN, 0, 1, ENABLE); //�ж�����
  //GPIO������ʱ����ʼ��,PLCK���ų�ʼ��
  DMA_CameraTriggerTimerInit(TIMER_1, PA12); //GPIO����TIM���Ž�ֹ�����޸�
  //VSYNC��ʼ�� ���жϳ�ʼ��
  PIN_Exti(MT9V034_VSYNC_PIN, EXTI_Trigger_Falling, GPIO_Mode_IPU);
  nvic_init(MT9V034_VSYNC_IRQN, 0, 0, ENABLE);
#else
  //PC3���ж����Լ��жϳ�ʼ������
  //GPIOC3
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);
  
  EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //�½��ش���//
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���//
  EXTI_Init(&EXTI_InitStructure);
  
  //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��//
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  //�����ȼ�2//
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  //ʹ���ⲿ�ж�ͨ��//
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���//
  NVIC_Init(&NVIC_InitStructure);
  
#endif
}

/************************************************************************
*  �������ƣ�void Get_Bin_Image (unsigned char mode)
*  ����˵����ͼ���ֵ����Bin_Image[][]
*  ����˵����mode  ��
*    0��ʹ�ô����ֵ
*    1��ʹ��ƽ����ֵ
*    2: sobel ���ӸĽ���  �ֶ���ֵ��ͬʱ�����Ϊ��ȡ���ص�ͼ��
*    3��sobel ���ӸĽ���   ��̬��ֵ��ͬʱ�����Ϊ��ȡ���ص�ͼ��
*  �������أ���
*  �޸�ʱ�䣺2020��10��28��
*  ��    ע��  Get_Bin_Image(0); //ʹ�ô�򷨶�ֵ��
************************************************************************/
void Get_Bin_Image (unsigned char mode)
{
  unsigned short i = 0, j = 0;
  unsigned short Threshold = 0;
  unsigned long tv = 0;
  char txt[16];
  
  if (mode == 0)
  {
    Threshold = GetOSTU(Image_Data);  //�����ֵ
  }
  if (mode == 1)
  {
    //�ۼ�
    for (i = 0; i < LCDH; i++)
    {
      for (j = 0; j < LCDW; j++)
      {
        tv += Image_Data[i][j];   //�ۼ�
      }
    }
    Threshold =(unsigned short)(tv / LCDH / LCDW);   //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100
    Threshold = Threshold + 20;      //�˴���ֵ���ã����ݻ����Ĺ������趨
  }
  else if (mode == 2)
  {
    Threshold = 80;                          //�ֶ�������ֵ
    lq_sobel(Image_Data, Bin_Image, (unsigned char) Threshold);
    
    return;
    
  }
  else if (mode == 3)
  {
    lq_sobelAutoThreshold(Image_Data, Bin_Image);  //��̬������ֵ
    return;
  }
  //
#ifdef USEOLED
  sprintf(txt,"V:%03d",Threshold);//��ʾ��ֵ
  OLED_P6x8Str(80,0,txt);
#else
  sprintf(txt, "V:%03d", Threshold);  //��ʾ��ֵ
  TFTSPI_P6X8Str(8,8, txt, u16RED, u16BLUE);
#endif
  //
  // ��ֵ�� //
  for (i = 0; i < LCDH; i++)
  {
    for (j = 0; j < LCDW; j++)
    {
      if (Image_Data[i][j] > Threshold) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����
        Bin_Image[i][j] = 1;
      else
        Bin_Image[i][j] = 0;
    }
  }
}
/************************************************************************
*  �������ƣ�short GetOSTU (unsigned char tmImage[LCDH][LCDW])
*  ����˵�����������ֵ��С
*  ����˵����tmImage �� ͼ������
*  �������أ���
*  �޸�ʱ�䣺2011��10��28��
*  ��    ע��  GetOSTU(Image_Use);//�����ֵ
Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ���� 1
4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��
�ı���w0��        ��ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����
������)  * ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
6) i++��ת��4)��ֱ��iΪ256ʱ��������
7) �����g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
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
  float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
  signed short MinValue, MaxValue;
  signed short Threshold = 0;
  unsigned char HistoGram[256];              //
  
  for (j = 0; j < 256; j++)
    HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ
  
  for (j = 0; j < LCDH; j++)
  {
    for (i = 0; i < LCDW; i++)
    {
      HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    }
  }
  
  for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++);        //��ȡ��С�Ҷȵ�ֵ
  for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--); //��ȡ���Ҷȵ�ֵ
  
  if (MaxValue == MinValue)
    return MaxValue;         // ͼ����ֻ��һ����ɫ
  if (MinValue + 1 == MaxValue)
    return MinValue;        // ͼ����ֻ�ж�����ɫ
  
  for (j = MinValue; j <= MaxValue; j++)
    Amount += HistoGram[j];        //  ��������
  
  Pixelshortegral = 0;
  for (j = MinValue; j <= MaxValue; j++)
  {
    Pixelshortegral += HistoGram[j] * j;        //�Ҷ�ֵ����
  }
  SigmaB = -1;
  for (j = MinValue; j < MaxValue; j++)
  {
    PixelBack = PixelBack + HistoGram[j];     //ǰ�����ص���
    PixelFore = Amount - PixelBack;           //�������ص���
    OmegaBack = (float) PixelBack / Amount;   //ǰ�����ذٷֱ�
    OmegaFore = (float) PixelFore / Amount;   //�������ذٷֱ�
    PixelshortegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
    PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //�����Ҷ�ֵ
    MicroBack = (float) PixelshortegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
    MicroFore = (float) PixelshortegralFore / PixelFore;   //�����ҶȰٷֱ�
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);   //������䷽��
    if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
    {
      SigmaB = Sigma;
      Threshold = j;
    }
  }
  return Threshold;                        //���������ֵ;
}
/*************************************************************************
*  �������ƣ�lq_sobel
*  ����˵��������soble���ؼ�����ӵ�һ�ֱ��ؼ��
* @param    imageIn    ��������
*           imageOut   �������      ����Ķ�ֵ����ı�����Ϣ
*           Threshold  ��ֵ
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��ע����Ҫʹ��  ������Ű棨��ɫ��ת�����������°�����ͷģ��
*************************************************************************/
void lq_sobel (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold)
{
  //* ����˴�С //
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
      // ���㲻ͬ�����ݶȷ�ֵ  //
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
      
      // �ҳ��ݶȷ�ֵ���ֵ  //
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
*  �������ƣ�lq_sobel
*  ����˵��������soble���ؼ�����ӵ�һ���Զ���ֵ���ؼ��
* @param    imageIn    ��������
*           imageOut   �������      ����Ķ�ֵ����ı�����Ϣ *
*  �������أ�void
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��ע����Ҫʹ��  ������Ű棨��ɫ��ת�����������°�����ͷģ��
*************************************************************************/
void lq_sobelAutoThreshold (unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW])
{
  //* ����˴�С //
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
      // ���㲻ͬ�����ݶȷ�ֵ  //
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
      
      // �ҳ��ݶȷ�ֵ���ֵ  //
      for (k = 1; k < 4; k++)
      {
        if (temp[0] < temp[k])
        {
          temp[0] = temp[k];
        }
      }
      
      // ʹ�����ص����������ص�֮�͵�һ������    ��Ϊ��ֵ  //
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
����    ����Bin_Image_Filter
����    �ܡ��������
����    ������
���� �� ֵ����
��ע�����
----------------------------------------------------------------*/
void Bin_Image_Filter (void)
{
  int16_t nr; //��
  int16_t nc; //��
  
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
*  �������ƣ�Seek_Road(void)                                           *
*  ����˵����Ѱ�Ұ�ɫ����ƫ��ֵ                                            *
*  ����˵������                                                            *
*  �������أ�ֵ�Ĵ�С                                                      *
*  �޸�ʱ�䣺2017-07-16                                                    *
*  ��    ע�����м�Ϊ0������һ���Ҳ��һ����ֵ����1�����                *
*            ��������ӵ�һ�п�ʼ�������ڶ��н�����                        *
*            ������Ϊ��������ֵԽ��˵��Խƫ��ߣ�                        *
*            ������Ϊ��������ֵԽ��˵��Խƫ�ұߡ�                        *
**************************************************************************/
void Seek_Road (void)
{
  int16_t nr; //��
  int16_t nc; //��
  int16_t temp = 0; //��ʱ��ֵ
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
