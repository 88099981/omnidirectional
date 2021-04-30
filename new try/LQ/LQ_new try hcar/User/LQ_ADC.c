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

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"
/********************************************************************************************************
**������Ϣ ��void ADC1_SingleChannel(uint8_t ADC_Channel_x)      
**�������� ������ADC1����ת��ģʽ
**������� ��ADC_Channel_x , xΪ0~11
**������� ����
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


extern int16_t LnowADC[8];           // ��е�ǰADC��ֵ


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
  ADC_InitStructure.ADC_ContinuousConvMode =  DISABLE;//����ת��
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
**������Ϣ ��ADC1_SingleChannel_Get(uint8_t ADC_Channel_x)       
**�������� ����ȡADC1ת������
**������� ��ADC_Channel_x , xΪ0~11
**������� ��puiADDataΪADC������ֵ
********************************************************************************************************/
u16 ADC_Read(uint8_t ch)
{
    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
    ADC_RegularChannelConfig(ADC1,ch, 1, ADC_SampleTime_1Cycles5);  //ADC1,ADCͨ��,����ʱ��Ϊ1.5����
    ADC1->CTLR2 |= CTLR2_EXTTRIG_SWSTART_Set;                               //ʹ��ָ����ADC1�����ת����������
    while((ADC1->STATR & ADC_FLAG_EOC) == (uint8_t)RESET);                  //�ȴ�ת������
    return ((ADC1->RDATAR));     // �ֱ���12λ
    //return ((ADC1->RDATAR)>>2);// �ֱ���10λ
    //return ((ADC1->RDATAR)>>4);// �ֱ���8λ
}

/********************************************************************************************************
**������Ϣ ��Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)     
**�������� ����ȡ����ADC1����ֵ��ƽ��ֵ
**������� ��ADC_Channel_x , xΪ0~8
**������� ��puiADDataΪADC������ֵ
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
*  �������ƣ�void TestADC(void)
*  ����˵����ADC�ɼ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��   ADC���ղ�Ϊ�㣬�����
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


  /*TFTSPI_Init(1);        //LCD��ʼ��  0:����  1������
  TFTSPI_CLS(u16BLUE);   //��ɫ��Ļ	
  TFTSPI_P8X16Str(0,1,"LQ ADC demo",u16WHITE,u16BLACK);     //�ַ�����ʾ*/


  LnowADC[0] = ADC_Read(0);  // ��ǰ���1����У�������ƽ�У��ɼ�������е�ADֵ
  LnowADC[1] = ADC_Read(1);  // ��ǰ���2����У��봹ֱ������
  LnowADC[2] = ADC_Read(4);  // ��ǰ���2����У��봹ֱ������
  LnowADC[3] = ADC_Read(5);  // ��ǰ���1����У�������ƽ�У�
  LnowADC[4] = ADC_Read(6);  // �����1����У�������ƽ�У�
  LnowADC[5] = ADC_Read(7);  // �����2����У���������ֱ
  LnowADC[6] = ADC_Read(8);  // �Һ���2����У���������ֱ��
  LnowADC[7] = ADC_Read(9);  // �Һ���1����У�������ƽ��
  BatVolt    = ADC_Read(10);  // ˢ�µ�ص�ѹ

  if (LnowADC[0] < ad_min[0])        ad_min[0] = LnowADC[0];     // ˢ����Сֵ
  else if (LnowADC[0] > ad_max[0])   ad_max[0] = LnowADC[0];     // ˢ�����ֵ

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

  // ��ƫ������һ����0--100����
  LleftFP  = (LnowADC[0] - ad_min[0]) * 100 / (ad_max[0] - ad_min[0]);  //��ǰ1
  LleftFV  = (LnowADC[1] - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);  //��ǰ2
  LrightFV = (LnowADC[2] - ad_min[2]) * 100 / (ad_max[2] - ad_min[2]);  //��ǰ2
  LrightFP = (LnowADC[3] - ad_min[3]) * 100 / (ad_max[3] - ad_min[3]);  //��ǰ1

  LleftBP = (LnowADC[4] - ad_min[4]) * 100 / (ad_max[4] - ad_min[4]);   //���1
  LleftBV = (LnowADC[5] - ad_min[5]) * 100 / (ad_max[5] - ad_min[5]);   //���2
  LrightBV = (LnowADC[6] - ad_min[6]) * 100 / (ad_max[6] - ad_min[6]);  //�Һ�2
  LrightBP = (LnowADC[7] - ad_min[7]) * 100 / (ad_max[7] - ad_min[7]);  //�Һ�1


      //��ʾ����й�һ�����ƫ����  ��ǰ����е�ѹֵ ����п�������ʷ��Сֵ ����п�������ʷ���ֵ


    /*vbat=ADC_Read(ADC_Channel_10);
    vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7
    sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3
    UART_PutStr(USART1,txt);
    TFTSPI_P8X16Str(3, 3, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    vbat=ADC_Read(ADC_Channel_4);   
    vbat = vbat * 2 / 25;  // x/4095*3.3*100
    sprintf(txt, "ch4:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095
    TFTSPI_P8X16Str(3, 4, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
    UART_PutStr(USART1,txt);
    vbat=ADC_Read(ADC_Channel_5);   
    vbat = vbat * 2 / 25;  // x/4095*3.3*100
    sprintf(txt, "ch5:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095
    TFTSPI_P8X16Str(3, 5, txt, u16PURPLE, u16BLACK);   // �ַ�����ʾ
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



